CC = gcc
CFLAGS= -Wall -Wextra -std=c99
LIB= player1.so player2.so player3.so
SRC=./src
INSTALL=./install
DIR=-I. -I../ -I./src/ -I./test
DEBUG= -g



build: $(SRC)/server.c $(LIB)
	$(CC) $(CFLAGS) $(DIR) $< -o server -ldl $(DEBUG)

test:

install:
	cp $(SRC)/*.so $(INSTALL)/
	cp server $(INSTALL)/

play:
	make build
	make install
	./server ./install/player1.so ./install/player2.so

player1.o: $(SRC)/player1.c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/player1.o

player2.o: $(SRC)/player2.c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/player2.o

player3.o: $(SRC)/player3.c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/player3.o

player1.so: player1.o
	$(CC) $(SRC)/player1.o -shared -o $(SRC)/player1.so

player2.so: player2.o
	$(CC) $(SRC)/player2.o -shared -o $(SRC)/player2.so

player3.so: player3.o
	$(CC) $(SRC)/player3.o -shared -o $(SRC)/player3.so

clean:
		rm ./src/*.o
		rm ./src/*.so
		rm install/*
		rm server

.PHONY: clean install
