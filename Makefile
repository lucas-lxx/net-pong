CC = g++
CFLAGS = -Wall -g

main: main.cpp
	$(CC) $(CFLAGS) -o main.out main.cpp
