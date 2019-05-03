PLAYER1=playerRandom
PLAYER2=playerKevin
PLAYER3=playerMinMax
CC = gcc
CFLAGS= -Wall -Wextra -std=c99
LIB= $(PLAYER1).so $(PLAYER2).so
SRC=./src
INSTALL=./install
DIR=-I. -I../ -I./src/ -I./test
DEBUG= -g -O0

build:
	make main.o

test: $(SRC)/server_tests.c $(SRC)/player_test.o $(SRC)/bitboard_test.o $(SRC)/server.o $(SRC)/$(PLAYER1).c $(SRC)/bitboard.o  $(LIB)
	$(CC) $(CFLAGS) $(DIR) $< $(SRC)/server.o $(SRC)/player_test.o $(SRC)/bitboard_test.o -o server_tests $(SRC)/$(PLAYER1).c $(SRC)/bitboard.o -ldl $(DEBUG)
	cp $(SRC)/*.so $(INSTALL)/
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

player_test.o: $(SRC)/player_test.c $(SRC)/player_test.h $(SRC)/bitboard_move.h $(SRC)/bitboard_player.h $(SRC)/$(PLAYER2).c
	$(CC) $(CFLAGS) -c $<

bitboard_test.o: $(SRC)/bitboard_test.c $(SRC)/bitboard.c
	$(CC) $(CFLAGS) -c $<

$(PLAYER1).o: $(SRC)/$(PLAYER1).c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/$(PLAYER1).o

$(PLAYER2).o: $(SRC)/$(PLAYER2).c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/$(PLAYER2).o

$(PLAYER3).o: $(SRC)/$(PLAYER3).c
	$(CC) $(CFLAGS) -c $< -fPIC -o $(SRC)/$(PLAYER3).o

$(PLAYER1).so: $(PLAYER1).o
	$(CC) $(SRC)/$(PLAYER1).o -shared -o $(SRC)/$(PLAYER1).so

$(PLAYER2).so: $(PLAYER2).o
	$(CC) $(SRC)/$(PLAYER2).o -shared -o $(SRC)/$(PLAYER2).so

$(PLAYER3).so: $(PLAYER3).o
	$(CC) $(SRC)/$(PLAYER3).o -shared -o $(SRC)/$(PLAYER3).so

clean:
		rm -f ./src/*.o
		rm -f ./src/*.so
		rm -f ./src/*.gch
		rm -f install/*
		rm -f server
		rm -f server_tests

.PHONY: clean install
