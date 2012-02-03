/** \addtogroup ext */
/*@{*/
/**
 *	\file	include/avr-halib/ext/sensor.h
 *	\brief	Defines classes AnalogSensor and AnalogSensorInterrupt
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once

#include "avr-halib/avr/adc.h"



/// Pseudo interface to be implemented by Sensors
#if 0
template< class ReturnT > class Sensor
{
protected:
	typedef ReturnT ReturnType;
	ReturnType value;
public:
	Sensor(){}
	
	/// returns true if process started by startGetValue() had been completed.
	bool isReady(){return true;}
	
	/// returns true if value-getting-process could be started
	bool startGetValue()
	{
		//here the process of getting the realworld-value to this.value schould be started
		return true;
	}
	
	/// returns true if isReady() and writes gotten realworld-value to value
	bool getCachedValue(ReturnType &value)
	{
		if(isReady())
		{
			value = this.value;
			return true;
		}
		else	
			return false;
	}
};
#endif

template<class SensorConfig> 
	class AnalogSensorInterrupt:
		public AnalogDigitalConverterInterrupt<
			typename SensorConfig::ReturnType,
			typename SensorConfig::ADConverter>
{
	public:
		typedef typename SensorConfig::ReturnType ReturnType;
	private:
		ReturnType value;
		
	protected:
	public:
		/// returns true if process started by startGetValue() had been completed.
		bool isReady(){return !this->isThatTarget(value);}
		
		/// returns true if value-getting-process could be started
		bool startGetValue()
		{
			//here the process of getting the realworld-value to this.value schould be started
			return this->getValue( value, SensorConfig::mux, SensorConfig::refV, SensorConfig::prescaler );
		}
		
		/// returns true if isReady() and writes gotten realworld-value to value
		bool getCachedValue(ReturnType &value)
		{
			if(isReady())
			{
				value = this->value;
				return true;
			}
			else	
				return false;
		}
};

template<class SensorConfig> 
	class AnalogSensor:
		public AnalogDigitalConverter<
			typename SensorConfig::ReturnType,
			typename SensorConfig::ADConverter>
{
	public:
		typedef typename SensorConfig::ReturnType ReturnType;
	private:
		ReturnType value;
		
	protected:
	public:
		AnalogSensor(){}
		
		/// returns true if process started by startGetValue() had been completed.
		bool isReady(){return true;}
		
		/// returns true if value-getting-process could be started
		bool startGetValue()
		{
			//here the process of getting the realworld-value to this.value schould be started
			return this->getValue( value, SensorConfig::mux, SensorConfig::refV, SensorConfig::prescaler );
		}
		
		/// returns true if isReady() and writes gotten realworld-value to value
		bool getCachedValue(ReturnType &value)
		{
			if(isReady())
			{
				value = this->value;
				return true;
			}
			else	
				return false;
		}
};

/*@}*/
