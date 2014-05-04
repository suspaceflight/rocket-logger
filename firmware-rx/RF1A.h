#include <stdint.h>
/* ------------------------------------------------------------------------------------------------
 *                                          Defines
 * ------------------------------------------------------------------------------------------------
 */


 
/********************
 * Variable definition
 */
 
// RF settings for CC430/cc1101

typedef struct {
    uint8_t iocfg2;        // GDO2 Output Configuration
    uint8_t iocfg1;        // GDO1 Output Configuration
    uint8_t iocfg0;        // GDO0 Output Configuration
    uint8_t fifothr;       // RX FIFO and TX FIFO Thresholds
    uint8_t sync1;         // Sync Word, High Byte
    uint8_t sync0;         // Sync Word, Low Byte
    uint8_t pktlen;        // Packet Length
    uint8_t pktctrl1;      // Packet Automation Control
    uint8_t pktctrl0;      // Packet Automation Control
    uint8_t addr;          // Device Address
    uint8_t channr;        // Channel Number
    uint8_t fsctrl1;       // Frequency Synthesizer Control
    uint8_t fsctrl0;       // Frequency Synthesizer Control
    uint8_t freq2;         // Frequency Control Word, High Byte
    uint8_t freq1;         // Frequency Control Word, Middle Byte
    uint8_t freq0;         // Frequency Control Word, Low Byte
    uint8_t mdmcfg4;       // Modem Configuration
    uint8_t mdmcfg3;       // Modem Configuration
    uint8_t mdmcfg2;       // Modem Configuration
    uint8_t mdmcfg1;       // Modem Configuration
    uint8_t mdmcfg0;       // Modem Configuration
    uint8_t deviatn;       // Modem Deviation Setting
    uint8_t mcsm2;         // Main Radio Control State Machine Configuration
    uint8_t mcsm1;         // Main Radio Control State Machine Configuration
    uint8_t mcsm0;         // Main Radio Control State Machine Configuration
    uint8_t foccfg;        // Frequency Offset Compensation Configuration
    uint8_t bscfg;         // Bit Synchronization Configuration
    uint8_t agcctrl2;      // AGC Control
    uint8_t agcctrl1;      // AGC Control
    uint8_t agcctrl0;      // AGC Control
    uint8_t worevt1;       // High Byte Event0 Timeout
    uint8_t worevt0;       // Low Byte Event0 Timeout
    uint8_t worctrl;       // Wake On Radio Control
    uint8_t frend1;        // Front End RX Configuration
    uint8_t frend0;        // Front End TX Configuration
    uint8_t fscal3;        // Frequency Synthesizer Calibration
    uint8_t fscal2;        // Frequency Synthesizer Calibration
    uint8_t fscal1;        // Frequency Synthesizer Calibration
    uint8_t fscal0;        // Frequency Synthesizer Calibration
    uint8_t fstest;        // Frequency Synthesizer Calibration Control
    uint8_t ptest;         // Production Test
    uint8_t agctest;       // AGC Test
    uint8_t test2;         // Various Test Settings
    uint8_t test1;         // Various Test Settings
    uint8_t test0;         // Various Test Settings
    uint8_t partnum;       // Chip ID
    uint8_t version;       // Chip ID
    uint8_t freqest;       // Frequency Offset Estimate From Demodulator
    uint8_t lqi;           // Demodulator Estimate for Link Quality
    uint8_t rssi;          // Received Signal Strength Indication
    uint8_t marcstate;     // Main Radio Control State Machine State
    uint8_t wortime1;      // High Byte of WOR Time
    uint8_t wortime0;      // Low Byte of WOR Time
    uint8_t pktstatus;     // Current GDOx Status and Packet Status
    uint8_t vco_vc_dac;    // Current Setting from PLL Calibration Module
    uint8_t txbytes;       // Underflow and Number of Bytes
    uint8_t rxbytes;       // Overflow and Number of Bytes
    uint8_t rf1aifctl0;    // Radio interface control register 0
    uint8_t rf1aifctl1;    // Radio interface control register 1
    uint8_t rf1aifctl2;    // Reserved
    uint8_t rf1aiferr;     // Radio interface error flag register
    uint8_t rf1aiferrv;    // Radio interface error vector word register
    uint8_t rf1aifiv;      // Radio interface interrupt vector word register
    uint8_t rf1ainstrw;    // Radio instruction word register
    uint8_t rf1ainstr1w;   // Radio instruction word register with 1-byte auto-read (low-byte ignored)
    uint8_t rf1ainstr2w;   // Radio instruction word register with 2-byte auto-read (low-byte ignored)
    uint8_t rf1adinw;      // Radio word data in register
    uint8_t rf1astat0w;    // Radio status word register without auto-read
    uint8_t rf1astat1w;    // Radio status word register with 1-byte auto-read
    uint8_t rf1astat2w;    // Radio status word register with 2-byte auto-read
    uint8_t rf1adout0w;    // Radio core word data out register without auto-read
    uint8_t rf1adout1w;    // Radio core word data out register with 1-byte auto-read
    uint8_t rf1adout2w;    // Radio core word data out register with 2-byte auto-read
    uint8_t rf1ain;        // Radio core signal input register
    uint8_t rf1aifg;       // Radio core interrupt flag register
    uint8_t rf1aies;       // Radio core interrupt edge select register
    uint8_t rf1aie;        // Radio core interrupt enable register
    uint8_t rf1aiv;        // Radio core interrupt vector word register
    uint8_t rf1arxfifo;    // Direct receive FIFO access register
    uint8_t rf1atxfifo;    // Direct transmit FIFO access register
} RF_SETTINGS;



void ResetRadioCore (void);
uint8_t Strobe(uint8_t strobe);

void WriteRfSettings(RF_SETTINGS *pRfSettings);

void WriteSingleReg(uint8_t addr, uint8_t value);
void WriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count);
uint8_t ReadSingleReg(uint8_t addr);
void ReadBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count);
void WriteSinglePATable(uint8_t value);
void WriteBurstPATable(uint8_t *buffer, uint8_t count); 
void init_spi_master(void);
void spi_tx(uint8_t b);
uint8_t spi_tx_rx(uint8_t b);
uint8_t spi_rx(void);


/* Radio Core Instructions */
/* command strobes               */
#define RF_SRES                0x30           /*  SRES    - Reset chip. */
#define RF_SFSTXON             0x31           /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define RF_SXOFF               0x32           /*  SXOFF   - Turn off crystal oscillator. */
#define RF_SCAL                0x33           /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define RF_SRX                 0x34           /*  SRX     - Enable RX. Perform calibration if enabled. */
#define RF_STX                 0x35           /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define RF_SIDLE               0x36           /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
//#define RF_SRSVD            0x37      /*  SRVSD   - Reserved.  Do not use. */
#define RF_SWOR                0x38           /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define RF_SPWD                0x39           /*  SPWD    - Enter power down mode when CSn goes high. */
#define RF_SFRX                0x3A           /*  SFRX    - Flush the RX FIFO buffer. */
#define RF_SFTX                0x3B           /*  SFTX    - Flush the TX FIFO buffer. */
#define RF_SWORRST             0x3C           /*  SWORRST - Reset real time clock. */
#define RF_SNOP                0x3D           /*  SNOP    - No operation. Returns status byte. */

#define RF_RXSTAT              0x80           /* Used in combination with strobe commands delivers number of availabe bytes in RX FIFO with return status */
#define RF_TXSTAT              0x00           /* Used in combination with strobe commands delivers number of availabe bytes in TX FIFO with return status */

/* other radio instr */
#define RF_SNGLREGRD           0x80
#define RF_SNGLREGWR           0x00
#define RF_REGRD               0xC0
#define RF_REGWR               0x40
#define RF_STATREGRD           0xC0           /* Read single radio core status register */
#define RF_SNGLPATABRD         (RF_SNGLREGRD+PATABLE)
#define RF_SNGLPATABWR         (RF_SNGLREGWR+PATABLE)
#define RF_PATABRD             (RF_REGRD+PATABLE)
#define RF_PATABWR             (RF_REGWR+PATABLE)
#define RF_SNGLRXRD            (RF_SNGLREGRD+RXFIFO)
#define RF_SNGLTXWR            (RF_SNGLREGWR+TXFIFO)
#define RF_RXFIFORD            (RF_REGRD+RXFIFO)
#define RF_TXFIFOWR            (RF_REGWR+TXFIFO)

// Radio Core Registers
#define IOCFG2                 0x00           /*  IOCFG2   - GDO2 output pin configuration  */
#define IOCFG1                 0x01           /*  IOCFG1   - GDO1 output pin configuration  */
#define IOCFG0                 0x02           /*  IOCFG1   - GDO0 output pin configuration  */
#define FIFOTHR                0x03           /*  FIFOTHR  - RX FIFO and TX FIFO thresholds */
#define SYNC1                  0x04           /*  SYNC1    - Sync word, high byte */
#define SYNC0                  0x05           /*  SYNC0    - Sync word, low byte */
#define PKTLEN                 0x06           /*  PKTLEN   - Packet length */
#define PKTCTRL1               0x07           /*  PKTCTRL1 - Packet automation control */
#define PKTCTRL0               0x08           /*  PKTCTRL0 - Packet automation control */
#define ADDR                   0x09           /*  ADDR     - Device address */
#define CHANNR                 0x0A           /*  CHANNR   - Channel number */
#define FSCTRL1                0x0B           /*  FSCTRL1  - Frequency synthesizer control */
#define FSCTRL0                0x0C           /*  FSCTRL0  - Frequency synthesizer control */
#define FREQ2                  0x0D           /*  FREQ2    - Frequency control word, high byte */
#define FREQ1                  0x0E           /*  FREQ1    - Frequency control word, middle byte */
#define FREQ0                  0x0F           /*  FREQ0    - Frequency control word, low byte */
#define MDMCFG4                0x10           /*  MDMCFG4  - Modem configuration */
#define MDMCFG3                0x11           /*  MDMCFG3  - Modem configuration */
#define MDMCFG2                0x12           /*  MDMCFG2  - Modem configuration */
#define MDMCFG1                0x13           /*  MDMCFG1  - Modem configuration */
#define MDMCFG0                0x14           /*  MDMCFG0  - Modem configuration */
#define DEVIATN                0x15           /*  DEVIATN  - Modem deviation setting */
#define MCSM2                  0x16           /*  MCSM2    - Main Radio Control State Machine configuration */
#define MCSM1                  0x17           /*  MCSM1    - Main Radio Control State Machine configuration */
#define MCSM0                  0x18           /*  MCSM0    - Main Radio Control State Machine configuration */
#define FOCCFG                 0x19           /*  FOCCFG   - Frequency Offset Compensation configuration */
#define BSCFG                  0x1A           /*  BSCFG    - Bit Synchronization configuration */
#define AGCCTRL2               0x1B           /*  AGCCTRL2 - AGC control */
#define AGCCTRL1               0x1C           /*  AGCCTRL1 - AGC control */
#define AGCCTRL0               0x1D           /*  AGCCTRL0 - AGC control */
#define WOREVT1                0x1E           /*  WOREVT1  - High byte Event0 timeout */
#define WOREVT0                0x1F           /*  WOREVT0  - Low byte Event0 timeout */
#define WORCTRL                0x20           /*  WORCTRL  - Wake On Radio control */
#define FREND1                 0x21           /*  FREND1   - Front end RX configuration */
#define FREND0                 0x22           /*  FREDN0   - Front end TX configuration */
#define FSCAL3                 0x23           /*  FSCAL3   - Frequency synthesizer calibration */
#define FSCAL2                 0x24           /*  FSCAL2   - Frequency synthesizer calibration */
#define FSCAL1                 0x25           /*  FSCAL1   - Frequency synthesizer calibration */
#define FSCAL0                 0x26           /*  FSCAL0   - Frequency synthesizer calibration */
//#define RCCTRL1             0x27      /*  RCCTRL1  - RC oscillator configuration */
//#define RCCTRL0             0x28      /*  RCCTRL0  - RC oscillator configuration */
#define FSTEST                 0x29           /*  FSTEST   - Frequency synthesizer calibration control */
#define PTEST                  0x2A           /*  PTEST    - Production test */
#define AGCTEST                0x2B           /*  AGCTEST  - AGC test */
#define TEST2                  0x2C           /*  TEST2    - Various test settings */
#define TEST1                  0x2D           /*  TEST1    - Various test settings */
#define TEST0                  0x2E           /*  TEST0    - Various test settings */

#define TXBYTES					0x3A
#define RXBYTES					0x3B

/* burst write registers */
#define PATABLE                0x3E           /*  PATABLE - PA control settings table */
#define TXFIFO                 0x3F           /*  TXFIFO  - Transmit FIFO */
#define RXFIFO                 0x3F           /*  RXFIFO  - Receive FIFO */