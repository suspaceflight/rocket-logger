 #include "inttypes.h"
 void radio_init(void);
 void radio_init_packet(void);
 void radio_sleep(void);
 void radio_idle(void);
 void radio_cal(void);
 void radio_carrier_on(void);
 void radio_high_power(void);
 void radio_low_power(void);
 void radio_nolock_beep(void);
 void radio_write_power (uint8_t level);
 void transmit_packet(unsigned char *buffer, unsigned char length);
 
 void rtty_start(char* str);
 int rtty_next_bit(void);		//returns >=1 on complete
 int rtty_done(void);			//returns >=1 for complete
 
 void radio_temp_comp(int temp);
 
#define rtty_high() WriteSingleReg(FREQ0,    0x3B) /*PWMVAL = 120*/
#define rtty_low() WriteSingleReg(FREQ0,    0x3C)  /*PWMVAL = 30*/
