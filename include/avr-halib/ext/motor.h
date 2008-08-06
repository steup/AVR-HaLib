/**
 *	\file	include/avr-halib/ext/motor.h
 *	\brief	Defines Motor
 *	\author	Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */

#pragma once



/**
 *	
 *	\brief	Generic Motor class
 *	\param	MotorConnectClass	Class that controls motor hardware, must contain a function
 *					<tt>void doSetSpeed(uint8_t s, bool forward)</tt> which
 *					sets the motor speed (0 to 200) and direction.
 *
 *
 */
template <class MotorConnectClass>
class Motor : public MotorConnectClass
{
protected:
	// Calibration factor in hundredth
	int8_t calibrationFactor;
	
	// Speed of motor (-100 to 100)
	int8_t speed;
	
	// Absolute speed of motor, calibration factor included in calculation (0 bis 200)
	uint8_t absSpeed;
	
	// Recalculates \c absSpeed and returns if direction is forward
	bool updateSpeed()
	{
		// -100 <= speed <= 100
		// -200 <= s <= 200
		// 0 <= absSpeed <= 200
		int16_t s = (((int16_t)speed) * ((int16_t)calibrationFactor)) / 50; 
		absSpeed = (s < 0 ? -s : s);
		return s > 0;
	}


public:
	
	Motor() :
		calibrationFactor(100),
		speed(0),
		absSpeed(0)
	{
	}

	/** 	\brief	Set speed of motor
	 *	\param	s	Speed (-100 to 100)
	 */
	void setSpeed(int8_t s)
	{
		if (s > 100)
			s = 100;
		else if (s < -100)
			s = -100;

		speed = s;
		bool forward = updateSpeed();
		MotorConnectClass::doSetSpeed(absSpeed, forward);
	}
	
	/**	\brief	Returns speed of motor
	 *	\return	Speed (-100 to 100)
	 */
	int8_t getSpeed()
	{
		return speed;
	}
	
	/**	\brief	Set calibration factor
	 *	\param factorDiv100	Calibration factor is factorDiv100 / 100 (Calibration by slow down of one motor).
	 *				-100 <= factorDiv100 <= 100, factorDiv100 != 0!
	 *	
	 *	\par	Examples
	 *	\code
	 *	motor.setCalibrationFactor(100);   // Calibrate motor full speed forward (default)
	 *	motor.setCalibrationFactor(50);    // Calibrate motor half speed forward
	 *	motor.setCalibrationFactor(-75);   // Calibrate motor 3/4 speed backward
	 *	\endcode
	 */
	void setCalibrationFactor(int8_t factorDiv100)
	{
		if (!factorDiv100)
			return;
		if (factorDiv100 > 100)
			factorDiv100 = 100;
		else if (factorDiv100 < -100)
			factorDiv100 = -100;

		calibrationFactor = factorDiv100;
		bool forward = updateSpeed();
		MotorConnectClass::doSetSpeed(absSpeed, forward);
	}
	
	/**	\brief	Returns calibration factor
	 *	\return	Kalibrierungsfaktor (-100 bis 100)
	 */
	int8_t getCalibrationFactor()
	{
		return calibrationFactor;
	}

};

