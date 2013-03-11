#include <iostream>

#include "Quash.h"

using namespace std;

int main(int argc, char **argv, char **envp) {
	Quash quash = Quash(envp);
	quash.startMainLoop(); 

	return EXIT_SUCCESS;
}

