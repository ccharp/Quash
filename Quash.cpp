#ifndef QUASH_CPP
#define QUASH_CPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#include "Quash.h"
#include "Utilities.h"

using namespace std;

// Global to track background jobs
map<pid_t, JobIdentifier> backGroundJobs;
pid_t currProcess;

Quash::Quash(char **&aEnv) {
	mEnv = aEnv;
	mJobid = 0;
	currDir = (string)get_current_dir_name();

	// Initialize ignal handling
	struct sigaction action; 

	memset(&action, 0, sizeof(action));
	action.sa_handler = signalHandler;

	if(sigaction(SIGCHLD, &action, 0)) {
		cerr << "Error in initSignals\n";
		exit(0);
	}

	signal(SIGINT, signalHandler); 
}

void Quash::mainLoop() {
	string input;
	bool hasInfile = !isatty(STDIN_FILENO);
	bool promptPrinted = false;

	do{
		if(!cin.good()) {
			cin.clear();
			continue;	
		}
		// If commands were passed in
	   	// execute them then terminate, and do not print prompt
		if(!hasInfile && !promptPrinted) {
	        printPrompt();
			promptPrinted = true;
	    }
			
		if(getline(cin, input) && cin.good()) { 
	   		Job *job;
			
			promptPrinted = false;
			if(input.empty() || (job = parseJob(input)) == NULL) {
				continue;	
			}
			
			executeJob(job);
	    } 
		else if (hasInfile) {
	    	break;
	  	}
#if DEBUG
		else {
			cout << "GETLINEFAILED!\n";
			cout.flush();
		}
#endif
	}while(1);
}

void Quash::startMainLoop() {
	mainLoop();
}

void Quash::executeJob(const Job *job) {

	int prevPipe[2];
	prevPipe[0] = STDIN_FILENO;
	prevPipe[1] = STDOUT_FILENO;

	unsigned int numProcesses = job->processes.size();
	for(unsigned int i = 0; i < numProcesses; i++) {

		Process *process = job->processes[i];	
		int pipes[2];

		pipe(pipes);

		QuashCmds quashCmd;
		if((quashCmd = isQuashCommand(process)) != NOT_QUASH_CMD) {
			
			executeQuashCommand(quashCmd, process);	
			
		// Else it's a binary that needs executed.
		} else { 
			bool binExists = false;
			
			// if it's an absolute path
			if('/' == process->argv[0][0]) {
				binExists = fileExists(process->argv[0]);		
			} 
			// if it's a local/relative path 
			else if(strncmp("./", process->argv[0], 2) == 0) {
				// Trim the "./" off of the process->argv[0]
				char *execPath = process->argv[0];
				memmove(execPath, execPath + 2, strlen(execPath) - 1);
				
				binExists = fileExists(execPath);
			}
			// if we need to look though PATH
			else {
				// if the binary exists, it's path will be set to process->argv[0];
				binExists = findPath(process->argv[0]);	
			}	
			
			////////////////////////////////////////////////////////////
			/////////// FORK AND EXEC //////////////////////////////////
			////////////////////////////////////////////////////////////
			if(binExists) {
				
				// Orginal had this abstracted to a function call, but inlining it 
				// makes piping a bit easier
				pid_t pid = fork();
				switch(pid) {
					case FAILURE:
						cerr << "Problem forking\n";
						break;
					case CHILD:
					{
						redirectFiles(process->inputFile, process->outputFile);
						
						// if this is the first process but not the last
						if(i == 0 && i < numProcesses - 1) {
							dup2(pipes[1], STDOUT_FILENO);
						} 
						// else if it's not the first AND not the last process...
						else if (i != numProcesses - 1) {
							dup2(prevPipe[0], STDIN_FILENO);
							dup2(pipes[1], STDOUT_FILENO);
						}
						// else if this is the last process but not the first 
						else if(i != 0 && i == numProcesses - 1) {
							dup2(prevPipe[0], STDIN_FILENO);	
						}
						
						close(pipes[0]);
						close(pipes[1]);
						
						if(job->runInBackground) {
							// Put this process in it's own group
							setpgid(0, 0);	
						}
						
						// Exec dat mofo
						if(execve(process->argv[0], process->argv, mEnv) == FAILURE) {
							cerr << "PROBLEM EXECUTING: " << process->argv[0] << endl;
							exit(0);	
						}
					}
					default: // Parent
						process->pid = pid;
				}

			} else {
				cerr << "Quash: " << process->argv[0] << ": command not found...\n";
			}
			
			// If this is the last process...
			if(i == numProcesses - 1) {
				if(job->runInBackground) {
					printf("[%i] %i running in background\n", job->jobID.jobid, process->pid);
					backGroundJobs[process->pid] = job->jobID;	
				} else {
					currProcess = process->pid;
					wait(NULL);	
					currProcess = 0;
				}
			}
		}
		
		prevPipe[1] = pipes[1];
		prevPipe[0] = pipes[0];
		
		close(pipes[1]);
	} /* END FOR */

	//close(prevPipe[0]);
	//close(prevPipe[1]);
	dup2(STDIN_FILENO, STDIN_FILENO);	
	dup2(STDOUT_FILENO, STDOUT_FILENO);
}

// Searches through PATH, looking for the process->argv[0]
bool Quash::findPath(char *&execPath) {
	// Ex: PATH=/usr/bin:/some/other
	string PATH = string(getenv("PATH"));
	vector<string> pathToks = tokenize(PATH.substr(5), ':'); 

	for(string path : pathToks) {
		path += "/" + string(execPath); 
		
		if(fileExists(path.c_str())) {
			char *foundPath = new char[path.length() + 1];
			strcpy(foundPath, path.c_str());
			
		delete []execPath;
			execPath = foundPath;
			
			return foundPath;
		}
	}

	return NULL;
}

void Quash::executeQuashCommand(
	QuashCmds quashCmd, 
	const Process * const process
	) 
	{
	switch(quashCmd) {
		case CD:
			executeCd(process);
			break;
		case SET:
			executeSet(process);
			break;
		case EXIT:
		case QUIT:
			executeExit(process); 
			break;
		case JOBS:
			executeJobs(process); 
			break;
		default:
			cerr << "Problem in executeQuashCommand\n";
	}
}

int Quash::executeProcess(Process * const process, bool runInBackground) {
	pid_t pid = fork();

	switch(pid) {
		case FAILURE:
			cerr << "Problem forking\n";
			break;
		case CHILD:
			redirectFiles(process->inputFile, process->outputFile);
			
			if(runInBackground) {
				// Put it in it's own group
				setpgid(0, 0);	
			}
			
			if(execve(process->argv[0], process->argv, mEnv) == FAILURE) {
				cerr << "PROBLEM EXECUTING: " << process->argv[0] << endl;
				exit(0);	
			}
		default: // Parent
			process->pid = pid;
	}

	return 1;
}

void Quash::redirectFiles(FILE *input, FILE *output) {
	
	if(fileno(input) != STDIN_FILENO) {
		dup2(fileno(input), STDIN_FILENO);	
	}

	if(fileno(output) != STDOUT_FILENO) {
		dup2(fileno(output), STDOUT_FILENO);
	}
}

Process *Quash::parseProcess(const string input) {
	Process *process = new Process();

	vector<string> tokProcess = tokenize(input, ' ');
	vector<string> tokArgs;

	unsigned int len = tokProcess.size();
	for(int i = 0; i < len; i++) {
		
		// Check for redirection
		if(tokProcess[i] == "<") {
			
			if(i + 1 >= len) {
				cerr << "Bad syntax: '<'\n";
				break;
			}
			
			const char *filename = tokProcess[++i].c_str();
			process->inputFile = fopen(filename, "r"); 
			
			if(process->outputFile == NULL) {
				cerr << "Couldn't open file: " << filename << endl;	
				return NULL;
			}
		} 
		else if(tokProcess[i] == ">") {
		
			if(i + 1 >= len) {
				cerr << "Bad syntax: '>'\n";
				break;
			}
			
			const char *filename = tokProcess[++i].c_str();
			process->outputFile = fopen(filename, "w"); 
			
			if(process->outputFile == NULL) {
				cerr << "Couldn't open file: " << filename << endl;	
				return NULL;
			}
		} 
		// No redirection
		else {
			tokArgs.push_back(tokProcess[i]);
		}
	}
	
	// Returns pointer to C style argument array made from the 
	// argument tokens
	process->argv = argify(tokArgs, process->argv); 

	return process;
}

// Parses complex input (thanks to pipes and redirects) into a Job, a set
// of connected processes
Job *Quash::parseJob(const string input) {
	// Create job and assign unique job ID.
	Job *job = new Job(mJobid++);
	job->jobID.jobTextString = input;
	
	// Tokenize input into individual process
	vector<string> tokProcesses = tokenize(input, '|');

	// Check for '&' in the last process
	int pos;
	string lastStr = tokProcesses[tokProcesses.size() - 1];
	if((pos = lastStr.find("&")) != string::npos) {
		
		// Found a '&'
		job->runInBackground = true; 
		tokProcesses[tokProcesses.size() - 1].erase(pos);
	}
		
	// Parse each process and add it to the job
	for(string strProcess : tokProcesses) {
		Process *process;
		if((process = parseProcess(strProcess)) == NULL) {
			return NULL;	
		}

		job->processes.push_back(process); 	
	}

#if DEBUG
	job->print();
#endif
		
	return job;
}

void Quash::printPrompt() {
	
	// Sleep for 10 milliseconds to prevent a child process's output
	// from printing after the prompt.
	usleep(10 * 1000);

	char *cwd = get_current_dir_name(); 
	cout << "[quash " << cwd << "]\n$ ";	

	delete []cwd;
}

QuashCmds Quash::isQuashCommand(const Process * const process) {
	QuashCmds retVal = NOT_QUASH_CMD;

	if(strcmp("cd", process->argv[0]) == 0) {
		retVal = CD;
	}
	else if(strcmp("set", process->argv[0]) == 0) {
		retVal = SET;
	}
	else if(strcmp("exit", process->argv[0]) == 0) {
		retVal = EXIT;
	}
	else if(strcmp("quit", process->argv[0]) == 0) {
		retVal = QUIT;
	}
	else if(strcmp("jobs", process->argv[0]) == 0) {
		retVal = JOBS;
	}
	
	return retVal;	
}

void Quash::executeCd(const Process * const process) {
	string dir;

    if( process->argv[1] == NULL || process->argv[1] == "HOME" ){
        dir = getenv("HOME");
    } else {
        dir = process->argv[1];
    }

    currDir = dir;
    chdir(dir.c_str());
}

void Quash::executeSet(const Process *  const process) {
	vector<string> strToks = tokenize(string(process->argv[1]), '=');
		
	setenv(strToks[0].c_str(), strToks[1].c_str(), 1);
}

void Quash::executeExit(const Process * const process) {
	exit(EXIT_SUCCESS);
}

void Quash::executeJobs(const Process * const process) {

	if(backGroundJobs.empty()) {
		printf("There are currently no jobs executing in the background\n");	
	}

	for(auto jobIdPair : backGroundJobs) {
		unsigned int jobid = jobIdPair.second.jobid;
		string jobStr = jobIdPair.second.jobTextString;
		
		printf("[%i] %i %s\n", jobid, jobIdPair.second.pid, jobStr.c_str());
	}
}

/*static*/ 
void Quash::signalHandler(int signal) {
	pid_t pid;

	if(signal == SIGINT) {
		printf("\n");

		if(currProcess) {
			kill(currProcess, SIGTERM); 
		} else { // Hack go get the reprint the prompt. static functions suck. 
			char *cwd = get_current_dir_name(); 
			cout << "[quash " << cwd << "]\n$ ";	
			cout.flush();

			delete []cwd;
		}
	}

	while((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
		
		// Check if the key exists. The signal may not be from the job-pid 
		// identifier
		if(backGroundJobs.find(pid) != backGroundJobs.end()) {
			unsigned int jobid = backGroundJobs[pid].jobid;
			string jobStr = backGroundJobs[pid].jobTextString;
			
			printf("[%i] %i finished %s\n", jobid, pid, jobStr.c_str());
			
			backGroundJobs.erase(pid);
		}
	}
}

#endif
