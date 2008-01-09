/**
 *	\file halib/port.h
 *	\brief	Definiert Port-Interface
 *
 */
 
#pragma once
#include <avr/io.h>
#include "halib/config.h"



class Port
{
public:
	virtual void set(const port_t pattern)=0;
	virtual port_t get()=0;
	virtual port_t getPin()=0;
	virtual void  setDirection(const port_t pattern)=0;
	virtual port_t getDirection()=0;
}

class SimplePort:Port
{
	volatile port_t & port;
	volatile port_t & ddr;
public:
	SimplePort(volatile port_t & port, volatile port_t & ddr);
	void set(const port_t pattern);
	port_t get();
	port_t getPin();
	void setDirection(const port_t pattern);
	port_t getDirection();
	
}

class ShiftedPort:Port
{
	volatile port_t & port;
	volatile port_t & ddr;
	uint8_t shift;
	port_t mask;
public:
	ShiftedPort(volatile port_t & port,volatile port_t &pin volatile port_t & ddr, const port_t mask, const uint8_t shift);
	void set(const port_t pattern);
	port_t get();
	port_t getPin();
	void setDirection(const port_t pattern);
	port_t getDirection();

}

