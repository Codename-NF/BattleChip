// definition of Memory-Mapped RS232 registers

// First X-1 bytes of the memory-mapped address range (the part that's the same for all addresses on the device)
#define Base_Address_0 0xFF21021
#define Base_Address_1 0xFF21022
// assuming 50 MHz from DE1-SoC
#define BR_CLK 50000000
// https://piazza.com/class/kji9z25zeez1qk?cid=86
// 115200 for WiFi
// For Bluetooth - 38400 for AT, 115200 for communication
#define DESIRED_BAUD_RATE 115200

// Macros to concatenate the addresses cleanly
#define CONCATENATE_ADDRESSES_(base, offset) base ## offset
#define CONCATENATE_ADDRESSES(base, offset) CONCATENATE_ADDRESSES_(base, offset)

// Register definitions
#define RS232_ReceiverFifo_0       ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 0)))
#define RS232_TransmitterFifo_0    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 0)))
#define RS232_InterruptEnableReg_0 ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 2)))
#define RS232_InterruptIdentificationReg_0        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 4)))
#define RS232_FifoControlReg_0                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 4)))
#define RS232_LineControlReg_0                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 6)))
#define RS232_ModemControlReg_0                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 8)))
#define RS232_LineStatusReg_0                     ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, A)))
#define RS232_ModemStatusReg_0                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, C)))
#define RS232_ScratchReg_0                        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, E)))
#define RS232_DivisorLatchLSB_0                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 0)))
#define RS232_DivisorLatchMSB_0                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_0, 2)))

// Register definitions
#define RS232_ReceiverFifo_1       ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 0)))
#define RS232_TransmitterFifo_1    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 0)))
#define RS232_InterruptEnableReg_1 ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 2)))
#define RS232_InterruptIdentificationReg_1        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 4)))
#define RS232_FifoControlReg_1                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 4)))
#define RS232_LineControlReg_1                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 6)))
#define RS232_ModemControlReg_1                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 8)))
#define RS232_LineStatusReg_1                     ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, A)))
#define RS232_ModemStatusReg_1                    ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, C)))
#define RS232_ScratchReg_1                        ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, E)))
#define RS232_DivisorLatchLSB_1                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 0)))
#define RS232_DivisorLatchMSB_1                   ((volatile unsigned char *)(CONCATENATE_ADDRESSES(Base_Address_1, 2)))

void Init_RS232(void);
int putcharRS232_0(int c);
int getcharRS232_0(void);
int RS232TestForReceivedData_0(void);
void RS232Flush_0(void);

// void Init_RS232_1(void);
int putcharRS232_1(int c);
int getcharRS232_1(void);
int RS232TestForReceivedData_1(void);
void RS232Flush_1(void);

void BT_send_0(const char *word);
int BT_receive_0(char *receive_char);

void BT_send_1(const char *word);
int BT_receive_1(char *receive_char);


static inline void clear_bit(volatile unsigned char *reg, int bit) {
    *reg &= ~(1 << bit);
}

static inline void set_bit(volatile unsigned char *reg, int bit) {
    *reg |= (1 << bit);
}
