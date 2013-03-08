#include <iostream>

using namespace std;

struct Process {
	int pid;
	char **args;

};

class Job {
	privae:
		vector<Process> processes;
				
		int numPipes;
		char redirection;
		char *redirFile;

		bool background;

	public:
		void execute();
};
