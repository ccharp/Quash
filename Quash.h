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
		cout << endl;
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

enum Forked {
	FAILURE = -1,
	CHILD = 0,
};

class Quash {
	private: // Member Functions
		void printPrompt();
		
		void mainLoop();

		Process *parseProcess(const string input);

		Job *parseJob(const string input); 

		QuashCmds isQuashCommand(const Process * const process);

		void execute(
			const Job * const job
		); 

		void executeQuashCommand(
			QuashCmds quashCmd, 
			const Process * const process
		);
		
		int executeBinary(
			Process * const process
		);

		void executeCd(
			const Process * const process
		);
		
		void executeSet(
			const Process * const process
		);
		
		void executeExit(
			const Process * const process
		);
		
		void executeJobs(
			const Process * const process
		);
		
		bool fileExists(const char *path);
				
		bool findPath(char *&execPath);		
		
		void initSignals();
		
		static void signalHandler(int signal);
		

	private: // Member Variables

		// Some data structure to track jobs

		char **mEnv;
		
		string currDir;

	public:
		Quash(
			char **&aEnv
		); 
		
		void startMainLoop();
};
	

#endif
