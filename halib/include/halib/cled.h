/**
 *	\file	halib/cled.h
 *	\brief	Definiert und Implementiert die Led-Char-Devices
 */


#pragma once

#include "halib/queuebuffer.h"
#include "halib/cdevice.h"
#include "halib/misc.h"

/*!
 * \brief character Device zur Ausgabe von Zeichen auf einem LedBlock
 *
 * \param length_t	Typ f�r die Indexierung / Gr��e des Puffer
 * \param bufLen	Gr��e des Puffers
 * 
 */

template <class length_t, length_t bufLen> class CLed : public CDevice
{
	private:
		QueueBuffer<char, length_t, bufLen> buffer;
		LedBlock ledb;
	public:
	
		/*!
		 * \brief Konstruktor
		 * \param leds der LedBlock auf dem die Ausgabe erfolgen soll
		 */
		CLed(LedBlock & leds) : ledb(leds){}


		/**
		 *  putc nimmt ein Zeichen an und speichert es in Puffer
		 */
		void putc (const char c)
		{
			buffer.put(c);
		}
		
		/**
		 *  getc gibt 0 zur�ck 
		 */ 
		char getc()
		{
			return 0;
		}
		
		/*!
		 *  gibt ein Zeichen aus dem Speicher auf dem LedBlock aus
		 */
		void step()
		{
			char i = buffer.get();
			if (i)
				ledb.setLedPattern(i);
		
		}
};

/*!
 * \brief character Device zur Ausgabe von Zeichen auf einem LedBlock wobei Timer 0 die ausgabe des n�chsten Zeichen ausl�st.
 *
 * \param length_t	Typ f�r die Indexierung / Gr��e des Puffer
 * \param bufLen	Gr��e des Puffers
 * 
 */

template <class length_t, length_t bufLen> class CLedTimer : public CDevice, public Timer0
{
	private:
		QueueBuffer<char, length_t, bufLen> buffer;
		LedBlock ledb;
	public:
		
		/*! 
		 * \brief Konstruktor
		 * \param leds der LedBlock auf dem die Ausgabe erfolgen soll
		 */
		CLedTimer(LedBlock & leds) : ledb(leds)
		{
			start(TIMER_PS_1024);
		}


		/**
		 *  putc nimmt ein Zeichen an und speichert es in Puffer
		 */
		void putc (const char c)
		{
			buffer.put(c);
		}
		
		/**
		 *  getc gibt 0 zur�ck 
		 */ 
		char getc()
		{
			return 0;
		}
		
		/*!
		 *  wird von Timer aufgerufen und gibt ein Zeichen aus dem Speicher auf dem LedBlock aus
		 */
		void onTimer()
		{
			char i = buffer.get();
			if (i)
				ledb.setLedPattern(i);
		}
};
