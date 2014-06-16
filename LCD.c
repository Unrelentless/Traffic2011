#include <p18f4520.h>
#include "LCD.h"
#include "delay.h"


void init_display(void)
		{
		LCD_TRIS=0x00; //set the data direction for LCD PORT
		LCD_PORT=0x00;
		POWER=1; //enable the LCD display
		RW=0; //always write data to display
		delay_ms(15);	//15msec
		write_8bit_data(0x30) ;   // data = 0011 RS = 0 = command	
		delay_ms(5); //5msec
		write_8bit_data(0x30) ;
		delay_ms(1); //160 uSec
		write_8bit_data(0x30) ;
		delay_ms(1);	//160 uSec
		write_8bit_data(0x20) ;  // set  for 4 bit operation
		delay_ms(1);	//160 uSec
		write_4bit_data(0x28) ;   // set operating conditions
		write_4bit_data(0x08) ;   // display off
		write_4bit_data(0x01) ;   // clear display
		write_4bit_data(0x06) ;   // shift
		write_4bit_data(0x0C) ;   // display on
		RS = 1 ; // set for data transmission
		}

void	write_8bit_data(char data_to_send)	       
					{
					PORTD=PORTD&0xF0; // clear lower 4 bits
					PORTD = PORTD|((data_to_send>>4) & 0x0F); // put 0011 on the data lines = 8 bit //operation
						       // sets the RS line to "0" i.e. a //command
					EN = 1 ;
					EN = 0 ;       // blip the ENABLE line	
					}


void	write_4bit_data(char byte)		
					{
					PORTD=(PORTD & 0xF0)|((byte>>4) & 0x0F) ;
					EN=1;
					EN=0;
				    delay_ms(6);
					PORTD=(PORTD & 0xF0)|(byte & 0x0F);
					EN=1;
					EN=0;
					delay_ms(6);
					}
void write_data(unsigned char data_to_write)
					{
					write_4bit_data(data_to_write);
					}
void write_command(unsigned char command_to_write)
					{
					RS=0; //set for command
					write_4bit_data(command_to_write);
					RS=1; // set for data
					}
void set_address_line1(unsigned char position)
					{
					write_command(0x7F + position);
					} // postion range 0-15
void set_address_line2(unsigned char position)
					{
					write_command(0xBF + position);
					} //position range 0-15
void clear_display(void)
					{
					write_command(0x01);
					delay_ms(60); //allow time for clearing
					}
void send_msg(unsigned char msg[])
		{
		unsigned char *ptr;
		ptr=&msg[0]; //initialise the pointer
		for(ptr;*ptr!='\0';ptr++)
		write_4bit_data(*ptr);
		}
