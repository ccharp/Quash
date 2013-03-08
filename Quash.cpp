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

using namespace std;


void Quash::mainLoop() {
	while(1) {
		printPrompt();

		string input;
		getline(cin, input);

		// Tokenize input on spaces 
		char **args = NULL;
		tokenize(input, args); 

		if(!args) {
			cout << "Error in mainLoop\n";
			continue; 
		}

		// Determine if the command is a binary to be executed 
		// or a shell command
		if(isShellcommand(args)) {
			
		} else {
			Job job = parseJob(input);	
			job.execute(); 	
		}	
	}
}

Job Quash::parseJob(const string input) {
		
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
