FLAGS = -Wall -std=c++17
GXX = g++

main: main.o
	$(GXX) main.o -o main

main.o: main.cpp
	$(GXX) main.cpp -c $(FLAGS)

.PHONY: clean
clean:
	rm *.o main
