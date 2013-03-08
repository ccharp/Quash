#ifndef QUASH_HPP
#define QUASH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "utilities.h"

using namespace std;

struct Process { 
	pid_t pid;	
	char **argv

	//input file
	//output file
}

struct Job {
	vector<Process> processes;
	unsigned int numPipes;	
	bool runInBackground; 
}

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

		Job parseJob(string input); 

		QuashCmds isShellCommand(string input);

		void execute(const char **argsv); 

		void mainLoop();

		void executeQuashCommand(QuashCmds quashCmd, char **args); 

		void executeCd(char **args);
		void executeSet(char **args);
		void executeExit(char **args);
		void executeJobs(char **args);

	private: // Member Variables

		// Some data structure to track jobs

		char **env;

	public:
		Quash(
			char **aEnv,
		); 
		
		void startMainLoop();
};
	

#endif
