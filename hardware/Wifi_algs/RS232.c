#include "RS232.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**************************************************************************
/*  Subroutine to initialise the RS232 Port by writing some data
**  to the internal registers.
**  Call this function at the start of the program before you attempt
**  to read or write to data via the RS232 port
**
**  Refer to UART data sheet for details of registers and programming
***************************************************************************/

static const char dofile[] = "dofile(\"send_text_message.lua\")\n";
static const char checkthewifi[] = "check_wifi()\n";

int main(void) {
    int i = 0;
    // *WIFI_ENABLE = 1;
    // for (i = 0; i < 20; i++) {
    //     printf("1\n");
    // }
    Init_RS232();
    RS232Flush();
    *WIFI_RESET = 0;
    char hello;
    for (i = 0; i < 20; i++) {
        printf("1\n");
    }
    *WIFI_RESET = 1;
    for (i = 0; i < 20; i++) {
        printf("2\n");
    }
    RS232Flush();
    
    i = 0;
    while (dofile[i] != '\0') {
        putcharRS232(dofile[i]);
        i++;
    }
    for (i = 0; i < 5; i++) {
        printf("1\n");
    }
    RS232Flush();
    i = 0;
    while (checkthewifi[i] != '\0') {
        putcharRS232(checkthewifi[i]);
        i++;
    }
    hello = 'a';

    while (hello != '\n') {
        hello = getcharRS232();
        if (hello == '\n') {
            printf("Received new line\n");
        } else if (hello == '\r') {
            printf("Received backslash r line\n");
        } else {
            printf("%c\n", hello);
        }
    }

    for (i = 0; i < 20; i++) {
        printf("1\n");
    }

    i = 0;
    while (checkthewifi[i] != '\0') {
        putcharRS232(checkthewifi[i]);
        i++;
    }

    char thirdlast = 'h';
    char secondlast = 'i';
    
    while (1) {
        thirdlast = secondlast;
        secondlast = hello;                
        hello = getcharRS232();
        if (hello == '\n') {
            printf("Received new line\n");
        } else if (hello == '\r') {
            printf("Received backslash r line\n");
        } else {
            printf("%c\n", hello);
        }

        if (hello == 'd' && secondlast == 'e' && thirdlast == 'u') {
            break;
        }

        
    }
    printf("All done %d\n", RS232TestForReceivedData());
    RS232Flush();
    return 0;
}

void Init_RS232(void)
{
    // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    set_bit(RS232_LineControlReg, 7);
    // set Divisor latch (LSB and MSB) with correct value for required baud rate
    int divisor_value = BR_CLK / (DESIRED_BAUD_RATE * 16);
    *RS232_DivisorLatchLSB = (divisor_value & 0xFF);
    *RS232_DivisorLatchMSB = (divisor_value & 0xFF00) >> 8;
    // set bit 7 of Line control register back to 0 and
    clear_bit(RS232_LineControlReg, 7);
    // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *RS232_LineControlReg |= 0x3; // 8 bit data
    clear_bit(RS232_LineControlReg, 2); // 1 stop bit
    clear_bit(RS232_LineControlReg, 3); // no parity
    // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    *RS232_FifoControlReg |= 0x6;
    // Now Clear all bits in the FiFo control registers
    *RS232_FifoControlReg = 0;
}

int putcharRS232(int c)
{
    // wait for Transmitter Holding Register bit (5) of line status register to be '1'
    // indicating we can write to the device
    // write character to Transmitter fiforegister
    // return the character we printed
    while (!(*RS232_LineStatusReg & (1 << 5)));
    *RS232_TransmitterFifo = c;
    return c;
}

int getcharRS232(void)
{
    // wait for Data Ready bit (0) of line status register to be '1'
    // read new character from ReceiverFiFo register
    // return new character
    while (!RS232TestForReceivedData());
    return *RS232_ReceiverFifo;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one,or read it, it simply tests
// to see if one is available to read from the FIFO
int RS232TestForReceivedData(void)
{
    // if RS232_LineStatusReg bit 0 is set to 1
    // return TRUE, otherwise return FALSE
    return (*RS232_LineStatusReg) & 1;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//
void RS232Flush(void)
{
    unsigned char unwanted_char;
    // read while bit 0 of Line Status Register == ‘1’
    while (RS232TestForReceivedData()) {
        unwanted_char = *RS232_ReceiverFifo;
    }
}