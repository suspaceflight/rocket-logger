#ifndef __CONFIG_H__
#define __CONFIG_H__
	

#define DEBUG



#define  RFPOWER_H    (0xC0)
#define  RFPOWER_L    (0x80)
#define  RFCHANNEL  8

#define CONTINLIM 9000000

//timer values
#define TIMER_VAL_300 106
#define TIMER_VAL_50 637  //636
#define TIMER_VAL_WAIT 400  /*1500*/
#define TIMER_VAL_REG TA1CCR0

//UART values
//for clock = 32768x32 ~ 1MHz
#define defUCBR0l	6
#define defUCBR1l	0
#define defUCBRSl	0
#define defUCBRFl	13
//for clock = 8.125MHz
#define defUCBR0h	54
#define defUCBR1h	0
#define defUCBRSh	0
#define defUCBRFh	1

#define defUCOS16	1
/*clock source: 01: ACLK, 10: SMCLK */
#define defUCCSEL	0x2		

//IO

#define UARTTX_PIN (1<<6)
#define UARTRX_PIN (1<<5)
#define UART_SEL P1SEL

#define PWM_PIN (1<<4)
#define PWM_SEL P3SEL
#define PWM_DIR P3DIR
#define PWMVAL TA0CCR3

#define SDCS_PIN BIT0
#define SDCS_DIR P1DIR
#define SDCS_PORT P1OUT



#endif
