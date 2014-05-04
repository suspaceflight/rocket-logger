#include <avr/io.h>
#include "config.h"
#include "radio.h"
#include "RF1A.h"
#include <util/delay.h>


#define BITCOUNTMAX 9
 
extern RF_SETTINGS rfSettings;
extern RF_SETTINGS rfSettings_packet;

char bitCounter;
uint8_t currentChar;
char* strptr;


void radio_init(void)
{
	init_spi_master();
	uint8_t pwrs[2] = {RFPOWER_H, RFPOWER_L};
	WriteRfSettings(&rfSettings);	
	WriteBurstPATable(&pwrs[0],2);
	bitCounter = 0;
	strptr = &bitCounter;
}

void radio_init_packet(void)
{
	init_spi_master();
	uint8_t pwrs[2] = {RFPOWER_H, RFPOWER_L};
	WriteRfSettings(&rfSettings_packet);
	WriteBurstPATable(&pwrs[0],2);
	bitCounter = 0;
	strptr = &bitCounter;
}

void transmit_packet(uint8_t *buffer, uint8_t length)
{

	  WriteBurstReg(RF_TXFIFOWR, buffer, length);

	  radio_carrier_on();
}

 void radio_sleep(void)
 {
 	Strobe( RF_SXOFF );
 }
 
 void radio_high_power (void)
 {
 	WriteSinglePATable(RFPOWER_H);
 	//RF_SETTINGS *pRfSettings = &rfSettings;
 	//WriteSingleReg(FREND0, ((pRfSettings->frend0) & 0xF0) + 1);
 }
 
  void radio_write_power (uint8_t level)
 {
 	WriteSinglePATable(level);
 	//RF_SETTINGS *pRfSettings = &rfSettings;
 	//WriteSingleReg(FREND0, ((pRfSettings->frend0) & 0xF0) + 1);
 }
 
 void radio_low_power (void)
 {
 	WriteSinglePATable(RFPOWER_L);
 	//RF_SETTINGS *pRfSettings = &rfSettings;
 	//WriteSingleReg(FREND0, ((pRfSettings->frend0) & 0xF0) + 2);
 }
 
 void radio_nolock_beep(void)
 {
 	radio_carrier_on();
 	_delay_ms(400);
 	radio_idle();
 	radio_sleep();
 }
 
 void radio_idle(void)
 {
 	Strobe( RF_SIDLE );
 }
 
 void radio_cal(void)
 {
 	Strobe( RF_SFSTXON );
 }
 
 void radio_carrier_on(void)
 {
 	uint8_t st = Strobe( RF_SNOP );
 	if (!((st & 0x70) == 0x20 ))
 	{
 		Strobe( RF_SIDLE );
 		Strobe( RF_STX );
 	}
 }
 
 void rtty_start(char* str)
 {
 	strptr = str;
 	bitCounter = 0;
 }
 
 int rtty_next_bit(void)
//returns >=1 on complete
 {
 	if (*strptr)
 	{
	 	if (bitCounter == 0)  //start bit	 		
	 	{
	 		rtty_low();
	 		currentChar = *strptr;
	 	}
	 	else if (bitCounter >= 8)  //stop bit(s)	 		
	 		rtty_high();
	 	else
	 	{	 
	 		//normal bits
	 		if (currentChar & 0x1)
	 			rtty_high();
	 		else
	 			rtty_low();
	 			
	 		currentChar = (currentChar >> 1) | 0x80;
	 	}
	 	
	 	bitCounter++;
	 	
	 	if (bitCounter > BITCOUNTMAX)
	 	{
	 		bitCounter = 0;
	 		strptr++;
	 	}
 	
	 	if(*strptr)
	 		return 0;
	 	else
	 		return 1;
 	}
 	else
 	{
 		return 1;	
 	}
 }
 
 int rtty_done(void)
//returns >=1 for complete
 {
 	if (*strptr)
 		return 0;
 	else
 		return 1;	
 }
 
 
 void radio_temp_comp(int temp)
 {
 	
 }
