#include "cc430x513x.h"
#include "config.h"
#include "radio.h"
#include "radio/RF1A.h"
#include "radio/hal_pmm.h"
#include "I2Croutines.h"
#include <stdio.h>

void accurate_clk(void);
void init_i2c(void);

#define BITHIGH WriteSingleReg(FREQ0,    0x3E)
#define BITLOW  WriteSingleReg(FREQ0,    0x3C)

volatile int prev_bit = 0;
char sendBuff[100] = "\0";
volatile uint8_t bitpos = 0;
volatile char* sendPtr = &sendBuff[0];
char outvar;

/*
 * main.c
 */
void main(void) {
	
	WDTCTL = WDTPW + WDTHOLD;
	WDTCTL = WDTPW + (0x2 << 5) + 3;
	radio_init();
	accurate_clk();

	//setup counting timer
	TA1CCR0 = 636;//636; //106; //14	//temporary top
	TA1CTL = (0x1 << 8) | (0x3 << 6); //ACLK, upto CCR0 mode, /8 prescaler,
	TA1CCTL0 = CCIE;
	TA1R = 0;
	TA1CTL |= (0x1 << 4);

	init_i2c();

	__bis_SR_register(GIE);

	__delay_cycles(400000);

	radio_high_power();
	radio_carrier_on();

while(1)
	{

	while(P3IN & BIT4);

	unsigned char read_val[150];
		EEPROM_SequentialRead(1,&read_val[1],5);
	// read_val[0] = EEPROM_RandomRead(0x00);  // Read from address 0x0000
	/*  read_val[0] = EEPROM_CurrentAddressRead();//(0x0000);  // Read from address 0x0000
	  read_val[1] = EEPROM_CurrentAddressRead();//EEPROM_RandomRead(0x0001);
	  read_val[2] = EEPROM_CurrentAddressRead();//EEPROM_RandomRead(0x0002);
	  read_val[3] = EEPROM_CurrentAddressRead();//EEPROM_RandomRead(0x0003);
	  read_val[4] = EEPROM_CurrentAddressRead();//EEPROM_RandomRead(0x0004);
	  read_val[5] = EEPROM_CurrentAddressRead();//EEPROM_RandomRead(0x0005);*/
	  read_val[10] = EEPROM_CurrentAddressRead();//EEPROM_RandomRead(0x0C);
	  char temperature = read_val[4];
	  int16_t altitude = ((int16_t)read_val[1])<<8 | ((int16_t)read_val[2]);
	  read_val[8] = altitude;
	//  printf("hello\n");

	 // __delay_cycles(900000);

	  while(*sendPtr);
	  snprintf(sendBuff,100,"HELLO,%d,%d\n\n",altitude,read_val[3]>>4);
	  _delay_cycles(200000);
	  sendPtr = sendBuff;


	}{
		radio_low_power();
		//radio_carrier_on();
		//rtty_high();
		__delay_cycles(900000);
		//radio_idle();
		__delay_cycles(100000);
		radio_high_power();
		//radio_carrier_on();
		//rtty_low();
		__delay_cycles(900000);
		//radio_idle();
		__delay_cycles(100000);
		radio_write_power (0xC0);
		__delay_cycles(900000);
		__delay_cycles(100000);

	}

}


// Timer A0 interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{

	if(*sendPtr)
	{
		if (bitpos == 0)
		{
			//start bit
			BITLOW;
			outvar = *sendPtr;

		}
		else if (bitpos > 7)
		{
			//stop bits
			BITHIGH;
		}
		else
		{
			if (outvar & 1)
				BITHIGH;
			else
				BITLOW;
			outvar = outvar >> 1;
		}

		bitpos++;
		if (bitpos > 9)
		{
			bitpos = 0;
			sendPtr++;
		}
	}
/*
	if (prev_bit){
		prev_bit = 0;
		WriteSingleReg(FREQ0,    0x3C);
	}
	else
	{
		prev_bit = 1;
		WriteSingleReg(FREQ0,    0x3E);
	}*/

}

void init_i2c(void)
{
	//remap pins
	PMAPPWD = 0x02D52;
	P3MAP7 = PM_UCB0SDA;
	P1MAP0 = PM_UCB0SCL;
	PMAPPWD = 0;
	P3SEL |= (1<<7);
	P1SEL |= (1<<0);

	InitI2C(0x60);

	EEPROM_ByteWrite(0x26,0x80);//0xB8);   //set OSR to 128
	EEPROM_ByteWrite(0x13,0x07);   //enable data flags
	EEPROM_ByteWrite(0x28,0x11);   //set int active low open drain
	EEPROM_ByteWrite(0x29,0x80);   //enable DRDY interrupt
	EEPROM_ByteWrite(0x2A,0x80);   //DRDY interrupt to INT1
	EEPROM_ByteWrite(0x26,0xB9);   //set active

	//setup pin to get drdy
	P3REN |= BIT4;
	P3OUT |= BIT4;

}

void accurate_clk(void)
//SMCLK: 8.125MHz
//ALCK: 253kHz
{
	UCSCTL6 = (0<<8) | (1<<0); 	//RF XL on,
	//UCSCTL0 = ;  DCO and MOD modified automatically during FLL operation
	UCSCTL1 = (5<<4); 	//DCO to range 5
	UCSCTL2 = 4 | (0 << 12);	//prescaler: 1, divider: 4
	UCSCTL3 = (0x5 << 4) | 0x5;			//xt2 reference for FLL, input prescale: 16
	UCSCTL4 = SELA__DCOCLKDIV | SELS__DCOCLK | SELM__DCOCLKDIV;  	//SMCLK: DCO, ACLK/MCLK: DCODIV
	UCSCTL5 = (0x0 << 12) | (0x5 << 8) | (0x0 << 4) | 0x4;				//ACLK: /32, SMCLK: /1, MCLK: /16


}
