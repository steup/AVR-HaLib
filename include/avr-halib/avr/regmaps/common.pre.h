
enum _offset{};
#if defined CPU_FREQUENCY
class DefineController
{
	public:
		enum
	{
		controllerClk=CPU_FREQUENCY
	};

};
#elif defined F_CPU
class DefineController
{
	public:
		enum
	{
		controllerClk=F_CPU
	};

};
#else
class DefineController
{
	public:
		enum
	{
		controllerClk=8000000 //atmel internal rc default
	};

};
#endif
