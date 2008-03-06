
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>


#define delay_us _delay_us
#define delay_ms _delay_ms

// PIN COLLECTION

struct LcdPinCollection
{
	volatile uint8_t pina : 8;
	volatile uint8_t ddra : 8;
	volatile uint8_t porta : 8;
	
	volatile uint8_t pinb : 8;
	volatile uint8_t ddrb : 8;
	volatile uint8_t portb : 8;
	
	volatile uint8_t pinc : 8;
	volatile uint8_t ddrc : 8;
	volatile uint8_t portc : 8;
	
	volatile uint8_t pind : 7;
	volatile bool pinE : 1;
	volatile uint8_t ddrd : 7;
	volatile bool ddrE : 1;
	volatile uint8_t portd : 7;
	volatile bool portE : 1;
	
	volatile uint8_t pine : 4;
	volatile bool pin4 : 1;
	volatile bool pin5 : 1;
	volatile bool pin6 : 1;
	volatile bool pin7 : 1;
	volatile uint8_t ddre : 4;
	volatile bool ddr4 : 1;
	volatile bool ddr5 : 1;
	volatile bool ddr6 : 1;
	volatile bool ddr7 : 1;
	volatile uint8_t porte : 4;
	volatile bool port4 : 1;
	volatile bool port5 : 1;
	volatile bool port6 : 1;
	volatile bool port7 : 1;
	
	volatile uint8_t pinf : 6;
	volatile bool pinRW : 1;
	volatile bool pinRS : 1;
	volatile uint8_t ddrf : 6;
	volatile bool ddrRW : 1;
	volatile bool ddrRS : 1;
	volatile uint8_t portf : 6;
	volatile bool portRW : 1;
	volatile bool portRS : 1;
};

#define lcdPins (*((LcdPinCollection *)&PINA))



// --------------------------------------------------------------------------------	   
	
class LCD
{
	void write(char data, bool wait, bool command);
	void wait_busy();
public:
	LCD();
	void print(const char * s);
	void print(const char * s, int n);
	void setPos(uint8_t pos);
};

void LCD::setPos(uint8_t pos)
{
	if (pos < 0x60)
	{
		// Sprung an Position pos
		write(0x80 | pos, true, true);
	}
}

	
LCD::LCD()
{
	// power on	
	delay_ms(16);				
	// initialisierungsmuster 		 000 0010 xxxx = 32
	write(0x20, false, true);
	delay_us(5000);
	write(0x20, false, true);
	delay_us(150);
	// 4 Bit modus				 000 0010 1100 = 44
	write(0x2c, false, true);	
	delay_us(5000);
	// Display off 				 000 0000 1110 = 14
	write(0x0e, true, true);
	// clear display 			 000 0000 0001 = 1
	write(0x01, true, true);
	// input mode				 000 0000 0110 = 6		
	write(0x06, true, true);
}

void LCD::wait_busy()
{	
	// LCD 7 als Input
	lcdPins.port7 = true;
	lcdPins.ddr7 = false;
	// RW = Read
	lcdPins.portRW = true;
	// RS auf low
	lcdPins.portRS = false;
	// enable Bit setzen
	lcdPins.portE = true;
	while (lcdPins.pin7);
	// enable Bit wieder loeschen
	lcdPins.portE = false;
	
	// LCD 7 als Output
	lcdPins.ddr7 = true;
	lcdPins.port7 = false;
	
	lcdPins.portRW = false;
	lcdPins.portRS = true;
}


void LCD::write(char data, bool wait, bool command)
{
	// alles als Output definieren
	lcdPins.ddrRS = true;
	lcdPins.ddrRW = true;
	lcdPins.ddrE = true;
	lcdPins.ddr4 = true;
	lcdPins.ddr5 = true;
	lcdPins.ddr6 = true;
	lcdPins.ddr7 = true;
	
	lcdPins.portRS = !command;
	lcdPins.portRW = false;		// Schreiben
	lcdPins.portE = false;

	// Datenuebergabe jeweils 4 Bit
	lcdPins.port4 = (data & 0x10);
	lcdPins.port5 = (data & 0x20);
	lcdPins.port6 = (data & 0x40);
	lcdPins.port7 = (data & 0x80);
	
	// Enable Bit setzen und wieder loeschen
	lcdPins.portE = true;
	lcdPins.portE = false;
	
	// Datenuebergabe jeweils 4 Bit
	lcdPins.port7 = (data & 0x08);
	lcdPins.port6 = (data & 0x04);
	lcdPins.port5 = (data & 0x02);
	lcdPins.port4 = (data & 0x01);
	
	// Enable Bit setzen und wieder loeschen
	lcdPins.portE = true;
	lcdPins.portE = false;
	
	if (wait)
	{
		wait_busy();
	}
}

void LCD::print(const char *s)
{
	while (*s)
		write(*s++, true, false);
}	 

void LCD::print(const char *s, int n)
{
	for (int i = 0; i < n; i++)
		write(s[i], true, false);
}	 


int main () 
{	
	const char str [] = "Fischers Fritze fischt frische Fische! Frische Fische fischt Fischers Fritz! --- ";
//	const char str [] = "1-2-3-4-5-6-7-8-9-10-11-12-13-14-15-16-17-18-19-20-21-22-23-24-25-26-27-28-29-30 --- ";
	const int str_length = sizeof(str) - 1;
	int pos[] = { 0, 16, 32, 48 };
	{
		LCD lcd;
		while (1)
		{
			lcd.setPos(0x0);
			if (pos[0] + 16 > str_length)
			{
				lcd.print(str + pos[0], str_length - pos[0]);
				lcd.print(str, 16 - (str_length - pos[0]));
			}
			else
				lcd.print(str + pos[0], 16);
			
			lcd.setPos(0x40);
			if (pos[1] + 16 > str_length)
			{
				lcd.print(str + pos[1], str_length - pos[1]);
				lcd.print(str, 16 - (str_length - pos[1]));
			}
			else
				lcd.print(str + pos[1], 16);
			
			lcd.setPos(0x10);
			if (pos[2] + 16 > str_length)
			{
				lcd.print(str + pos[2], str_length - pos[2]);
				lcd.print(str, 16 - (str_length - pos[2]));
			}
			else
				lcd.print(str + pos[2], 16);
			
			lcd.setPos(0x50);
			if (pos[3] + 16 > str_length)
			{
				lcd.print(str + pos[3], str_length - pos[3]);
				lcd.print(str, 16 - (str_length - pos[3]));
			}
			else
				lcd.print(str + pos[3], 16);
			
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			delay_ms(255);
			
			(++pos[0]) %= str_length;
			(++pos[1]) %= str_length;
			(++pos[2]) %= str_length;
			(++pos[3]) %= str_length;
		}
	}
	
	while(true);
} 
