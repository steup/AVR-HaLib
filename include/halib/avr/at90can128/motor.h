/**
 *	\file	halib/at90can128/motor.h
 *	\brief	Stellt eine Motor-Klasse bereit.
 *
 *	\warning	Timer2 wird f&uuml;r die Software-PWM des Motors reserviert und kann somit bei Betrieb des Motors nicht anderweitig verwendet werden.
 *	\see \ref	halibmotoren
 *
 */

#pragma once


/*! \brief Implementiert Ansteuerung eines Motors.
 *	
 *	Es k&ouml;nnen beliebig viele Instanzen dieser Klasse existieren (im Rahmen der
 * verf&uuml;gbaren Resourcen), also auch mehr als zwei Motoren betrieben werden.
 *
 *	\see \ref	halibmotoren
 */
class Motor
{
	/// N&auml;chster Motor in der verketteten Liste aller Motoren
	Motor * nextMotor;
	
	/// Kalibierungsfaktor in Hundertsiebenundzwanzigsteln
	int8_t calibrationFactor;
	
	/// Geschwindigkeit des Motors
	int8_t speed;
	
	/// Absolute Geschwindigkeit des Motors, Kalibrierungsfaktor und Minimum-Speed einbezogen. F&uuml;r PWM verwendeter Wert.
	uint8_t absSpeed;
	
	volatile port_t & port;		///< Port, &uuml;ber den der Motor ansprechbar ist.
	const port_t enableMask;		///< Bitmaske: enable
	const port_t forwardMask;		///< Bitmaske: vorw&auml;rts
	const port_t backwardMask;		///< Bitmaske: r&uuml;ckw&auml;rts

	friend class MotorPwm;

	/// Berechnet \c absSpeed neu und passt ggf. PWM an. Wird aufgerufen von setSpeed() und setCalibrationFactor().
	void speedChanged();
 
protected:

	inline void enable()		{ port |= enableMask; }
	inline void disable()		{ port &= ~enableMask; }
	inline void stop()		{ port &= ~forwardMask; port &= ~backwardMask; }
	inline void forward()		{ port |= forwardMask; port &= ~backwardMask; }
	inline void backward()		{ port &= ~forwardMask; port |= backwardMask; }

public:

	/** Konstruktor.
	 *	\param	port	Port, &uuml;ber den der Motor ansprechbar ist.
	 *	\param	enableMask	Bitmaske zum enablen des Motors.
	 *	\param	forwardMask	Bitmaske, die Pins kennzeichnet, an denen 1 ausgegeben werden muss, damit der Motor vorw&auml;rts dreht.
	 *	\param	backwardMask	Bitmaske, die Pins kennzeichnet, an denen 1 ausgegeben werden muss, damit der Motor r&uuml;ckw&auml;rts dreht.
	 */
	Motor(volatile port_t & port, const port_t enableMask, const port_t forwardMask, const port_t backwardMask);

	/**	Stoppt alle Motoren.
	*/
	static void stopAll();
	
	/** Setzt die Geschwindigkeit des Motors.
	 *	\param s	Geschwindigkeit (-127 bis 127)
	 */
	void setSpeed(int8_t s);
	
	/** Gibt die Geschwindigkeit des Motors zur&uuml;ck.
	*	\return	Geschwindigkeit (-127 bis 127)
	*/
	int8_t getSpeed()
	{
		return speed;
	}
	
	/**	Setzt den Kalibrierungsfaktor f&uuml;r den Motor.
	 *	\param factorDiv127	Kalibrieungsfaktor ergibt sich durch factorDiv127 / 127 (Kalibrierung durch Verlangsamung eines Motors).
	 */
	void setCalibrationFactor(int8_t factorDiv127)
	{
		calibrationFactor = factorDiv127;
		speedChanged();
	}
	
	/** Gibt den Kalibrierungsfaktor des Motors zur&uuml;ck.
	*	\return	Kalibrierungsfaktor (-127 bis 127)
	*/
	int8_t getCalibrationFactor()
	{
		return calibrationFactor;
	}

};



