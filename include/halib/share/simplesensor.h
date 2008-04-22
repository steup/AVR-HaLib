template <class ReturnType,class Sensor> SimpleSensor:public Sensor<class ReturnType>
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