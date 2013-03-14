QUASH: main.o Quash.o Utilities.o
	g++-4.7 -std=c++11 -g main.o Quash.o Utilities.o -o Quash
main.o: main.cpp
	g++-4.7 -std=c++11 -c -g main.cpp
Quash.o: Quash.h Quash.cpp Utilities.h
	g++-4.7 -std=c++11 -c -g Quash.h Quash.cpp
Utilities.o: Utilities.h Utilities.cpp
	g++-4.7 -std=c++11 -c -g Utilities.h Utilities.cpp


clean:
		rm -f *.o *.gch *.swp *~
