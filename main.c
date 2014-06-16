
//CONFIGURATION BITS ======================================================

#pragma config OSC=XT, IESO=OFF //oscillator type XT,oscillator switch OFF
#pragma config PWRT=OFF, BOREN=OFF //power-up timer ON brown out OFF
#pragma config WDT=OFF // watchdog OFF
#pragma config STVREN=ON, LVP=OFF //stack overflow reset enable ON
							//low voltage programming OFF
#pragma config PBADEN=OFF // make PORTB digital I/O

//==========================================================================
//compiler directives to recognize PIC/i2c and USART and necessary headers
	#include <p18f4520.h >
	#include "delay.h"
	#include "LCD.h"
	#include "main.h"
	#include <i2c.h>
	#include <usart.h>

//DEFINING CONSTANTS
	#define RS PORTDbits.RD4
	#define RW PORTDbits.RD5
	#define EN PORTDbits.RD6
	#define POWER PORTDbits.RD7
	#define ON 1
	#define OFF 0
	
	#define MAIN_STRAIGHT_B1 14
	#define MAIN_STRAIGHT_B2 13
	#define MAIN_TURN_B1 12
	#define MAIN_TURN_B2 11
	#define SIDE_STRAIGHT_B1 10
	#define SIDE_STRAIGHT_B2 9
	#define SIDE_TURN_B1 8
	#define SIDE_TURN_B2 7
	#define MAIN_WALK_B PORTBbits.RB0
	#define SIDE_WALK_B PORTBbits.RB1
	
	#define MAIN_STRAIGHT 1
	#define MAIN_TURN 2
	#define MAIN_WALK 5
	#define SIDE_STRAIGHT  3
	#define SIDE_TURN 4
	#define SIDE_WALK 6


//	GLOBAL VARIABLES
	unsigned char hours,minutes,seconds;
	unsigned char state;
	unsigned int delay_sec;

	unsigned int led[6] = {1,2,3,4,5,6};
	
	unsigned int minTime = 10; 		//minimum time in s
	unsigned int maxTime = 30; 		//minimum time in s
	unsigned int currentTime = 0; 	//current time in s
	
	unsigned char checkPortB;		//PORTB input through encoder
	unsigned char newState;			//the next state

//=======================START FUNCTIONS===============================
void main(void)
{
	init(); 		// initialise everything
	setup_clock();	//setup clock
	
	while(1)
	{
//===========================STATE MACHINE=============================
		switch(state)
		{			
//======================DEFAULT/MAIN ROAD STRAIGHTS====================	
			case(MAIN_STRAIGHT):
			currentTime = 0;
			newState = 0;
			do 
			{
				get_time();
				display_time();
				set_LED(led[0]);
				delay_ms(1000);
				currentTime = currentTime++;
							
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;
				
				if(checkPortB==MAIN_TURN_B1 || checkPortB==MAIN_TURN_B2)
				{
					newState = 1;
				}
				else if((checkPortB==SIDE_STRAIGHT_B1 || checkPortB==SIDE_STRAIGHT_B2)&&(newState!=1))
				{
					newState = 2;
				}	
				else if((checkPortB==SIDE_TURN_B1 || checkPortB==SIDE_TURN_B2)&&(newState!=1)&&(newState!=2))
				{
					newState = 3;
				}

			} while (currentTime < minTime);	
			do
			{
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;
				
				if(checkPortB==MAIN_TURN_B1 || checkPortB==MAIN_TURN_B2)
				{
					newState = 1;
				}
				else if((checkPortB==SIDE_STRAIGHT_B1 || checkPortB==SIDE_STRAIGHT_B2)&&(newState!=1))
				{
					newState = 2;
				}	
				else if((checkPortB==SIDE_TURN_B1 || checkPortB==SIDE_TURN_B2)&&(newState!=1)&&(newState!=2))
				{
					newState = 3;
				}
					
			}while(newState==0);
	
			if(newState==1)
			{
				state = MAIN_TURN;
			}
			else if(newState==2)
			{
				state = SIDE_STRAIGHT;
			}	
			else if(newState==3)
			{
				state = SIDE_TURN;
			}
			break;
			
//==========================MAIN ROAD TURNS============================			
			case(MAIN_TURN):
			currentTime = 0;
			newState = 0;
			do 
			{
				get_time();
				display_time();
				set_LED(led[1]);
				delay_ms(1000);
				currentTime = currentTime++;
							
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;
				

				if(checkPortB==SIDE_STRAIGHT_B1 || checkPortB==SIDE_STRAIGHT_B2)
				{
					newState = 1;
				}
				else if((checkPortB==SIDE_TURN_B1 || checkPortB==SIDE_TURN_B2)&&(newState!=1))
				{
					newState = 2;
				}	
				else if(newState==0)
				{
					newState = 3;
				}			

			} while (currentTime < minTime);	
			do
			{
				if(checkPortB==SIDE_STRAIGHT_B1 || checkPortB==SIDE_STRAIGHT_B2)
				{
					newState = 1;
				}
				else if((checkPortB==SIDE_TURN_B1 || checkPortB==SIDE_TURN_B2)&&(newState!=1))
				{
					newState = 2;
				}	
				else if(newState==0)
				{
					newState = 3;
				}		
				
			}while(newState==0);
	
			if(newState==1)
			{
				state = SIDE_STRAIGHT;
			}	
			else if(newState==2)
			{
				state = SIDE_TURN;
			}
			else if(newState==3)
			{
				state = MAIN_STRAIGHT;
			}
			
			break;
			
//===========================SIDE ROAD STRAIGHTS=======================		
			case(SIDE_STRAIGHT):
			currentTime = 0;
			newState = 0;
			do 
			{
				get_time();
				display_time();
				set_LED(led[2]);
				delay_ms(1000);
				currentTime = currentTime++;
							
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;
				
				if(checkPortB==SIDE_TURN_B1 || checkPortB==SIDE_TURN_B2)
				{
					newState = 1;
				}
				else if((newState!=1))
				{
					newState = 2;
				}				

			} while (currentTime < minTime);	
			do
			{
				if(checkPortB==SIDE_TURN_B1 || checkPortB==SIDE_TURN_B2)
				{
					newState = 1;
				}
				else if((newState!=1))
				{
					newState = 2;
				}			
				
			}while(newState==0);
	
			if(newState==1)
			{
				state = SIDE_TURN;
			}
			else if(newState==2)
			{
				state = MAIN_STRAIGHT;
			}	

			break;

//===========================SIDE ROAD TURNS===========================			
			case(SIDE_TURN):
			currentTime = 0;
			newState = 0;
			do 
			{
				get_time();
				display_time();
				set_LED(led[3]);
				delay_ms(1000);
				currentTime = currentTime++;
							
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;

				if((newState==0))
				{
					newState = 1;
				}				

			} while (currentTime < minTime);	
			do
			{
				if((newState==0))
				{
					newState = 1;
				}			
				
			}while(newState==0);
	
			if(newState==1)
			{
				state = MAIN_STRAIGHT;
			}

			break;
			
//===========================MAIN ROAD WALK===========================			
			case(MAIN_WALK):
			currentTime = 0;
			newState = 0;
			do 
			{
				get_time();
				display_time();
				set_LED(led[4]);
				delay_ms(1000);
				currentTime = currentTime++;
							
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;
				
			} while (currentTime < minTime);

			break;
			
//==========================SIDE ROAD WALK============================
			case(SIDE_WALK):
			currentTime = 0;
			newState = 0;
			do 
			{
				get_time();
				display_time();
				set_LED(led[5]);
				delay_ms(1000);
				currentTime = currentTime++;
							
				checkPortB = PORTB&0xF0;
				checkPortB = checkPortB>>0x4;
				
			} while (currentTime < minTime);
			break;		
		}
//=======================END STATE MACHINE============================
	}
}	

//======================SETUP REAL TIME CLOCK=========================
void setup_clock(void)
	{
	OpenI2C(MASTER,SLEW_OFF); //enable I2C at 400KHz
	StartI2C();
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	WriteI2C(0xD0); //select DS1307
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	WriteI2C(0x07);
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	WriteI2C(0x10);
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	StopI2C();
	IdleI2C();
	StartI2C(); //send start sequence
	IdleI2C();
	WriteI2C(0xD0);
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	WriteI2C(0xD1);
	while(SSPCON2bits.ACKSTAT!=0);
	IdleI2C();
	StopI2C();
	IdleI2C();
	CloseI2C();
	}
//=======================GET TIME FROM RTC============================
void get_time(void)
	{
	OpenI2C(MASTER,SLEW_OFF); //enable I2C at 400KHz
	IdleI2C();
	StartI2C();
	WriteI2C(0xD0); //write to DS1307
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	WriteI2C(0x00);
	while(SSPCON2bits.ACKSTAT!=0);
	IdleI2C();
	StopI2C();
	IdleI2C();
	StartI2C();
	IdleI2C();
	WriteI2C(0xD1); //read from DS1307
	while(SSPCON2bits.ACKSTAT!=0); //wait for acknowledge
	IdleI2C();
	seconds=ReadI2C()&0x7F;
	AckI2C(); //generate acknowledge
	IdleI2C();
	minutes=ReadI2C()&0x7F;
	AckI2C();
	IdleI2C();
	hours=ReadI2C()&0x3F;
	NotAckI2C(); //generate not acknowledge
	IdleI2C();
	StopI2C();
	IdleI2C();
	CloseI2C();
	}
//===================DISPLAY TIME ON LCD AND TERMINAL=================
void display_time(void)
	{
	set_address_line1(1);
	WriteUSART('\r');
	write_data((hours>>4) + 48);
	WriteUSART((hours>>4) + 48);
    write_data((hours&0x0F)+ 48);
	WriteUSART((hours&0x0F)+ 48);
	write_data(' '); //place a space
	WriteUSART(' '); //place a space
    write_data((minutes>>4) + 48);
    WriteUSART((minutes>>4) + 48);
    write_data((minutes&0x0F)+ 48);
    WriteUSART((minutes&0x0F)+ 48);
	write_data(' '); //place a space
	WriteUSART(' '); //place a space
	write_data((seconds>>4) + 48);
	WriteUSART((seconds>>4) + 48);
    write_data((seconds&0x0F)+ 48);
	WriteUSART((seconds&0x0F)+ 48);
	}
//===================DISPLAY MESSAGE ON LCD===========================
	void display_LCD(void)
	{
	set_address_line1(1);
	write_data((hours>>4) + 48);
    write_data((hours&0x0F)+ 48);
	write_data(' '); //place a space
    write_data((minutes>>4) + 48);
    write_data((minutes&0x0F)+ 48);
	write_data(' '); //place a space
	write_data((seconds>>4) + 48);
    write_data((seconds&0x0F)+ 48);
	}

//=======================GREEN LIGHT CONTROL==========================
	void set_LED(int led)
	{
		switch(led)
		{
			case(1):
			PORTA = 0x0;
			break;
			
			case(2):
			PORTA = 0x1;			
			break;
			
			case(3):
			PORTA = 0x2;			
			break;
			
			case(4):
			PORTA = 0x3;	
			break;
			
			case(5):
			PORTA = 0x4;			
			break;	
			
			case(6):
			PORTA = 0x5;			
			break;	
		}		
	}
		
//=======================YELLOW LIGHT CONTROL=========================
	void blink_LED(int led)
	{
		switch(led)
		{
			case(1):
			PORTA = 0x0;
			break;
			
			case(2):
			PORTA = 0x1;			
			break;
			
			case(3):
			PORTA = 0x2;			
			break;
			
			case(4):
			PORTA = 0x3;	
			break;
			
			case(5):
			PORTA = 0x4;			
			break;	
			
			case(6):
			PORTA = 0x5;			
			break;	
		}		
	}
	
//=========================INITIALIZATION=============================		
void init (void)
{	
	RW   = 1 ;  // permanent write selection for display
	
	TRISC=0b00001000; //set PORTC Bit 3 (SCL) to INPUT
	TRISD=0x00;	// set PORTD as outputs
	TRISA=0b11110000; //set PORTA I/O
	TRISB=0xFF; //use PORTB as inputs
	
	PORTA=0x00;
	PORTD=0x00;	// clear PORTS			
	PORTB=0x00;
	
	POWER = ON; // turns on power to LCD
	state=MAIN_STRAIGHT; // set default state

//init USART	
	OpenUSART( USART_TX_INT_OFF &
	USART_RX_INT_OFF &
	USART_ASYNCH_MODE &
	USART_EIGHT_BIT &
	USART_CONT_RX &
	USART_BRGH_HIGH,
	25 );	
}

//========================END OF FUNCTIONS============================
