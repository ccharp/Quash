#ifndef QUASH_CPP
#define QUASH_CPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "Quash.h"
#include "Utilities.h"

using namespace std;

Quash::Quash(char **&aEnv) {
	mEnv = aEnv;	
}

void Quash::mainLoop() {
	while(1) {
		printPrompt();

		string input;
		getline(cin, input);

		Job job = parseJob(input);	

		execute(job);
	}
}

void Quash::startMainLoop() {
	mainLoop();
}

int Quash::executeCommands(int argc, char **argv) {
	
}

void Quash::execute(Job job) {
	/*for (Process process : job.processes )
	{
	//	if(!argv) {
	//		cout << "Error in mainLoop\n";
	//		continue; 
	//	}

		// No piping or complex jobs yet, so no need for parsing. 	

		QuashCmds quashCmd;
		if((quashCmd = isShellcommand(argv)) != NOT_QUASH_CMD) {
			executeQuashCommand(quashCmd, argv);	
		} else {
			Job job = parseJob(argv);	
			executeJob(job); 	
		}
	}*/
}

void Quash::executeQuashCommand(QuashCmds quashCmd, const Process process) {
	switch(quashCmd) {
		case CD:
			executeCd(process);
		case SET:
			executeSet(process);
		case EXIT:
		case QUIT:
			executeExit(process); 
		case JOBS:
			executeJobs(process); 
		default:
			cerr << "Problem in executeQuashCommand\n";
	}
			
}

Process Quash::parseProcess(const string input) {
	Process process;

	vector<string> tokProcess = tokenize(input, ' ');
	vector<string> tokArgs;

	unsigned int len = tokProcess.size();
	for(int i = 0; i < len; i++) {
		if(tokProcess[i] == "<") {
			
			if(i + 1 >= len) {
				cerr << "Bad syntax: '<'\n";
				break;
			}
			
			const char *filename = tokProcess[++i].c_str();
			process.inputFile = fopen(filename, "r"); 
		} 
		else if(tokProcess[i] == ">") {
		
			if(i + 1 >= len) {
				cerr << "Bad syntax: '>'\n";
				break;
			}
			
			char const *filename = tokProcess[++i].c_str();
			process.inputFile = fopen(filename, "w"); 
		} else {
			tokArgs.push_back(tokProcess[i]);
		}
	}
	
	argify(tokArgs, process.argv); 

	return process;
}

// Parses a potentially complex (thanks to pipes and redirects) into a Job, a set
// of connected processes
Job Quash::parseJob(const string input) {
	Job job;
	
	// Tokenize input into individual process
	vector<string> tokProcesses = tokenize(input, '|');

	// Check for "&"
	int pos;
	string lastStr = tokProcesses[tokProcesses.size() - 1];
	if((pos = lastStr.find("&")) != string::npos) {
		job.runInBackground = true; 
		tokProcesses[tokProcesses.size() - 1].erase(pos);
	}
		
	for(string strProcess : tokProcesses) {
		Process process = parseProcess(strProcess); 

		job.processes.push_back(process); 	
	}
		
	return job;
}

void Quash::printPrompt() {
	char *cwd = get_current_dir_name(); 
	cout << "Quash " << cwd << "\n$ ";	

	delete []cwd;
}

QuashCmds Quash::isShellCommand(const Process process) {
	QuashCmds retVal = NOT_QUASH_CMD;

	if("cd" == process.argv[0])
		retVal = CD;
	else if("set" == process.argv[0])
		retVal = SET;
	else if("exit" == process.argv[0])
		retVal = EXIT;
	else if("quit" == process.argv[0])
		retVal = QUIT;
	else if("jobs" == process.argv[0])
		retVal = JOBS;
	
	return retVal;	
}

void Quash::executeCd(Process process) {

}

void Quash::executeSet(Process process) {

}

void Quash::executeExit(Process process) {

}

void Quash::executeJobs(Process process) {

}













































#endif
