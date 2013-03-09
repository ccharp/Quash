#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "Quash.h"

using namespace std;

int main(int argc, char **argv, char **envp) {
	Quash quash = Quash(envp);
	
	// If commands were passed in, execute them then terminate. 
	if(0) { // Don't know how to check to input from stdin without pausing. 
		quash.executeCommands(argc, argv); 	
	} else {
		quash.startMainLoop(); 
	}
	
	return EXIT_SUCCESS;
}

/* ./binary < input > output 
 *
 * ls | grep foo | head > bar.txt &
 *
 * 
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */



















