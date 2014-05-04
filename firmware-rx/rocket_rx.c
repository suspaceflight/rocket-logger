#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "radio.h"
#include "RF1A.h"
#include "lcd.h"
#include <stdio.h>



int main (void)
{
	init_lcd();
	display_string("hello"); 
/*
	radio_init_packet();
	radio_high_power();
	radio_carrier_on();
	_delay_ms(1000);
	//while(1);
	
	unsigned char txbuffp[] = {1,2,3,4,5,6,7,8,9,10};

	while(1)
	{
		transmit_packet(txbuffp,10);
		//spi_tx(0x53);
		_delay_ms(1000);
		
	}
	
*/
	char buff[100] = {0};

		
//	ResetRadioCore(); 
	radio_init_packet();
	Strobe( RF_SRX ); 
	

	
	while(1)
	{
		_delay_ms(1000);
		uint8_t stat = Strobe( RF_SNOP );
		uint8_t rx_count = ReadSingleReg( RXBYTES );
		snprintf(buff,100,"  s %u  rx % u",stat,rx_count);
		display_string(buff);
		
	}

	return 0;
}


