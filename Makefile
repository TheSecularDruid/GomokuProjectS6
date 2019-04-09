PLAYER1=player1
PLAYER2=player2
CC = gcc
CFLAGS= -Wall -Wextra -std=c99
LIB= $(PLAYER1).so $(PLAYER2).so
SRC=./src
INSTALL=./install
DIR=-I. -I../ -I./src/ -I./test
DEBUG= -g -O0

build:
	make main.o

test: $(SRC)/server_tests.c $(SRC)/server.o $(SRC)/bitboard.o $(LIB)
	$(CC) $(CFLAGS) $(DIR) $< $(SRC)/server.o -o server_tests -ldl $(DEBUG)
	./server_tests

install:
	cp $(SRC)/*.so $(INSTALL)/
	cp server $(INSTALL)/

play:
	make build
	make install
	./server ./install/$(PLAYER1).so ./install/$(PLAYER2).so -n 6 

main.o: $(SRC)/main.c $(SRC)/server.c $(SRC)/bitboard.o $(LIB)
	$(CC) $(CFLAGS) $(DIR) $< $(SRC)/server.c $(SRC)/bitboard.o -o server -ldl $(DEBUG)

server.o: $(SRC)/server.c $(SRC)/server.h
		$(CC) $(CFLAGS) -c $<

bitboard.o: $(SRC)/bitboard.c $(SRC)/bitboard.h
	$(CC) $(CFLAGS) -c $<

$(PLAYER1).o: $(SRC)/$(PLAYER1).c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/$(PLAYER1).o

$(PLAYER2).o: $(SRC)/$(PLAYER2).c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/$(PLAYER2).o


$(PLAYER1).so: $(PLAYER1).o
	$(CC) $(SRC)/$(PLAYER1).o -shared -o $(SRC)/$(PLAYER1).so

$(PLAYER2).so: $(PLAYER2).o
	$(CC) $(SRC)/$(PLAYER2).o -shared -o $(SRC)/$(PLAYER2).so


clean:
		rm ./src/*.o
		rm ./src/*.so
		rm install/*
		rm server

.PHONY: clean install
