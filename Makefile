QUASH: main.o Quash.o
	g++ -std=c++0x -g main.o Quash.o -o main 
main.o: main.cpp
	g++ -std=c++0x -c -g main.cpp
Quash.o: Quash.h Quash.cpp
	g++ -std=c++0x -c -g Quash.h Quash.cpp Job.0
Job.o: Job.h Job.cpp
	g++ -std=c++0x -c -g Job.h Job.cpp 
clean: rm *.o
