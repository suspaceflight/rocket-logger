#include "cc430x513x.h"
#include "config.h"
#include "radio.h"
#include "radio/RF1A.h"
#include "radio/hal_pmm.h"
#include "I2Croutines.h"
#include <stdio.h>

void accurate_clk(void);
void init(void);
void send_debug(char*);
void send_packet(void);
uint16_t crc_xmodem_update (uint16_t crc, uint8_t data);

#define BITHIGH WriteSingleReg(FREQ0,    0x3E)
#define BITLOW  WriteSingleReg(FREQ0,    0x3C)

#define ODR_HIGH  cntrl_reg1_val = 0xA1; EEPROM_ByteWrite(0x26,cntrl_reg1_val)
#define ODR_MED   cntrl_reg1_val = 0xB1; EEPROM_ByteWrite(0x26,cntrl_reg1_val)
#define ODR_LOW   cntrl_reg1_val = 0xB9; EEPROM_ByteWrite(0x26,cntrl_reg1_val)


#define AVERAGE_BUF_POWER (4)
#define LAUNCH_DET_LEN (4)
#define AVERAGE_BUF_LEN (1<<AVERAGE_BUF_POWER)

							//16 is sample rate at max speed
#define THRES_NO_LAUNCH (5 *16)
#define THRES_POST_LAUNCH (20)

#define THRES_MED (2 *16)
//#define THRES_LARGE (6 *16)
#define THRES_LARGE (20)

volatile int prev_bit = 0;
uint8_t sendBuff[100] = "\0";
char debugBuff[100] = "\0";
volatile uint8_t bitpos = 0;
volatile uint8_t* sendPtr = &sendBuff[0];
char outvar;

volatile uint8_t state = 0;   //0-idle, 1-inflight, 2-postflight
uint8_t cntrl_reg1_val = 0xB8 | 1;

int32_t tx_max = 0;
int32_t tx_min = 0;
uint16_t tx_id = 0;
uint16_t batt_voltage = 0;

uint16_t ticks_since_last = 0;

uint8_t button_held = 3;
uint8_t button_held_off = 2;

uint16_t packet_count = 0;
const uint16_t device_id = 0x5E06;

/*
 * main.c
 */
void main(void) {
	
	WDTCTL = WDTPW + WDTHOLD;
//	WDTCTL = WDTPW + (0x2 << 5) + 3;

	init();

	__bis_SR_register(GIE);

	//__delay_cycles(400000);

	while(ADC12CTL1 & ADC12BUSY);
	batt_voltage = ADC12MEM0;
	ADC12CTL0 |= ADC12SC;
	__delay_cycles(400000);
	while(ADC12CTL1 & ADC12BUSY);
	batt_voltage = ADC12MEM0;
	ADC12CTL0 |= ADC12SC;
	while(ADC12CTL1 & ADC12BUSY);
	batt_voltage = ADC12MEM0;

	if (batt_voltage > 2662) // 3.9V
		P2OUT |= BIT3;
	else
		P2OUT |= BIT4;

	radio_high_power();
	//radio_carrier_on();


	send_debug("rst\n");

	//unsigned char txbuffp[] = {1,2,3,4,5,6,7,8,9,10};

//	while(1)
//	{
//		transmit_packet(txbuffp,10);
//		__delay_cycles(4000000);
//	}


	int32_t launch_det_buff[LAUNCH_DET_LEN];
	int32_t averaging_buff[AVERAGE_BUF_LEN] = {0};


	state = 2;		//allows a base average to be got

	uint8_t ldb_ptr = 0;
	uint8_t av_ptr = 0;

	uint8_t buff[10];

	int32_t average_sum = 0;

	uint8_t launch = 0;     //0 - no launch; >0 - launch
							//1 - medium increase since last value (needs another medium increase)
							//2 - large increase since last value  (needs another value close)

	uint8_t sample_rate = 0; //0 - slow
							 //1 - med
							 //2 - max

	uint16_t launch_sample_count = 0;	//how many samples has a launch been detected for
	uint16_t no_launch_sample_count = 0;	//how many samples has a no launch been detected for (used when in launch state)
	uint16_t post_launch_sample_count = 0;

	int32_t level_point = 0;
	int32_t max_point = 0x80000000;
	int32_t min_point = 0x7FFFFFFF;
	uint8_t set_level = 0;    //reset when returning to idle

	uint16_t since_last_tx = 0;

	//all altitudes in meters x16
	EEPROM_SequentialRead(1,&buff[0],3);

	//set high to get initial ground average
	sample_rate = 2;
	ODR_HIGH;

	uint8_t test;

	while(1)
	{

		//slow down
		/*
		while(P3IN & BIT4);
		EEPROM_SequentialRead(1,&buff[0],3);
		EEPROM_ByteWrite(0x26,cntrl_reg1_val);
		EEPROM_ByteWrite(0x26,cntrl_reg1_val | 2);
		while(P3IN & BIT4);
		EEPROM_SequentialRead(1,&buff[0],3);
		EEPROM_ByteWrite(0x26,cntrl_reg1_val);
		EEPROM_ByteWrite(0x26,cntrl_reg1_val | 2);
		*/
		//wait for new sample
		while(P3IN & BIT4);

		//get new value
		EEPROM_SequentialRead(1,&buff[0],3);
		if(sample_rate)
		{
			EEPROM_ByteWrite(0x26,cntrl_reg1_val);		//get a new value ASAP
			EEPROM_ByteWrite(0x26,cntrl_reg1_val | 2);
		}
		int32_t alt = 0;
		if (buff[0] & 0x80)
			alt = 0xFFF00000;	//sign extend

		alt |= ((int32_t)buff[0] << 12);
		alt |= ((int32_t)buff[1] << 4);
		alt |= ((int32_t)buff[2] >> 4);

		///////

#ifdef DEBUG
		if (state == 0)
		{
			test++;
			if (test == 60)
			{
				test = 0;
				alt = alt + 300;
			}
		}
#endif

		//////

		///////////////////////////////
		///    launch detect        ///
		///////////////////////////////

		//  current = alt
		//  last = launch_det_buff[ldb_ptr]
		//  prev = launch_det_buff[ptr_last]

		uint8_t ptr_last;
		if (ldb_ptr == 0)
			ptr_last = LAUNCH_DET_LEN-1;
		else
			ptr_last = ldb_ptr-1;

		int32_t diff1,diff2;
		diff1 = (alt-launch_det_buff[ldb_ptr]);
		diff2 = (launch_det_buff[ldb_ptr] - launch_det_buff[ptr_last]);

		uint8_t sign = 0;
		if ((diff1 & 0x80000000)^(diff2 & 0x80000000))  //see if same sign on diffs
			sign = 1;

		diff1 = abs(diff1);
		diff2 = abs(diff2);

		if (state < 2)
		{
			if (diff1 > THRES_LARGE)    //high rate launch happening
			{
				launch = 2;
				launch_sample_count++;
				sample_rate = 2;
				ODR_HIGH;
//				send_debug("HR ");
			}
			else if (sign)	//otheriwse, if different signs, no launch
			{
				launch = 0;
//				send_debug("DS ");
				if (sample_rate == 1)		//go back to idle, no launch
				{
					sample_rate = 0;
					ODR_LOW;
				}

			}
			else
			{
				if (diff1 > THRES_MED)
				{
					if (diff2 > THRES_MED)    //launch happening
					{
						launch = 1;
						launch_sample_count++;
						sample_rate = 2;
						ODR_HIGH;
//						send_debug("MR ");
					}
					else		//launch might be happening, but wait until next sample
					{
						sample_rate = 1;
						ODR_MED;
//						send_debug("QR ");
					}
				}
				else
				{
					if (sample_rate == 1)      //go back to idle, no launch
					{
						sample_rate = 0;
						ODR_LOW;
					}
//					send_debug("   ");
				}
			}
		}
		//TODO: compare current against average baselevel


		//if just launched, save average as base level
		if (launch > 0 && set_level == 0)
		{
			set_level = 1;
			level_point = average_sum;
		}

		///////////////////////////////
		///  calculate base level   ///
		///////////////////////////////

		//place onto launch buffer
		int32_t removed;
		ldb_ptr++;
		if (ldb_ptr == LAUNCH_DET_LEN)
			ldb_ptr = 0;
		removed = launch_det_buff[ldb_ptr];
		launch_det_buff[ldb_ptr] = alt;


		//place onto averaging buffer
		int32_t removed2 = averaging_buff[av_ptr];
		averaging_buff[av_ptr] = removed;
		av_ptr++;
		if (av_ptr == AVERAGE_BUF_LEN)
			av_ptr = 0;

		//update average (not divided by number of elements of the array yet)
		average_sum -= removed2;
		average_sum += removed;

		int32_t diff = alt-level_point;
		snprintf(debugBuff,100,"%d  %d \n\r",(int16_t)alt,(int16_t)diff);
		send_debug(debugBuff);
	//	WDTCTL = (WDTCTL & 0x00FF) | WDTPW | (0x1 << 3);

		switch (state)
		{
			case 0:  		//idle state

				//test battery voltage
				//read previous
				batt_voltage = ADC12MEM0;

				if (batt_voltage > 2662) // 3.9V
					P2OUT |= BIT3;
				else
					P2OUT |= BIT4;

				//monitor power button
				if ((P1IN & (1<<1)))
				{

					if (button_held == 0){
						if(button_held_off == 0){
							P1OUT &= ~BIT2;			 //turn off
							P2OUT &= ~(BIT3 | BIT4);
						}
						else
							button_held_off--;
					}
				}
				else if (button_held)
					button_held--;
				else
					button_held_off = 2;

				if (launch > 0)
				{
					tx_id++;
					state = 1;
					send_debug("launch det\n\r");
					level_point = average_sum >> AVERAGE_BUF_POWER;
					since_last_tx = 0;
				}

				//sync pulse tx / showalive
				since_last_tx++;
				if (since_last_tx > 10)
				{
					since_last_tx = 0;
					send_packet();

				}

				ticks_since_last++;
				if (ticks_since_last > 60)//1800)   //change
					P1OUT &= ~BIT2;					//turn off

				//test battery voltage
				//start next
				ADC12CTL0 |= ADC12SC;



				_delay_cycles(30000);
				P2OUT &= ~(BIT3 | BIT4);


				break;
			case 1:			//launch state
				if (launch == 0 )
					no_launch_sample_count++;

				if (no_launch_sample_count > THRES_NO_LAUNCH)   //if launch has been 0 for a while move to post launch state
				{
					state = 2;
					no_launch_sample_count = 0;
					launch_sample_count = 0;
					send_debug("launch ended\r\n");
				}

			//	send_packet();


				break;
			default:		//post launch state
				post_launch_sample_count++;
				if (post_launch_sample_count > THRES_POST_LAUNCH)    //stay in post launch for a while to get a new base average
				{
					state = 0;
					post_launch_sample_count = 0;
					set_level = 0;
					max_point = 0x80000000;
					min_point = 0x7FFFFFFF;
					send_debug("entering idle\r\n");
					sample_rate = 0;
					ODR_LOW;

					if (tx_max > 1600 || tx_min < -1600)    ///change
						ticks_since_last = 0;

				}

				send_packet();

				break;
		}


		//turn off?
		if (batt_voltage < 2388){     //3.5V
			P1OUT &= ~BIT2;			 //turn off
		}






		//if state==launch do min/max
		if (state == 1)
		{
			if (alt > max_point)
				max_point = alt;
			if (alt < min_point)
				min_point = alt;
			tx_max = max_point - level_point;
			tx_min = min_point - level_point;
		}

	}



}

void form_packet(uint8_t *buff)
{
	buff[0] = (uint8_t)(packet_count>>8);
	buff[1] = (uint8_t)(packet_count & 0xFF);
	buff[2] = (uint8_t)(device_id >> 8);
	buff[3] = (uint8_t)(device_id & 0xFF);

	int32_t t = tx_max;
	buff[7] = (uint8_t)(t & 0xFF);
	t = t >> 8;
	buff[6] = (uint8_t)(t & 0xFF);
	t = t >> 8;
	buff[5] = (uint8_t)(t & 0xFF);
	t = t >> 8;
	buff[4] = (uint8_t)(t & 0xFF);

	t = tx_min;
	buff[11] = (uint8_t)(t & 0xFF);
	t = t >> 8;
	buff[10] = (uint8_t)(t & 0xFF);
	t = t >> 8;
	buff[9]  = (uint8_t)(t & 0xFF);
	t = t >> 8;
	buff[8]  = (uint8_t)(t & 0xFF);

	buff[16] = state;

	buff[17] = (uint8_t)((batt_voltage >> 4)&0xFF);

	buff[18] = (uint8_t)(tx_id>>8);
	buff[19] = (uint8_t)(tx_id & 0xFF);

	buff[20] = (uint8_t)((ticks_since_last>>8)&0xFF);
	buff[21] = (uint8_t)(ticks_since_last&0xFF);




	uint16_t crc = 0xFFFF;
	uint8_t i;
	for (i = 0; i < 26; i++)
		crc = crc_xmodem_update(crc,buff[i]);

	buff[26] = (uint8_t)((crc>>8) & 0xFF);
	buff[27] = (uint8_t)(crc & 0xFF);

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

void send_packet(void)
{
//	if (!(*sendPtr))
//	{
//		snprintf(sendBuff,30,"XXX$$%u,%ld,%ld\n",(int16_t)tx_id,tx_max,tx_min);
//		TA1CCTL0 = CCIE;
//		sendPtr = sendBuff;
//	}
	unsigned char status = Strobe( RF_SNOP );
	if ((status & 0x30) == 0)
	{
		form_packet(sendBuff);
		transmit_packet(sendBuff,28);
		packet_count++;
	}

}

void send_debug(char* string)
{
	while (*string)
	{
		while(!(UCA0IFG & UCTXIFG));
		UCA0TXBUF = *string;
		string++;
	}
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_CCRx_Isr(void)
{
  switch(__even_in_range(TA1IV,6))
  {
    case 0: break;                          // No interrupts
    case 2: break;                          // TA0CCR1
    case 4:                                 // TA0CCR2
    {
      // the simulated GPIO interrupt occur - do something


      break;
    }
    case 6: break;                          // TA0CCR3
    default: break;
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
	else
		TA1CCTL0 &= ~CCIE;
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

void init(void)
{
	//radio_init();
	radio_init_packet();
	accurate_clk();

	//setup counting timer
	TA1CCR0 = 636;//636; //106; //14	//temporary top
	TA1CTL = (0x1 << 8) | (0x3 << 6); //ACLK, upto CCR0 mode, /8 prescaler,
//	TA1CCTL0 = CCIE;
	TA1R = 0;
	TA1CTL |= (0x1 << 4);


	//remap pins
	PMAPPWD = 0x02D52;
	P3MAP7 = PM_UCB0SDA;
	P1MAP0 = PM_UCB0SCL;
	P3MAP4 = PM_TA1CCR2A;  //bodge p3.4 to timer0 for interrupts

	P2MAP0 = PM_UCA0TXD;

	PMAPPWD = 0;
	P3SEL |= (1<<7) | (1<<4); //i2c and pin bodge
	P1SEL |= (1<<0);// | (1<<7);

	//temp pullups
	P3REN |= BIT7;
	P3OUT |= BIT7;

	InitI2C(0x60);

	EEPROM_ByteWrite(0x26,0xB8);   //set OSR to 128
	EEPROM_ByteWrite(0x13,0x07);   //enable data flags (pressure only)
	EEPROM_ByteWrite(0x28,0x11);   //set int active low open drain
	EEPROM_ByteWrite(0x29,0x80);   //enable DRDY interrupt
	EEPROM_ByteWrite(0x2A,0x80);   //DRDY interrupt to INT1
	EEPROM_ByteWrite(0x26,cntrl_reg1_val);   //set active

	//setup pin to get drdy
	P3REN |= BIT4;
	P3OUT |= BIT4;

	//ADC
	ADC12CTL0 = ADC12SHT11 + ADC12ON;         // Sampling time, ADC12 on
	ADC12CTL1 = ADC12SHP;                     // Use sampling timer
	ADC12MCTL0 = ADC12INCH_2+ADC12EOS;
	ADC12CTL0 |= ADC12ENC;
	ADC12CTL0 |= ADC12SC;


	//bodge p3.4 to timer0 for interrupts
	TA1CCTL2 = CM_2 + CCIS_0 + SCS + CAP + CCIE;

	//keepon
	P1DIR |= BIT2;
	P1OUT |= BIT2;

	//button sense pull down
	P1REN |= (1<<1);

	//status LEDs
	P2DIR |= BIT3 | BIT4;
	P2OUT &= ~(BIT3 | BIT4);


	//setup debug uart
	P2SEL |= BIT0;
	P2DIR |= BIT0;
	UCA0CTL1 |= UCSSEL_1;
	UCA0BR0 = 27;
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS1;
	UCA0CTL1 &= ~UCSWRST;




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


//int32_t abs(int32_t in1)
//{
//	if (in1 & 0x80000000)
//		return -in1;
//	else
//		return in1;
//}
