CC = gcc
CFLAGS= -Wall -Wextra -std=c99
LIB= joueur1.so joueur2.so joueur3.so
SRC=./src
INSTALL=./install
DIR=-I. -I../ -I./src/ -I./test

build: $(SRC)/server.c $(LIB)
	$(CC) $(CFLAGS) $< -o server -ldl

test:

install:
	cp $(SRC)/*.so $(INSTALL)/
	cp server $(INSTALL)/

joueur1.o: $(SRC)/joueur1.c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/joueur1.o

joueur2.o: $(SRC)/joueur2.c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/joueur2.o

joueur3.o: $(SRC)/joueur3.c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/joueur3.o

joueur1.so: joueur1.o
	$(CC) $(SRC)/joueur1.o -shared -o $(SRC)/joueur1.so

joueur2.so: joueur2.o
	$(CC) $(SRC)/joueur2.o -shared -o $(SRC)/joueur2.so

joueur3.so: joueur3.o
	$(CC) $(SRC)/joueur3.o -shared -o $(SRC)/joueur3.so

clean:
		rm ./src/*.o
		rm ./src/*.so
		rm install/*
		rm server

.PHONY: clean install
