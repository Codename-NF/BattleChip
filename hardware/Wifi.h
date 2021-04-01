// definition of Memory-Mapped RS232 registers

// First X-1 bytes of the memory-mapped address range (the part that's the same for all addresses on the device)
#define Base_Address 0xFF21024
// assuming 50 MHz from DE1-SoC
#define BR_CLK 50000000
// https://piazza.com/class/kji9z25zeez1qk?cid=86
// 115200 for WiFi
// For Bluetooth - 38400 for AT, 115200 for communication
#define DESIRED_BAUD_RATE 115200

#define WIFI_ENABLE ((volatile unsigned char *)0xFF230000)
#define WIFI_RESET ((volatile unsigned char *)0xFF220000)

// Macros to concatenate the addresses cleanly
#define CONCATENATE_ADDRESSES_(base, offset) base ## offset
#define CONCATENATE_ADDRESSES(base, offset) CONCATENATE_ADDRESSES_(base, offset)

//Register definitions
#define RS232_ReceiverFifo       ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 0)))
#define RS232_TransmitterFifo    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 0)))
#define RS232_InterruptEnableReg ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 2)))
#define RS232_InterruptIdentificationReg        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 4)))
#define RS232_FifoControlReg                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 4)))
#define RS232_LineControlReg                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 6)))
#define RS232_ModemControlReg                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 8)))
#define RS232_LineStatusReg                     ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, A)))
#define RS232_ModemStatusReg                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, C)))
#define RS232_ScratchReg                        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, E)))
#define RS232_DivisorLatchLSB                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 0)))
#define RS232_DivisorLatchMSB                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address, 2)))

void Init_RS232(void);
int putcharRS232(int c);
int getcharRS232(void);
int RS232TestForReceivedData(void);
void RS232Flush(void);
int print_to_board(int p1, int p2, int win, int score1, int score2);
void printdecimal(int x);
void postgameresults(int p1, int p2, int winner, int score1, int score2);

static const char dofile[] = "dofile(\"send_match_results.lua\")\n";
static const char upload_match_results[] = "upload_match_results(~, ~, ~, ~, ~)\n";

static inline void clear_bit(volatile unsigned char *reg, int bit) {
    *reg &= ~(1 << bit);
}

static inline void set_bit(volatile unsigned char *reg, int bit) {
    *reg |= (1 << bit);
}