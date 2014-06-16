	#define RS PORTDbits.RD4
	#define EN PORTDbits.RD6
	#define RW PORTDbits.RD5
	#define POWER PORTDbits.RD7 
	#define ON  1
	#define OFF 0
	#define LCD_TRIS	TRISD
	#define LCD_PORT	PORTD
	
void write_4bit_data(char);
void write_8bit_data(char);
void init_display(void);
void send_msg(unsigned char msg[]);
void clear_display(void);
void set_address_line1(unsigned char);
void set_address_line2(unsigned char);
void write_command(unsigned char);
void write_data(unsigned char);

