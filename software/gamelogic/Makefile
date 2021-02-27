CC = g++
CFLAGS = -g -w -Wall

gamelogic: gamelogic.o game.o inputs.o utils.o
	$(CC) $(CFLAGS) -o gamelogic gamelogic.o game.o inputs.o utils.o 

utilstest: utilstest.o game.o inputs.o utils.o
	$(CC) $(CFLAGS) -o utilstest utilstest.o game.o inputs.o utils.o 

utils.o: utils.h utils.cpp
	$(CC) $(CFLAGS) -c utils.cpp

game.o: game.h game.cpp
	$(CC) $(CFLAGS) -c game.cpp

inputs.o: inputs.h inputs.cpp
	$(CC) $(CFLAGS) -c inputs.cpp

clean:
	rm -f core *.o gamelogic utilstest