/** 
 *	\file	include/halib/share/simplifysensor.h
 *	\brief	Defines SimplifySensor
 *
 *	\author Karl Fessel
 */

/**
 *	\class	SimplifySensor simplifysensor.h "halib/share/simplifysensor.h"
 *	\brief	Simplifys use of Sensor classes
 *	\param	ReturnType	type of the value returned and the memory it needs
 *	\param	Sensor		Sensor class to use
 *	
 *	\par Pseudo interface to be implemented by Sensors
 *	template<class ReturnType> class Sensor
 *	{
 *	protected:
 *		ReturnType value;
 *	public:
 *		Sensor(){}
 *		
 *		/// returns true if process started by startGetValue() had been completed.
 *		bool isReady(){return true;}
 *	
 *		/// returns true if value-getting-process could be started
 *		bool startGetValue()
 *		{
 *			//here the process of getting the realworld-value to this.value schould be started
 *			return true;
 *		}
 *	
 *		/// returns true if isReady() and writes gotten realworld-value to value
 *		bool getCachedValue(ReturnType &value)
 *		{
 *			if(isReady())
 *			{
 *				value = this.value;
 *				return true;
 *			}
 *			else	
 *				return false;
 *		}
 *	};
 */
template <class ReturnType,class Sensor> SimplifySensor:public Sensor<class ReturnType>
{
	public:
	
	ReturnType getValue()
	{
		while (!startGetValue());
		ReturnType value;
		while (!getCachedValue(value));
		return value;
	}
};