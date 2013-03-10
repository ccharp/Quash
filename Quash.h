#ifndef QUASH_HPP
#define QUASH_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
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
		argv = NULL;
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

struct JobIdentifier {
	pid_t pid;
	unsigned int jobid;
	string jobTextString;

	JobIdentifier() {
		pid = -1; 
		jobid = -1;
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


struct Job {
	vector<Process*> processes;
	bool runInBackground; 
	JobIdentifier jobID;

	Job() {
		runInBackground = false;	
	}

	Job(int aJobIdTracker) {
		jobID.jobid = aJobIdTracker;
		runInBackground = false;
	}

	void print() {
		cout << "Print Job: \n";
		cout << " runInBackGround = " << runInBackground << endl;

		for(unsigned int i = 0; i < processes.size(); i++) {
			processes[i]->print();	
		}
	}

	~Job() {
		for(Process *p : processes) {
			delete p;	
		}	
	}
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

		void executeJob(
			const Job * const job
		); 

		void executeQuashCommand(
			QuashCmds quashCmd, 
			const Process * const process
		);
		
		int executeProcess(
			Process * const process,
			bool runInBackground
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
		
		void redirectFiles(FILE *input, FILE *output);
		
		bool fileExists(const char *path);
				
		bool findPath(char *&execPath);		
		
		static void signalHandler(int signal);
		
	private: // Member Variables

		char **mEnv;
		unsigned int mJobid;
		string currDir;
		
	public:
		Quash(
			char **&aEnv
		); 
		
		void startMainLoop();
};
	

#endif
