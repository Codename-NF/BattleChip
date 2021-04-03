// definition of Memory-Mapped WIFI registers
#include "RS232.h"
// First X-1 bytes of the memory-mapped address range (the part that's the same for all addresses on the device)
#define Base_Address_Wifi 0xFF21024

#define WIFI_ENABLE ((volatile unsigned char *)0xFF230000)
#define WIFI_RESET ((volatile unsigned char *)0xFF220000)

//Register definitions
#define WIFI_ReceiverFifo       ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 0)))
#define WIFI_TransmitterFifo    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 0)))
#define WIFI_InterruptEnableReg ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 2)))
#define WIFI_InterruptIdentificationReg        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 4)))
#define WIFI_FifoControlReg                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 4)))
#define WIFI_LineControlReg                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 6)))
#define WIFI_ModemControlReg                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 8)))
#define WIFI_LineStatusReg                     ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, A)))
#define WIFI_ModemStatusReg                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, C)))
#define WIFI_ScratchReg                        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, E)))
#define WIFI_DivisorLatchLSB                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 0)))
#define WIFI_DivisorLatchMSB                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_Wifi, 2)))

void Init_RS232_WIFI(void);
void Init_Wifi(void);
int putcharWIFI(int c);
int getcharWIFI(void);
int WIFITestForReceivedData(void);
void WIFIFlush(void);
void print_to_board(int p1, int p2, int win, int score1, int score2);
void printdecimal(int x);
void postgameresults(int p1, int p2, int winner, int score1, int score2);

static const char dofile[] = "dofile(\"send_match_results.lua\")\n";
static const char upload_match_results[] = "upload_match_results(~, ~, ~, ~, ~)\n";