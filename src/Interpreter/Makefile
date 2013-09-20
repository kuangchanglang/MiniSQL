minisql : main.o interpreter.o
		g++ -o minisql main.o interpreter.o -g
main.o  : main.cpp interpreter.h
		g++ -c main.cpp -g
interpreter.o : interpreter.cpp interpreter.h
		g++ -c interpreter.cpp -g

clean:
		rm *.o minisql
