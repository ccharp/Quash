QUASH: main.o Quash.o Utilities.o
	g++ -std=c++0x -g main.o Quash.o Utilities.o -o main 
main.o: main.cpp
	g++ -std=c++0x -c -g main.cpp
Quash.o: Quash.h Quash.cpp Utilities.h
	g++ -std=c++0x -c -g Quash.h Quash.cpp 
Utilities.o: Utilities.h Utilities.cpp
	g++ -std=c++0x -c -g Utilities.h Utilities.cpp 

	
clean: 
		rm *.o *.gch *.swp *~
