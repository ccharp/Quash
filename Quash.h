#ifndef QUASH_HPP
#define QUASH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "Utilities.h"

using namespace std;

struct Process { 
	pid_t pid;	
	char **argv;

	FILE *inputFile;
	FILE *outputFile;
	
	Process() {
		pid = -1
		inputFile = stdin;
		outputFile = stdout;
	}
};

struct Job {
	vector<Process> processes;
	bool runInBackground; 

	Job() {
		runInBackground = false;	
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
		
		Process parseProcess(const string input);

		Job parseJob(const string input); 

		QuashCmds isShellCommand(const Process process);

		void execute(Job job); 

		void mainLoop();

		void executeQuashCommand(QuashCmds quashCmd, const Process process); 

		void executeCd(Process process);
		void executeSet(Process process);
		void executeExit(Process process);
		void executeJobs(Process Process);

	private: // Member Variables

		// Some data structure to track jobs

		char **mEnv;

	public:
		Quash(
			char **&aEnv
		); 
		
		void startMainLoop();
		
		int executeCommands(int argc, char **argv); 
};
	

#endif
