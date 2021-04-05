CC = g++
CFLAGS = -g -w -Wall

gamelogic: gamelogic.o game.o inputs.o utils.o RS232.o Graphics.o Wifi.o ai_firm.o
	$(CC) $(CFLAGS) -o gamelogic gamelogic.o game.o inputs.o utils.o RS232.o Graphics.o Wifi.o ai_firm.o

utilstest: utilstest.o utils.o
	$(CC) $(CFLAGS) -o utilstest utilstest.o utils.o

utils.o: utils.h utils.cpp
	$(CC) $(CFLAGS) -c utils.cpp

game.o: game.h game.cpp
	$(CC) $(CFLAGS) -c game.cpp

inputs.o: inputs.h inputs.cpp
	$(CC) $(CFLAGS) -c inputs.cpp

RS232.o: RS232.h RS232.c
	$(CC) $(CFLAGS) -c RS232.c

Graphics.o: Graphics.h Graphics.c
	$(CC) $(CFLAGS) -c Graphics.c

Wifi.o: Wifi.h Wifi.c
	$(CC) $(CFLAGS) -c Wifi.c

ai_firm.o: ai_firm.h ai_firm.c
	$(CC) $(CFLAGS) -c ai_firm.c

clean:
	rm -f core *.o gamelogic utilstest