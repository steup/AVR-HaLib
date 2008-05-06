// Portmap for Robbyboard used in the EOS at university of magdeburg
// 
// Author:	Philipp Werner

#pragma once

#include <stdint.h>

// LedPortmap
struct Led0		// portmap for at90can128
{
	enum { onLevel = true };
	union
	{
		struct		// pin led: 0;
		{
			uint8_t __pad0 [0x20];
			bool pin : 1;		// PINA (0x20), bit 0
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 0
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 0
		} led;
	};
};

 
// LedPortmap
struct Led1		// portmap for at90can128
{
	enum { onLevel = true };
	union
	{
		struct		// pin led: 1;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 1;
			bool pin : 1;		// PINA (0x20), bit 1
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 1
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 1
		} led;
	};
};

 
// LedPortmap
struct Led2		// portmap for at90can128
{
	enum { onLevel = true };
	union
	{
		struct		// pin led: 2;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 2;
			bool pin : 1;		// PINA (0x20), bit 2
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 2
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 2
		} led;
	};
};

 
// LedPortmap
struct Led3		// portmap for at90can128
{
	enum { onLevel = true };
	union
	{
		struct		// pin led: 3;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 3;
			bool pin : 1;		// PINA (0x20), bit 3
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 3
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 3
		} led;
	};
};


// LedBlock
struct LedBlock0123		// portmap for at90can128
{
	enum { onLevel = 0x0ff };
	union
	{
		struct		// vportleds
		{
			uint8_t __pad0 [0x20];
			uint8_t led0123Pin : 4;		// PINA (0x20), bit 0-3
			uint8_t : 4;
			uint8_t led0123Ddr : 4;		// DDRA (0x21), bit 0-3
			uint8_t : 4;
			uint8_t led0123Port : 4;		// PORTA (0x22), bit 0-3
			uint8_t getPin()
			{
				return (led0123Pin);
			}
			void setPin(uint8_t i)
			{
				led0123Pin = (i & 0xf);
			}
			uint8_t getPort()
			{
				return (led0123Port);
			}
			void setPort(uint8_t i)
			{
				led0123Port = (i & 0xf);
			}
			uint8_t getDdr()
			{
				return (led0123Ddr);
			}
			void setDdr(uint8_t i)
			{
				led0123Ddr = (i & 0xf);
			}
		} leds;
	};
};


// ButtonPortmap
struct Button0		// portmap for at90can128
{
	enum { pressedLevel = true, usePullup = true };
	union
	{
		struct		// pin button: 4;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 4;
			bool pin : 1;		// PINA (0x20), bit 4
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 4
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 4
		} button;
	};
};

 
// ButtonPortmap
struct Button1		// portmap for at90can128
{
	enum { pressedLevel = true, usePullup = true };
	union
	{
		struct		// pin button: 5;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 5;
			bool pin : 1;		// PINA (0x20), bit 5
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 5
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 5
		} button;
	};
};


// ButtonPortmap
struct Button2		// portmap for at90can128
{
	enum { pressedLevel = true, usePullup = true };
	union
	{
		struct		// pin button: 6;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 6;
			bool pin : 1;		// PINA (0x20), bit 6
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 6
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 6
		} button;
	};
};


// ButtonPortmap
struct Button3		// portmap for at90can128
{
	enum { pressedLevel = true, usePullup = true };
	union
	{
		struct		// pin button: 7;
		{
			uint8_t __pad0 [0x20];
			uint8_t : 7;
			bool pin : 1;		// PINA (0x20), bit 7
			uint8_t : 7;
			bool ddr : 1;		// DDRA (0x21), bit 7
			uint8_t : 7;
			bool port : 1;		// PORTA (0x22), bit 7
		} button;
	};
};


// ButtonBlockPortmap
struct ButtonBlock0123		// portmap for at90can128
{
	enum { pressedLevel = 0xff, usePullup = 0xff };
	union
	{
		struct		// vportbuttons
		{
			uint8_t __pad0 [0x20];
			uint8_t : 4;
			uint8_t buttonsPin : 4;		// PINA (0x20), bit 4-7
			uint8_t : 4;
			uint8_t buttonsDdr : 4;		// DDRA (0x21), bit 4-7
			uint8_t : 4;
			uint8_t buttonsPort : 4;		// PORTA (0x22), bit 4-7
			uint8_t getPin()
			{
				return (buttonsPin);
			}
			void setPin(uint8_t i)
			{
				buttonsPin = (i & 0xf);
			}
			uint8_t getPort()
			{
				return (buttonsPort);
			}
			void setPort(uint8_t i)
			{
				buttonsPort = (i & 0xf);
			}
			uint8_t getDdr()
			{
				return (buttonsDdr);
			}
			void setDdr(uint8_t i)
			{
				buttonsDdr = (i & 0xf);
			}
		} buttons;
	};
};




#include <avr/io.h>

/// Uses Timer/Counter1
class RobbyMotorDriver
{
	/**	Timer/Counter1 in 8 bit PWM phase correct mode with output channels A and B
	 *	Clear OC1A/OC1B on compare-match when up- and set when down-counting
 	*/

	enum { ps0 = 0, ps1 = 1, ps8 = 2, ps64 = 3, ps256 = 4, ps1024 = 5, exClkFall = 6, exClkRise = 7 };

	void start()
	{
		// Clear OC1A/OC1B on compare-match when up- and set when down-counting
		TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
	}

	void stop()
	{
		// Normal Port Operation, OC1A/OC1B disconnected
		TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
	}

	bool isRunning()
	{
		return TCCR1A & (1 << COM1A1);
	}

public:
	RobbyMotorDriver()
	{
		// set OC1A/OC1B pins as outputs
		DDRB |= (1 << 5) | (1 << 6);
	
		// Use Timer1 in Phase correct PWM (8 Bits) - mode 1
		TCCR1A = 0x01;
		TCCR1B = ps256 & 0x7;
		TCCR1C = 0;
	
		OCR1AL = 0;
		OCR1BL = 0;
	
		// direction pins for first motor as output
		DDRE |= (1<<2) | (1<<3);
		// direction pins for second motor as output
		DDRG |= (1<<0) | (1<<1);	
		
		setSpeedA(0);
		setSpeedB(0);
	
		setForwardA(true);
		setForwardB(true);
	
		start();
	}

	// 0 <= a <= 200
	void setSpeedA(uint8_t a)
	{
		if (!a)
			OCR1AL = 0;
		else
			OCR1AL = a + 55;
	}

	void setSpeedB(uint8_t b)
	{
		if (!b)
			OCR1BL = 0;
		else
			OCR1BL = b + 55;
	}
	
	void setForwardA(bool forward)
	{
		if (forward)
		{
			PORTE |= (1 << 2);
			PORTE &= ~(1 << 3);
		}
		else
		{
			PORTE &= ~(1 << 2);
			PORTE |= (1 << 3);
		}
	}

	void setForwardB(bool forward)
	{
		if (forward)
		{
			PORTG |= (1 << 0);
			PORTG &= ~(1 << 1);
		}
		else
		{
			PORTG &= ~(1 << 0);
			PORTG |= (1 << 1);
		}
	}
};


template <class MotorDriver>
class RobbyMotorA : public MotorDriver
{
public:
	void doSetSpeed(int8_t s, bool forward)
	{
		MotorDriver::setSpeedA(s);
		MotorDriver::setForwardA(forward);
	}
};


template <class MotorDriver>
class RobbyMotorB : public MotorDriver
{
public:
	void doSetSpeed(int8_t s, bool forward)
	{
		MotorDriver::setSpeedB(s);
		MotorDriver::setForwardB(forward);
	}
};



