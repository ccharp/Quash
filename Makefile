QUASH: main.o Quash.o Utilities.o
	$(CPP) -std=c++0x -g main.o Quash.o Utilities.o -o Quash
main.o: main.cpp
	$(CPP) -std=c++0x -c -g main.cpp
Quash.o: Quash.h Quash.cpp Utilities.h
	$(CPP) -std=c++0x -c -g Quash.h Quash.cpp 
Utilities.o: Utilities.h Utilities.cpp
	$(CPP) -std=c++0x -c -g Utilities.h Utilities.cpp 

clean: 
		rm *.o *.gch *.swp *~
