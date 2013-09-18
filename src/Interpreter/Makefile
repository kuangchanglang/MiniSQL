minisql : main.o interpreter.o
		g++ -o minisql main.o interpreter.o
main.o  : main.cpp interpreter.h
		g++ -c main.cpp
interpreter.o : interpreter.cpp interpreter.h
		g++ -c interpreter.cpp

clean:
		rm *.o minisql
