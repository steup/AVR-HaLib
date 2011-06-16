#pragma once

namespace avr_halib
{
namespace drivers
{
namespace timer
{
namespace helpers
{
	template<typename RegMap, bool async, unsigned int num>
	struct OCUAsyncHelper;

	template<typename RegMap, unsigned int num>
	struct OCUAsyncHelper<RegMap, false, num>
	{
		void syncOCUs(){}
	};

	template<typename RegMap>
	struct OCUAsyncHelper<RegMap, true, 1>
	{
		void syncOCUs()
		{
			UseRegMap(rm, RegMap);
			while(rm.ocraub)
				SyncRegMap(rm);
		}
	};

	template<typename RegMap>
	struct OCUAsyncHelper<RegMap, true, 2>
	{
		void syncOCUs()
		{
			UseRegMap(rm, RegMap);
			while(rm.ocraub | rm.ocrbub)
				SyncRegMap(rm);
		}
	};

	template<typename RegMap>
	struct OCUAsyncHelper<RegMap, true, 3>
	{
		void syncOCUs()
		{
			UseRegMap(rm, RegMap);
			while(rm.ocraub | rm.ocrbub | rm.ocrcub)
				SyncRegMap(rm);
		}
	};
}

	template<typename config, bool async, unsigned int num>
	struct OutputCompareUnit;

	template<typename config, bool async>
	struct OutputCompareUnit<config, async, 1> : 
		protected helpers::OCUAsyncHelper<typename config::RegMap, 
										  async, 1>
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::IntMap IntMap;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;

		public:

			enum OutputCompareUnits
			{
				matchA
			};

		public:

			OutputCompareUnit()
			{
				UseRegMap(rm, RegMap);
				rm.ociea=config::ocmAInt;
				rm.coma=config::ocmAMode;
			}

			template<OutputCompareUnits unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);
				rm.ocra=value;
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<OutputCompareUnits unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.ocra;
			}
			
			template<OutputCompareUnits unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);
				rm.ociea=value;
			}
			
			template<OutputCompareUnits unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.ociea;
			}

			template<OutputCompareUnits unit>
			void setCompareMatchMode(CompareMatchModes newCMMode)
			{
				UseRegMap(rm, RegMap);
				rm.coma=newCMMode;
				SyncRegMap(rm);
				this->sync();
			}

			template<OutputCompareUnits unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.coma;
			}

/*			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(T& obj)
			{
				interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatch_Int, T, F>(obj);
			}

			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(const T& obj)
			{
				interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatch_Int, T, F>(obj);
			}

			template<OutputCompareUnits unit, void (*F)(void)>
			void registerCompareMatchCallback()
			{
				interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatch_Int, F>();
			}*/
	};

	template<typename config, bool async>
	struct OutputCompareUnit<config, async, 2> : 
		protected helpers::OCUAsyncHelper<typename config::RegMap, 
										  async, 2>
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::IntMap IntMap;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;

		public:

			enum OutputCompareUnits
			{
				matchA=0,
				matchB
			};

		public:

			OutputCompareUnit()
			{
				UseRegMap(rm, RegMap);
				rm.ociea=config::ocmAInt;
				rm.ocieb=config::ocmBInt;
				rm.coma=config::ocmAMode;
				rm.comb=config::ocmBMode;
			}

			template<OutputCompareUnits unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(matchA): rm.ocra=value;
								 break;
					case(matchB): rm.ocrb=value;
								 break;
				}
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<OutputCompareUnits unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(matchA): return rm.ocra;
					case(matchB): return rm.ocrb;
				}
			}
			
			template<OutputCompareUnits unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(matchA): rm.ociea=value;
								 break;
					case(matchB): rm.ocieb=value;
								 break;
				}
			}
			
			template<OutputCompareUnits unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(matchA): return rm.ociea;
					case(matchB): return rm.ocieb;
				}
			}

			template<OutputCompareUnits unit>
			void setCompareMatchMode(CompareMatchModes value)
			{
				UseRegMap(rm, RegMap);
				switch(unit)
				{
					case(matchA): rm.coma=value;
								 break;
					case(matchB): rm.comb=value;
								 break;
				}
				SyncRegMap(rm);
				this->sync();
			}

			template<OutputCompareUnits unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(matchA): return rm.coma;
					case(matchB): return rm.comb;
				}
			}

/*			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(T& obj)
			{
				switch(unit)
				{
					case(matchA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, T, F>(obj);
									break;
					case(matchB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, T, F>(obj);
									break;
				}
			}

			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(const T& obj)
			{
				switch(unit)
				{
					case(matchA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, T, F>(obj);
									break;
					case(matchB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, T, F>(obj);
									break;
				}
			}

			template<OutputCompareUnits unit, void (*F)(void)>
			void registerCompareMatchCallback()
			{
				switch(unit)
				{
					case(matchA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, F>();
									break;
					case(matchB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, F>();
									break;
				}
			}*/
	};

	template<typename config, bool async>
	struct OutputCompareUnit<config, async, 3> : 
		protected helpers::OCUAsyncHelper<typename config::RegMap, 
										  async, 3>
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::IntMap IntMap;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;

		public:

			enum OutputCompareUnits
			{
				matchA=0,
				matchB,
				matchC
			};

		public:

			OutputCompareUnit()
			{
				UseRegMap(rm, RegMap);
				rm.ociea=config::ocmAInt;
				rm.ocieb=config::ocmBInt;
				rm.ociec=config::ocmCInt;
				rm.coma=config::ocmAMode;
				rm.comb=config::ocmBMode;
				rm.comc=config::ocmCMode;
			}

			template<OutputCompareUnits unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(matchA): rm.ocra=value;
								 break;
					case(matchB): rm.ocrb=value;
								 break;
					case(matchC): rm.ocrc=value;
								 break;
				}
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<OutputCompareUnits unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(matchA): return rm.ocra;
					case(matchB): return rm.ocrb;
					case(matchB): return rm.ocrc;
				}
			}
			
			template<OutputCompareUnits unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(matchA): rm.ociea=value;
								 break;
					case(matchB): rm.ocieb=value;
								 break;
					case(matchC): rm.ociec=value;
								 break;
				}
			}
			
			template<OutputCompareUnits unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(matchA): return rm.ociea;
					case(matchB): return rm.ocieb;
					case(matchC): return rm.ociec;
				}
			}

			template<OutputCompareUnits unit>
			void setCompareMatchMode(CompareMatchModes value)
			{
				UseRegMap(rm, RegMap);
				switch(unit)
				{
					case(matchA): rm.coma=value;
								 break;
					case(matchB): rm.comb=value;
								 break;
					case(matchC): rm.comc=value;
								 break;
				}
				SyncRegMap(rm);
				this->sync();
			}

			template<OutputCompareUnits unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(matchA): return rm.coma;
					case(matchB): return rm.comb;
					case(matchC): return rm.comc;
				}
			}

/*			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(T& obj)
			{
				switch(unit)
				{
					case(matchA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, T, F>(obj);
									break;
					case(matchB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, T, F>(obj);
									break;
					case(matchC):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchC_Int, T, F>(obj);
									break;
				}
			}

			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(const T& obj)
			{
				switch(unit)
				{
					case(matchA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, T, F>(obj);
									break;
					case(matchB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, T, F>(obj);
									break;
					case(matchC):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchC_Int, T, F>(obj);
									break;
				}
			}

			template<OutputCompareUnits unit, void (*F)(void)>
			void registerCompareMatchCallback()
			{
				switch(unit)
				{
					case(matchA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, F>();
									break;
					case(matchB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, F>();
									break;
					case(matchC):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchC_Int, F>();
									break;
				}
			}*/
	};
}
}
}
