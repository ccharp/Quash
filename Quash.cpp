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

#include "Job.h"
#include "Quash.h"
#include "utilities.h"

using namespace std;


void Quash::mainLoop() {
	while(1) {
		printPrompt();

		string input;
		getline(cin, input);


		

		execute(args);
	}
}

void startMainLoop() {
	mainLoop();
}

void execute(const char **argv) {
	
	if(!args) {
		cout << "Error in mainLoop\n";
		continue; 
	}

	// No piping or complex jobs yet, so no need for parsing. 	

	QuashCmds quashCmd;
	if((quashCmd = isShellcommand(args)) != NOT_QUASH_CMD) {
		executeQuashCommand(quashCmd, args);	
	} else {
		Job job = parseJob(args);	
		executeJob(job); 	
	}
}

void Quash::executeQuashCommand(QuashCmds quashCmd, char **args) {
	switch(quashCmd) {
		case: CD
			executeCd(args);
		case: SET
			executeSet(args);
		case: EXIT
		case: QUIT
			executeExit(args); 
		case: JOBS
			executeJobs(args); 
		default:
			cerr << "Problem in executeQuashCommand\n";
			
}

// Parses a potentially complex (thanks to pipes and redirects) into a Job, a set
// of connected processes
Job Quash::parseJob(const string input) {
		char **args = NULL;
		argify(tokenize(input, ' '), args); 

}

void Quash::printPrompt() {
	char *cwd = get_current_dir_name(); 
	cout << "Quash " << cwd << "\n$ ";	
	delete []cwd;
}

QuashCmds Quash::isShellCommand(const char const **args) {
	bool retVal = NOT_QUASH_CMD;

	if("cd" == args[0])
		retVal = CD;
	else if("set" == args[0])
		retVal = SET;
	else if("exit" == args[0])
		retVal = EXIT;
	else if("quit" == args[0])
		retVal = QUIT;
	else if("jobs" == args[0])
		retVal = JOBS;
	
	return retVal;	
}













































#endif
