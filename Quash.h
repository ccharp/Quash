#ifndef QUASH_HPP
#define QUASH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "Utilities.h"

#define DEBUG 0

using namespace std;

struct Process { 
	pid_t pid;	
	char **argv;

	FILE *inputFile;
	FILE *outputFile;
	
	Process() {
		pid = -1;
		inputFile = stdin;
		outputFile = stdout;
	}

	void print() {
		cout << "  Print Process: \n";
		cout << "  pid = " << pid << endl;
		cout << "  argv =";

		for(unsigned int i = 0; argv[i] != NULL; i++) {
			cout << " " << argv[i];	
		}
	}

	~Process() {
		if(STDIN_FILENO != fileno(inputFile)) {
			fclose(inputFile);
		}
		if(STDOUT_FILENO != fileno(outputFile)) {
			fclose(outputFile); 
		}
		
		if(argv) {
			for(unsigned int i = 0; argv[i] != NULL; i++) {
				delete []argv[i];	
			}	
			
			delete []argv;
		}
	}
};

struct Job {
	vector<Process*> processes;
	bool runInBackground; 

	Job() {
		runInBackground = false;	
	}

	void print() {
		cout << "Print Job: \n";
		cout << " runInBackGround = " << runInBackground << endl;

		for(unsigned int i = 0; i < processes.size(); i++) {
			processes[i]->print();	
		}
	}
};

enum QuashCmds {
	NOT_QUASH_CMD = 0,
	CD,
	SET,
	EXIT,
	QUIT,
	JOBS
};

class Quash {
	private: // Member Functions
		void printPrompt();
		
		Process *parseProcess(const string input);

		Job *parseJob(const string input); 

		QuashCmds isShellCommand(const Process process);

		void execute(const Job *job); 

		void mainLoop();

		void executeQuashCommand(QuashCmds quashCmd, const Process process); 

		void executeCd(Process process);
		void executeSet(Process process);
		void executeExit(Process process);
		void executeJobs(Process Process);
		

	private: // Member Variables

		// Some data structure to track jobs

		char **mEnv;
		
		string currDir;

	public:
		Quash(
			char **&aEnv
		); 
		
		void startMainLoop();
		
		int executeCommands(int argc, char **argv); 
};
	

#endif
