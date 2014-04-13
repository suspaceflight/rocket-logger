
/* ------------------------------------------------------------------------------------------------
 *                                          Defines
 * ------------------------------------------------------------------------------------------------
 */

/********************
 * Variable definition
 */
 /*
typedef struct S_RF_SETTINGS {
    unsigned char fsctrl1;   // Frequency synthesizer control.
    unsigned char fsctrl0;   // Frequency synthesizer control.
    unsigned char freq2;     // Frequency control word, high byte.
    unsigned char freq1;     // Frequency control word, middle byte.
    unsigned char freq0;     // Frequency control word, low byte.
    unsigned char mdmcfg4;   // Modem configuration.
    unsigned char mdmcfg3;   // Modem configuration.
    unsigned char mdmcfg2;   // Modem configuration.
    unsigned char mdmcfg1;   // Modem configuration.
    unsigned char mdmcfg0;   // Modem configuration.
    unsigned char channr;    // Channel number.
    unsigned char deviatn;   // Modem deviation setting (when FSK modulation is enabled).
    unsigned char frend1;    // Front end RX configuration.
    unsigned char frend0;    // Front end RX configuration.
    unsigned char mcsm0;     // Main Radio Control State Machine configuration.
    unsigned char foccfg;    // Frequency Offset Compensation Configuration.
    unsigned char bscfg;     // Bit synchronization Configuration.
    unsigned char agcctrl2;  // AGC control.
    unsigned char agcctrl1;  // AGC control.
    unsigned char agcctrl0;  // AGC control.
    unsigned char fscal3;    // Frequency synthesizer calibration.
    unsigned char fscal2;    // Frequency synthesizer calibration.
    unsigned char fscal1;    // Frequency synthesizer calibration.
    unsigned char fscal0;    // Frequency synthesizer calibration.
    unsigned char fstest;    // Frequency synthesizer calibration control
    unsigned char test2;     // Various test settings.
    unsigned char test1;     // Various test settings.
    unsigned char test0;     // Various test settings.
    unsigned char fifothr;   // RXFIFO and TXFIFO thresholds.
    unsigned char iocfg2;    // GDO2 output pin configuration
    unsigned char iocfg0;    // GDO0 output pin configuration
    unsigned char pktctrl1;  // Packet automation control.
    unsigned char pktctrl0;  // Packet automation control.
    unsigned char addr;      // Device address.
    unsigned char pktlen;    // Packet length.
} RF_SETTINGS;
*/

// RF settings for CC430

typedef struct {
    unsigned char iocfg2;        // GDO2 Output Configuration
    unsigned char iocfg1;        // GDO1 Output Configuration
    unsigned char iocfg0;        // GDO0 Output Configuration
    unsigned char fifothr;       // RX FIFO and TX FIFO Thresholds
    unsigned char sync1;         // Sync Word, High Byte
    unsigned char sync0;         // Sync Word, Low Byte
    unsigned char pktlen;        // Packet Length
    unsigned char pktctrl1;      // Packet Automation Control
    unsigned char pktctrl0;      // Packet Automation Control
    unsigned char addr;          // Device Address
    unsigned char channr;        // Channel Number
    unsigned char fsctrl1;       // Frequency Synthesizer Control
    unsigned char fsctrl0;       // Frequency Synthesizer Control
    unsigned char freq2;         // Frequency Control Word, High Byte
    unsigned char freq1;         // Frequency Control Word, Middle Byte
    unsigned char freq0;         // Frequency Control Word, Low Byte
    unsigned char mdmcfg4;       // Modem Configuration
    unsigned char mdmcfg3;       // Modem Configuration
    unsigned char mdmcfg2;       // Modem Configuration
    unsigned char mdmcfg1;       // Modem Configuration
    unsigned char mdmcfg0;       // Modem Configuration
    unsigned char deviatn;       // Modem Deviation Setting
    unsigned char mcsm2;         // Main Radio Control State Machine Configuration
    unsigned char mcsm1;         // Main Radio Control State Machine Configuration
    unsigned char mcsm0;         // Main Radio Control State Machine Configuration
    unsigned char foccfg;        // Frequency Offset Compensation Configuration
    unsigned char bscfg;         // Bit Synchronization Configuration
    unsigned char agcctrl2;      // AGC Control
    unsigned char agcctrl1;      // AGC Control
    unsigned char agcctrl0;      // AGC Control
    unsigned char worevt1;       // High Byte Event0 Timeout
    unsigned char worevt0;       // Low Byte Event0 Timeout
    unsigned char worctrl;       // Wake On Radio Control
    unsigned char frend1;        // Front End RX Configuration
    unsigned char frend0;        // Front End TX Configuration
    unsigned char fscal3;        // Frequency Synthesizer Calibration
    unsigned char fscal2;        // Frequency Synthesizer Calibration
    unsigned char fscal1;        // Frequency Synthesizer Calibration
    unsigned char fscal0;        // Frequency Synthesizer Calibration
    unsigned char fstest;        // Frequency Synthesizer Calibration Control
    unsigned char ptest;         // Production Test
    unsigned char agctest;       // AGC Test
    unsigned char test2;         // Various Test Settings
    unsigned char test1;         // Various Test Settings
    unsigned char test0;         // Various Test Settings
    unsigned char partnum;       // Chip ID
    unsigned char version;       // Chip ID
    unsigned char freqest;       // Frequency Offset Estimate From Demodulator
    unsigned char lqi;           // Demodulator Estimate for Link Quality
    unsigned char rssi;          // Received Signal Strength Indication
    unsigned char marcstate;     // Main Radio Control State Machine State
    unsigned char wortime1;      // High Byte of WOR Time
    unsigned char wortime0;      // Low Byte of WOR Time
    unsigned char pktstatus;     // Current GDOx Status and Packet Status
    unsigned char vco_vc_dac;    // Current Setting from PLL Calibration Module
    unsigned char txbytes;       // Underflow and Number of Bytes
    unsigned char rxbytes;       // Overflow and Number of Bytes
    unsigned char rf1aifctl0;    // Radio interface control register 0
    unsigned char rf1aifctl1;    // Radio interface control register 1
    unsigned char rf1aifctl2;    // Reserved
    unsigned char rf1aiferr;     // Radio interface error flag register
    unsigned char rf1aiferrv;    // Radio interface error vector word register
    unsigned char rf1aifiv;      // Radio interface interrupt vector word register
    unsigned char rf1ainstrw;    // Radio instruction word register
    unsigned char rf1ainstr1w;   // Radio instruction word register with 1-byte auto-read (low-byte ignored)
    unsigned char rf1ainstr2w;   // Radio instruction word register with 2-byte auto-read (low-byte ignored)
    unsigned char rf1adinw;      // Radio word data in register
    unsigned char rf1astat0w;    // Radio status word register without auto-read
    unsigned char rf1astat1w;    // Radio status word register with 1-byte auto-read
    unsigned char rf1astat2w;    // Radio status word register with 2-byte auto-read
    unsigned char rf1adout0w;    // Radio core word data out register without auto-read
    unsigned char rf1adout1w;    // Radio core word data out register with 1-byte auto-read
    unsigned char rf1adout2w;    // Radio core word data out register with 2-byte auto-read
    unsigned char rf1ain;        // Radio core signal input register
    unsigned char rf1aifg;       // Radio core interrupt flag register
    unsigned char rf1aies;       // Radio core interrupt edge select register
    unsigned char rf1aie;        // Radio core interrupt enable register
    unsigned char rf1aiv;        // Radio core interrupt vector word register
    unsigned char rf1arxfifo;    // Direct receive FIFO access register
    unsigned char rf1atxfifo;    // Direct transmit FIFO access register
} RF_SETTINGS;

void ResetRadioCore (void);
unsigned char Strobe(unsigned char strobe);

void WriteRfSettings(RF_SETTINGS *pRfSettings);

void WriteSingleReg(unsigned char addr, unsigned char value);
void WriteBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count);
unsigned char ReadSingleReg(unsigned char addr);
void ReadBurstReg(unsigned char addr, unsigned char *buffer, unsigned char count);
void WriteSinglePATable(unsigned char value);
void WriteBurstPATable(unsigned char *buffer, unsigned char count); 
