/** \addtogroup avr */
/*@{*/
/**
 *	\file	avr-halib/avr/spi.h
 *	\brief	Defines Uart
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once


#include "avr-halib/avr/interrupt.h"
#include "avr-halib/avr/regmaps.h"



/*!
 *	\class Spi spi.h "avr-halib/avr/spi.h"
 *	\brief SPI Interface
 *	\param SpiRegmap Register map
 *	\param length_t	Type used for size of the buffers and addressing the buffers
 *	\param oBufLen	Size of output buffer
 *	\param iBufLen	Size of input buffer
 *
 *	For reading and writing strings and integers see \see doc_cdevices
 */
template <class SpiRegmap>	class SpiMaster
{
protected:
	typedef class SpiRegmap::Controller_Configuration Controller_Configuration;
	
	
public:
	
	/// Constructor
	SpiMaster()
	{
		init();
	}
	
	void onTransmissionComplete()
	{}
	
	/// (Re)Initializes Spi
	void init()
	{
		UseRegmap(rm, SpiRegmap);
		
		rm.spe = true;
		rm.mstr = true;
		
		rm.spie = SpiRegmap::useInterupt;
		
		rm.dord = SpiRegmap::dataDirection;
		rm.cpol = SpiRegmap::leadingEdge;
		rm.cpha = SpiRegmap::setupEdge;
		
		rm.spr0 = 0x02 & SpiRegmap::clockPrescaler;
		rm.spr1 = 0x04 & SpiRegmap::clockPrescaler;
		rm.spi2x = !(0x01 & SpiRegmap::clockPrescaler); //Bit negieren
		
		rm.mosi.ddr=true;
		rm.miso.ddr=false;
		rm.sck.ddr=true;
		// Reset Flags
		do
		{
			rm.wcol = true;
			rm.spif = true;
			uint8_t dummy;
			(void) (dummy = rm.spdr);
			SyncRegmap(rm);
		}
		while (rm.spif || rm.wcol);
		
		SyncRegmap(rm);
		
		// Set ISR for Interrupt
		if(SpiRegmap::useInterupt) SpiRegmap::template setSpiInterrupt<SpiMaster<SpiRegmap>, & SpiMaster<SpiRegmap>::onTransmissionComplete > (*this);
	}
	
	
	
	/// Writes a Char to the Spi and starts Tranmission
	void put(const char c)
	{
		UseRegmap(rm, SpiRegmap);
		rm.spdr = c;
		SyncRegmap(rm);
		while(SpiRegmap::bussywaitput && !rm.spif)SyncRegmap(rm);
	}
	
	/**	\brief	Reads a character from the spdr buffer
	 *	\param	c	Reference to variable which shall store the character
	 *	\return		true if a character was read
	 */
	
	
	bool ready()__attribute__ ((always_inline))
	{
		UseRegmap(rm, SpiRegmap);
		SyncRegmap(rm);
		return rm.spif;
	}
	/**	\brief	Reads a character from the spdr buffer
	 *	\param	c	Reference to variable which shall store the character
	 *	\return		true if a character was read
	 */
	bool get(char & c)
	{
		UseRegmap(rm, SpiRegmap);
		SyncRegmap(rm);
		c = rm.spdr;
		return true;
	}
	

};
