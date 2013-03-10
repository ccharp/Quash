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
//   cout << "PATH = " << getenv("PATH") << endl;
   Quash quash = Quash(envp);
    quash.startMainLoop(); 
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



















