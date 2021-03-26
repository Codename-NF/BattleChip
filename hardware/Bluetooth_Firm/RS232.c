#include "RS232.h"
#include <stdio.h>

/**************************************************************************
/*  Subroutine to initialise the RS232 Port by writing some data
**  to the internal registers.
**  Call this function at the start of the program before you attempt
**  to read or write to data via the RS232 port
**
**  Refer to UART data sheet for details of registers and programming
***************************************************************************/

int main(void) {
    Init_RS232();
    RS232Flush_0();
    RS232Flush_1();
    char receive_char_0[256];
    char receive_char_1[256];
    receive_char_0[0] = 1;
    receive_char_1[1] = 1;
    while(1) {

        if(RS232TestForReceivedData_0()) {
            // BT_send_0("Ready Player 1");
            if (BT_receive_0(receive_char_0)) {
                BT_send_0("Player1:");
                BT_send_0(receive_char_0);
                RS232Flush_0();
            }
        }
        
        if(RS232TestForReceivedData_1()) {
            // BT_send_1("Ready Player 2");
            if (BT_receive_1(receive_char_1)) {
                BT_send_1("Player2:");
                BT_send_1(receive_char_1);
                RS232Flush_1();
            }       
        }
    }
    BT_send_1("Message Received");
    printf("%d\n", RS232TestForReceivedData_1());
    RS232Flush_1();
    return 0;
}

void Init_RS232(void)
{
    // set bit 7 of Line Control Register to 1, to gain access to the baud rate registers
    set_bit(RS232_LineControlReg_0, 7);
    set_bit(RS232_LineControlReg_1, 7);
    // set Divisor latch (LSB and MSB) with correct value for required baud rate
    int divisor_value = BR_CLK / (DESIRED_BAUD_RATE * 16);
    *RS232_DivisorLatchLSB_0 = (divisor_value & 0xFF);
    *RS232_DivisorLatchMSB_0 = (divisor_value & 0xFF00) >> 8;
    *RS232_DivisorLatchLSB_1 = (divisor_value & 0xFF);
    *RS232_DivisorLatchMSB_1 = (divisor_value & 0xFF00) >> 8;
    // set bit 7 of Line control register back to 0 and
    clear_bit(RS232_LineControlReg_0, 7);
    clear_bit(RS232_LineControlReg_1, 7);
    // program other bits in that reg for 8 bit data, 1 stop bit, no parity etc
    *RS232_LineControlReg_0 |= 0x3; // 8 bit data
    clear_bit(RS232_LineControlReg_0, 2); // 1 stop bit
    clear_bit(RS232_LineControlReg_0, 3); // no parity
    *RS232_LineControlReg_1 |= 0x3; // 8 bit data
    clear_bit(RS232_LineControlReg_1, 2); // 1 stop bit
    clear_bit(RS232_LineControlReg_1, 3); // no parity
    // Reset the Fifo’s in the FiFo Control Reg by setting bits 1 & 2
    *RS232_FifoControlReg_0 |= 0x6;
    *RS232_FifoControlReg_1 |= 0x6;
    // Now Clear all bits in the FiFo control registers
    *RS232_FifoControlReg_0 = 0;
    *RS232_FifoControlReg_1 = 0;
}

int putcharRS232_0(int c)
{
    // wait for Transmitter Holding Register bit (5) of line status register to be '1'
    // indicating we can write to the device
    // write character to Transmitter fiforegister
    // return the character we printed
    while (!(*RS232_LineStatusReg_0 & (1 << 5)));
    *RS232_TransmitterFifo_0 = c;
    return c;
}
int putcharRS232_1(int c)
{
    while (!(*RS232_LineStatusReg_1 & (1 << 5)));
    *RS232_TransmitterFifo_1 = c;
    return c;
}

int getcharRS232_0(void)
{
    // wait for Data Ready bit (0) of line status register to be '1'
    // read new character from ReceiverFiFo register
    // return new character
    while (!RS232TestForReceivedData_0());
    return *RS232_ReceiverFifo_0;
}
int getcharRS232_1(void)
{
    while (!RS232TestForReceivedData_1());
    return *RS232_ReceiverFifo_1;
}

// the following function polls the UART to determine if any character
// has been received. It doesn't wait for one,or read it, it simply tests
// to see if one is available to read from the FIFO
int RS232TestForReceivedData_0(void)
{
    // if RS232_LineStatusReg bit 0 is set to 1
    // return TRUE, otherwise return FALSE
    return (*RS232_LineStatusReg_0) & 1;
}
int RS232TestForReceivedData_1(void)
{
    return (*RS232_LineStatusReg_1) & 1;
}

//
// Remove/flush the UART receiver buffer by removing any unread characters
//
void RS232Flush_0(void)
{
    unsigned char unwanted_char;
    // read while bit 0 of Line Status Register == ‘1’
    while (RS232TestForReceivedData_0()) {
        unwanted_char = *RS232_ReceiverFifo_0;
    }
}
void RS232Flush_1(void)
{
    unsigned char unwanted_char;
    while (RS232TestForReceivedData_1()) {
        unwanted_char = *RS232_ReceiverFifo_1;
    }
}

void BT_send_0(char *word)
{
    char *send_char;
    send_char = word;
    for(send_char++; *send_char != '\0'; send_char++) {
        putcharRS232_0(*send_char);
    }
}
void BT_send_1(char *word)
{
    char *send_char;
    send_char = word;
    for(send_char++; *send_char != '\0'; send_char++) {
        putcharRS232_1(*send_char);
    }
}

int BT_receive_0(char *receive_char)
{
    if(!RS232TestForReceivedData_0()) {
        return 0;
    }

    int x;
    for(x = receive_char[0];RS232TestForReceivedData_0();x++) {
        receive_char[x] = getcharRS232_0();
        putcharRS232_0(receive_char[x]);
    }

    if (receive_char[x-1] == '~') {
        receive_char[0] = 1;
        receive_char[x] = '\0';
        RS232Flush_0();
        return 1;
    }
    receive_char[0] = x;
    return 0;
}
int BT_receive_1(char *receive_char)
{
    if(!RS232TestForReceivedData_1()) {
        return 0;
    }

    int x;
    for(x = receive_char[0];RS232TestForReceivedData_1();x++) {
        receive_char[x] = getcharRS232_1();
        putcharRS232_1(receive_char[x]);
    }

    if (receive_char[x-1] == '~') {
        receive_char[0] = 1;
        receive_char[x] = '\0';
        RS232Flush_1();
        return 1;
    }
    receive_char[0] = x;
    return 0;
}
