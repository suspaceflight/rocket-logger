
#include "RF1A.h"
#include "../config.h"

// Rf settings for CC430F5135
RF_SETTINGS rfSettings = {
    0x0B,  // IOCFG2           GDO2 Output Configuration
    0x2E,  // IOCFG1           GDO1 Output Configuration
    0x2D,  // IOCFG0           GDO0 Output Configuration
    0x47,  // FIFOTHR          RX FIFO and TX FIFO Thresholds
    0xD3,  // SYNC1            Sync Word, High Byte
    0x91,  // SYNC0            Sync Word, Low Byte
    0xFF,  // PKTLEN           Packet Length
    0x04,  // PKTCTRL1         Packet Automation Control
    0x12,  // PKTCTRL0         Packet Automation Control
    0x00,  // ADDR             Device Address
    RFCHANNEL,  // CHANNR           Channel Number
    0x06,  // FSCTRL1          Frequency Synthesizer Control
    0x00,  // FSCTRL0          Frequency Synthesizer Control
    0x10,  // FREQ2            Frequency Control Word, High Byte
    0xB1,  // FREQ1            Frequency Control Word, Middle Byte
    0x3B,  // FREQ0            Frequency Control Word, Low Byte
    0xF4,  // MDMCFG4          Modem Configuration
    0x02,  // MDMCFG3          Modem Configuration
    0x30,  // MDMCFG2          Modem Configuration
    0x20,  // MDMCFG1          Modem Configuration
    0x00,  // MDMCFG0          Modem Configuration
    0x15,  // DEVIATN          Modem Deviation Setting
    0x07,  // MCSM2            Main Radio Control State Machine Configuration
    0x30,  // MCSM1            Main Radio Control State Machine Configuration
    0x10,  // MCSM0            Main Radio Control State Machine Configuration
    0x16,  // FOCCFG           Frequency Offset Compensation Configuration
    0x6C,  // BSCFG            Bit Synchronization Configuration
    0x03,  // AGCCTRL2         AGC Control
    0x40,  // AGCCTRL1         AGC Control
    0x91,  // AGCCTRL0         AGC Control
    0x87,  // WOREVT1          High Byte Event0 Timeout
    0x6B,  // WOREVT0          Low Byte Event0 Timeout
    0xFB,  // WORCTRL          Wake On Radio Control
    0x56,  // FREND1           Front End RX Configuration
    0x11,  // FREND0           Front End TX Configuration
    0xE9,  // FSCAL3           Frequency Synthesizer Calibration
    0x2A,  // FSCAL2           Frequency Synthesizer Calibration
    0x00,  // FSCAL1           Frequency Synthesizer Calibration
    0x1F,  // FSCAL0           Frequency Synthesizer Calibration
    0x59,  // FSTEST           Frequency Synthesizer Calibration Control
    0x7F,  // PTEST            Production Test
    0x3F,  // AGCTEST          AGC Test
    0x81,  // TEST2            Various Test Settings
    0x35,  // TEST1            Various Test Settings
    0x09,  // TEST0            Various Test Settings
    0x00,  // PARTNUM          Chip ID
    0x06,  // VERSION          Chip ID
    0x00,  // FREQEST          Frequency Offset Estimate From Demodulator
    0x7F,  // LQI              Demodulator Estimate for Link Quality
    0x80,  // RSSI             Received Signal Strength Indication
    0x00,  // MARCSTATE        Main Radio Control State Machine State
    0x00,  // WORTIME1         High Byte of WOR Time
    0x00,  // WORTIME0         Low Byte of WOR Time
    0x00,  // PKTSTATUS        Current GDOx Status and Packet Status
    0x94,  // VCO_VC_DAC       Current Setting from PLL Calibration Module
    0x00,  // TXBYTES          Underflow and Number of Bytes
    0x00,  // RXBYTES          Overflow and Number of Bytes
    0x00,  // RF1AIFCTL0       Radio interface control register 0
    0x00,  // RF1AIFCTL1       Radio interface control register 1
    0x00,  // RF1AIFCTL2       Reserved
    0x00,  // RF1AIFERR        Radio interface error flag register
    0x00,  // RF1AIFERRV       Radio interface error vector word register
    0x00,  // RF1AIFIV         Radio interface interrupt vector word register
    0x00,  // RF1AINSTRW       Radio instruction word register
    0x00,  // RF1AINSTR1W      Radio instruction word register with 1-byte auto-read (low-byte ignored)
    0x00,  // RF1AINSTR2W      Radio instruction word register with 2-byte auto-read (low-byte ignored)
    0x00,  // RF1ADINW         Radio word data in register
    0x00,  // RF1ASTAT0W       Radio status word register without auto-read
    0x00,  // RF1ASTAT1W       Radio status word register with 1-byte auto-read
    0x00,  // RF1ASTAT2W       Radio status word register with 2-byte auto-read
    0x00,  // RF1ADOUT0W       Radio core word data out register without auto-read
    0x00,  // RF1ADOUT1W       Radio core word data out register with 1-byte auto-read
    0x00,  // RF1ADOUT2W       Radio core word data out register with 2-byte auto-read
    0x00,  // RF1AIN           Radio core signal input register
    0x00,  // RF1AIFG          Radio core interrupt flag register
    0x00,  // RF1AIES          Radio core interrupt edge select register
    0x00,  // RF1AIE           Radio core interrupt enable register
    0x00,  // RF1AIV           Radio core interrupt vector word register
    0x00,  // RF1ARXFIFO       Direct receive FIFO access register
    0x00,  // RF1ATXFIFO       Direct transmit FIFO access register
};
/*

RF_SETTINGS rfSettings = {
    0x3F,  //0x3F, //0x0A,  // IOCFG2              GDO2 Output Pin Configuration
    0x2E,  // IOCFG1              GDO1 Output Pin Configuration
    0x0C,  // IOCFG0              GDO0 Output Pin Configuration
    0x47,  // FIFOTHR             RX FIFO and TX FIFO Thresholds
    0xD3,  // SYNC1               Sync Word, High Byte
    0x91,  // SYNC0               Sync Word, Low Byte
    0xFF,  // PKTLEN              Packet Length
    0x04,  // PKTCTRL1            Packet Automation Control
    0x12,  // PKTCTRL0            Packet Automation Control
    0x00,  // ADDR                Device Address
    RFCHANNEL,  // CHANNR         Channel Number
    0x06,  // FSCTRL1             Frequency Synthesizer Control
    0x00,  // FSCTRL0             Frequency Synthesizer Control
    0x10,  // FREQ2               Frequency Control Word, High Byte
    0xB2,  // FREQ1               Frequency Control Word, Middle Byte
    0x37,  // FREQ0               Frequency Control Word, Low Byte
    0xF5,  // MDMCFG4             Modem Configuration
    0x83,  // MDMCFG3             Modem Configuration
    0x30,  // MDMCFG2             Modem Configuration
    0x00,  // MDMCFG1             Modem Configuration
    0xF8,  // MDMCFG0             Modem Configuration
    0x00,  // DEVIATN             Modem Deviation Setting
    0x07,  // MCSM2               Main Radio Control State Machine Configuration
    0x30,  // MCSM1               Main Radio Control State Machine Configuration
    0x18,  // MCSM0               Main Radio Control State Machine Configuration
    0x16,  // FOCCFG              Frequency Offset Compensation Configuration
    0x6C,  // BSCFG               Bit Synchronization Configuration
    0x03,  // AGCCTRL2            AGC Control
    0x40,  // AGCCTRL1            AGC Control
    0x91,  // AGCCTRL0            AGC Control
    0x87,  // WOREVT1             High Byte Event0 Timeout
    0x6B,  // WOREVT0             Low Byte Event0 Timeout
    0xFB,  // WORCTRL             Wake On Radio Control
    0x56,  // FREND1              Front End RX Configuration
    0x11,  // FREND0              Front End TX Configuration
    0xE9,  // FSCAL3              Frequency Synthesizer Calibration
    0x2A,  // FSCAL2              Frequency Synthesizer Calibration
    0x00,  // FSCAL1              Frequency Synthesizer Calibration
    0x1F,  // FSCAL0              Frequency Synthesizer Calibration
    0x41,  // RCCTRL1             RC Oscillator Configuration
    0x00,  // RCCTRL0             RC Oscillator Configuration
    0x59,  // FSTEST              Frequency Synthesizer Calibration Control
    0x7F,  // PTEST               Production Test
    0x3F,  // AGCTEST             AGC Test
    0x81,  // TEST2               Various Test Settings
    0x35,  // TEST1               Various Test Settings
    0x09,  // TEST0               Various Test Settings
    0x00,  // PARTNUM             Chip ID
    0x04,  // VERSION             Chip ID
    0x00,  // FREQEST             Frequency Offset Estimate from Demodulator
    0x00,  // LQI                 Demodulator Estimate for Link Quality
    0x00,  // RSSI                Received Signal Strength Indication
    0x00,  // MARCSTATE           Main Radio Control State Machine State
    0x00,  // WORTIME1            High Byte of WOR Time
    0x00,  // WORTIME0            Low Byte of WOR Time
    0x00,  // PKTSTATUS           Current GDOx Status and Packet Status
    0x00,  // VCO_VC_DAC          Current Setting from PLL Calibration Module
    0x00,  // TXBYTES             Underflow and Number of Bytes
    0x00,  // RXBYTES             Overflow and Number of Bytes
    0x00,  // RCCTRL1_STATUS      Last RC Oscillator Calibration Result
    0x00,  // RCCTRL0_STATUS      Last RC Oscillator Calibration Result
};*/
/*
RF_SETTINGS rfSettings = {
    0x06,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x23,   // FREQ2     Frequency control word, high byte.
    0x31,   // FREQ1     Frequency control word, middle byte.
    0x3B,   // FREQ0     Frequency control word, low byte.
    0xCA,   // MDMCFG4   Modem configuration.
    0x83,   // MDMCFG3   Modem configuration.
    0x10,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x34,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end TX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x43,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
    0xE9,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x1F,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x47,   // FIFOTHR   RXFIFO and TXFIFO thresholds.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x2D,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x12,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xFF    // PKTLEN    Packet length.
};*/
