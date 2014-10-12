#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "radio.h"
#include "RF1A.h"
#include "lcd.h"
#include <stdio.h>
#include <stdlib.h> 

uint16_t crc_xmodem_update (uint16_t crc, uint8_t data);
void init_drawing(void);
void draw_rectangle_line(rectangle r, uint8_t thickness, uint16_t c);
void redraw_data(uint8_t index);



const uint8_t col1 = 20;
const uint8_t col2 = 120;

const uint8_t id_r = 10;
const uint8_t bv_r = 30;
const uint8_t ttl_r = 50;
const uint8_t alt_r[] = {10,40,50,60};

char buff[200] = {0};

uint16_t id_list[4] = {65535};
uint16_t fid_list[4] = {0};
uint16_t bv_list[4];

uint16_t ttl_list[4];

int32_t alt_list[4][4] = {0};   //[unit][time]
uint16_t alt_ago_list[4][4] = {0};

//uint8_t valid_list[4] = {0};

int main (void)
{
	init_lcd();
    init_drawing();

    for (uint8_t i = 0; i < 4; i++)
        id_list[i] = 65535;
    
    
    id_list[0] = 2;
    id_list[2] = 12;
    fid_list[4] = 0;
    bv_list[0] = 3840;
    bv_list[2] = 4120;
    alt_list[0][0] = 13850;
    alt_list[2][0] = -660;
    ttl_list[0] = 4*60;
    ttl_list[2] = 2*60;
    alt_ago_list[0][0] = 3;
    alt_ago_list[2][0] = 20;
    alt_ago_list[0][1] = 35;
    alt_ago_list[2][2] = 7;
    
    redraw_data(0);
    redraw_data(2);
    
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
				
				
			
            
                uint8_t index = 0;
                while(index < 4)
                {
                    if (id_list[index] == device_id)
                        break;
                    if (id_list[index] == 65535){
                        id_list[index] = device_id;
                        break;
                    }
                    index++;
                }
                
                if (index < 4)
                {      
                    bv_list[index] = (uint16_t)bv;
                    if (fid_list[index] != flight_id)
                    {
                        fid_list[index] = flight_id;
                        alt_list[index][3] = alt_list[index][2];
                        alt_list[index][2] = alt_list[index][1];
                        alt_list[index][1] = alt_list[index][0];
                    }
                    if (abs(max_alt) > abs(min_alt))                    
                        alt_list[index][0] = max_alt;
                    else
                        alt_list[index][0] = min_alt;
                    ttl_list[index] = since_last;
                    //alt_ago_list[index][0] = 3;
                    //alt_ago_list[index][1] = 35;
                
                    redraw_data(index);
                }

            
			/*
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
				*/
			}
			Strobe( RF_SRX ); 
			
		}
		while(!(PINB & (1<<1)));
		while((PINB & (1<<1)));
	}

	return 0;
}


void redraw_data(uint8_t index)
{
    uint8_t i,j;
    snprintf(buff,10,"%u",id_list[index]);
    display.x = col1;
    display.y = id_r + index*80;
    display_string("id: ");
	display_string_double(buff);
    
    snprintf(buff,10," %u mV",bv_list[index]);
    display.x = col1;
    display.y = bv_r + index*80;
    display_string_double("`");
	display_string(buff);
    
    snprintf(buff,10,"%ld",alt_list[index][0]>>4);
    display.x = col2;
    display.y = alt_r[0] + index*80;
    display_string_double(buff);
    if (alt_list[index][0] & 0b1000)
        display_string_double(".5");
    else
        display_string_double(".0");
    display.y += 8;
    display_string("m");
    
    for (i= 1; i < 4; i++)
    {
        j=snprintf(buff,17,"%ld",alt_list[index][i]>>4);
        snprintf(buff+j,17-j,"                      ");
        display.x = col2;
        display.y = alt_r[i] + index*80;
        display_string(buff);
        display.x = col2 + 8*j;
        if (alt_list[index][i] & 0b1000)
            display_string(".5");
        else
            display_string(".0");
        display_string("m");    
    }
    

    snprintf(buff,25,"ttl: %u min",ttl_list[index]/60);
    display.x = col1;
    display.y = ttl_r + index*80;
	display_string(buff);
    
    
    snprintf(buff,25,"  (rx %u min ago)",alt_ago_list[index][0]);
    display.x = col2;
    display.y = alt_r[0] + index*80 + 17;
    display_string(buff);
    
    for (i=1; i < 4; i++)
    {        
        j = snprintf(buff,25,"  (%u min)",alt_ago_list[index][i]);
        display.x = 236-(6*j);
        display.y = alt_r[i] + index*80;
        display_string(buff);
    }

}

void init_drawing(void)
{

    uint8_t i;
    rectangle r1;
    r1.left = 3;
    r1.top = 3;
    r1.bottom = 77;
    r1.right = 239;
    
    draw_rectangle_line(r1,2,BLUE);
    
    for (i=0; i< 3; i++){
        r1.top += 80;
        r1.bottom += 80;
        draw_rectangle_line(r1,2,BLUE);
    }



}

void draw_rectangle_line(rectangle r, uint8_t thickness, uint16_t c)
{
    rectangle r1;
    r1.left = r.left;
    r1.top = r.top;
    r1.bottom = r.bottom;
    r1.right = r.left+thickness;    
    fill_rectangle(r1,c);
    
    r1.right = r.right;
    r1.left = r.right - thickness;
    fill_rectangle(r1,c);
    
    r1.top = r.bottom - thickness;
    r1.left = r.left;
    fill_rectangle(r1,c);
    
    r1.top = r.top;
    r1.bottom = r.top + thickness;
    fill_rectangle(r1,c);

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


