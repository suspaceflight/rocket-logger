#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "radio.h"
#include "RF1A.h"
#include "lcd.h"
#include <stdio.h>

uint16_t crc_xmodem_update (uint16_t crc, uint8_t data);


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
	char buff[200] = {0};
	uint8_t buffrx[100] = {0};
		
//	ResetRadioCore(); 
	radio_init_packet();
	Strobe( RF_SRX ); 
	
	uint16_t lastid = 0;
	

	_delay_ms(1000);
	while(1)
	{
		
		
		uint8_t stat = Strobe( RF_SNOP );
		uint8_t rx_count = ReadSingleReg( RXBYTES );
		//snprintf(buff,100,"s %u r %u",stat,rx_count);
		//display_string(buff);
		if (rx_count > 0)
		{ 
			ReadBurstReg(RF_RXFIFORD, buffrx, rx_count); 
			
			uint8_t i;
			//for (i = 26; i < rx_count; i++)
			//{				
			//	snprintf(buff,5," %x",(uint8_t)buffrx[i]);
			//	display_string(buff);
			//} 
			
			uint16_t crc = 0xFFFF;
			
			for (i = 0; i < 26; i++)
				crc = crc_xmodem_update(crc,buffrx[i]);
			//uint16_t crctop = (((uint16_t)buffrx[26]) << 8);
			//uint16_t crcbtm = (((uint16_t)buffrx[27]));
			uint16_t crcrx = (uint16_t)buffrx[27] | (((uint16_t)buffrx[26]) << 8);

			//snprintf(buff,35," %u %u %u %u %u ",crcrx,crc,crctop,crcbtm,crcbtm|crctop);
			//display_string(buff);
			
			
			
			if (crc == crcrx)
			{
			
				uint16_t packet_count = (uint16_t)buffrx[1] + (((uint16_t)buffrx[0]) << 8);
				uint16_t device_id = (uint16_t)buffrx[3] + (((uint16_t)buffrx[2]) << 8);
				int32_t max_alt =  (int32_t)buffrx[7] + (((int32_t)buffrx[6]) << 8) + (((int32_t)buffrx[5]) << 16) + (((int32_t)buffrx[4]) << 24);
				int32_t min_alt =  (int32_t)buffrx[11] + (((int32_t)buffrx[10]) << 8) + (((int32_t)buffrx[9]) << 16) + (((int32_t)buffrx[8]) << 24);
				uint16_t flight_id = (uint16_t)buffrx[19] + (((uint16_t)buffrx[18]) << 8);
                uint32_t bv = (uint16_t)buffrx[17];
                uint16_t since_last = (uint16_t)buffrx[21] + (((uint16_t)buffrx[20]) << 8);
                bv = bv * 375;
                bv = bv >> 4;
				
				
			
			
				if (lastid != flight_id)
				{
					display.x = 0;
					display.y += 8;	
				}
				else
					display.x = 0;
					
				snprintf(buff,50,"%u %u %ld %ld   %u %u      ",packet_count,flight_id,max_alt,min_alt,(uint16_t)bv,since_last);
				display_string(buff);
					
				
				lastid = flight_id;
				
			}
			Strobe( RF_SRX ); 
			
		}
		while(!(PINB & (1<<1)));
		while((PINB & (1<<1)));
	}

	return 0;
}


uint16_t crc_xmodem_update (uint16_t crc, uint8_t data)
{
	int i;

	crc = crc ^ ((uint16_t)data << 8);
	for (i=0; i<8; i++)
	{
		if (crc & 0x8000)
			crc = (crc << 1) ^ 0x1021;
		else
			crc <<= 1;
	}

	return crc;
}


