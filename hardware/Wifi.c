#include "Wifi.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/**************************************************************************
/*  Subroutine to initialise the RS232 Port by writing some data
**  to the internal registers.
**  Call this function at the start of the program before you attempt
**  to read or write to data via the RS232 port
**
**  Refer to UART data sheet for details of registers and programming
***************************************************************************/

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 100 * number_of_seconds;
	// Storing start time
	clock_t start_time = clock();
	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds) ;
}

void printdecimal(int x) 
{ 
  int digit = x % 10; 
  x = x/10; 
  if (x != 0) printdecimal(x);
  char c = digit + '0'; 
  putcharWIFI(c);
} 

void print_to_board(int p1, int p2, int win, int score1, int score2) {
    int args[5] = {p1, p2, win, score1, score2};
    int i = 0;
    int y = 0;
    while (upload_match_results[i] != '\0') {
        if (upload_match_results[i] == '~') {
            printdecimal(args[y]);
            y++;
        } else {
            putcharWIFI(upload_match_results[i]);
        }
        i++;
    }
    //putcharWIFI('\n');

}

void Init_RS232_WIFI(void)
{
    // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    set_bit(WIFI_LineControlReg, 7);
    // set Divisor latch (LSB and MSB) with correct value for required baud rate
    int divisor_value = BR_CLK / (DESIRED_BAUD_RATE * 16);
    *WIFI_DivisorLatchLSB = (divisor_value & 0xFF);
    *WIFI_DivisorLatchMSB = (divisor_value & 0xFF00) >> 8;
    // set bit 7 of Line control register back to 0 and
    clear_bit(WIFI_LineControlReg, 7);
    // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *WIFI_LineControlReg |= 0x3; // 8 bit data
    clear_bit(WIFI_LineControlReg, 2); // 1 stop bit
    clear_bit(WIFI_LineControlReg, 3); // no parity
    // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    *WIFI_FifoControlReg |= 0x6;
    // Now Clear all bits in the FiFo control registers
    *WIFI_FifoControlReg = 0;
}

int putcharWIFI(int c)
{
    // wait for Transmitter Holding Register bit (5) of line status register to be '1'
    // indicating we can write to the device
    // write character to Transmitter fiforegister
    // return the character we printed
    while (!(*WIFI_LineStatusReg & (1 << 5)));
    *WIFI_TransmitterFifo = c;
    return c;
}

int getcharWIFI(void)
{
    // wait for Data Ready bit (0) of line status register to be '1'
    // read new character from ReceiverFiFo register
    // return new character
    while (!WIFITestForReceivedData());
    return *WIFI_ReceiverFifo;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one,or read it, it simply tests
// to see if one is available to read from the FIFO
int WIFITestForReceivedData(void)
{
    // if WIFI_LineStatusReg bit 0 is set to 1
    // return TRUE, otherwise return FALSE
    return (*WIFI_LineStatusReg) & 1;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//
void WIFIFlush(void)
{
    unsigned char unwanted_char;
    // read while bit 0 of Line Status Register == ‘1’
    while (WIFITestForReceivedData()) {
        unwanted_char = *WIFI_ReceiverFifo;
    }
}

void Init_Wifi(void) {
    int i = 0;
    Init_RS232_WIFI();
    WIFIFlush();

    *WIFI_RESET = 0;
    //DELAY FOR RESET
    delay(2);

    *WIFI_RESET = 1;
    //DELAY FOR RESET OFF INIT SEQUENCE
    delay(2);
    WIFIFlush();
    
    //INITIALIZE THE FILE
    i = 0;
    while (dofile[i] != '\0') {
        putcharWIFI(dofile[i]);
        i++;
    }
    //CONFIRM FILE LOAD
    delay(2);
    WIFIFlush();  
}

void postgameresults(int p1, int p2, int winner, int score1, int score2) {
    print_to_board(p1, p2, winner, score1, score2);
    int i = 0;
    char results[256];

    //DELAY FOR CONFIRM
    delay(2);

    // KEEP HERE IN EVENT WE NEED RESULT
    // for (i = 0; i < 256; i++) {
    //     hello = getcharWIFI();
    //     results[i] = hello;
    // }
    
    WIFIFlush();
}
