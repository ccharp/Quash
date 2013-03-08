#ifndef QUASH_HPP
#define QUASH_HPP


#include <iostream>

using namespace std;

enum QuashCmds {
	NOT_QUASH_CMD = 0,
	CD,
	SET,
	EXIT,
	QUIT,
	JOBS
};

class Quash {
	private:
		void printPrompt();

		Job parseInput(string input); 

		QuashCmds isShellCommand(string input);

		vector<string> tokenize(string str, char delimeter);


	public:
		void mainLoop();

};
	

#endif
