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
			while(rm.ocrub)
				SyncRegMap(rm);
		}
	};

	
	template<typename RegMap>
	struct OCUAsyncHelper<RegMap, true, 3>
	{
		void syncOCUs()
		{
			UseRegMap(rm, RegMap);
			while(rm.ocruba | rm.ocrubb | rm.ocrubc)
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
				unitA
			};

		public:

			OutputCompareUnit()
			{
				UseRegMap(rm, RegMap);
				rm.ocie=config::ocmAInt;
				rm.com=config::ocmAMode;
			}

			template<OutputCompareUnits unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);
				rm.ocr=value;
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<OutputCompareUnits unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.ocr;
			}
			
			template<OutputCompareUnits unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);
				rm.ocie=value;
			}
			
			template<OutputCompareUnits unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.ocie;
			}

			template<OutputCompareUnits unit>
			void setCompareMatchMode(CompareMatchModes newCMMode)
			{
				UseRegMap(rm, RegMap);
				rm.com=newCMMode;
				SyncRegMap(rm);
				this->sync();
			}

			template<OutputCompareUnits unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.com;
			}

			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
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
			}
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
				unitA=0,
				unitB,
				unitC
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
					case(unitA): rm.ocra=value;
								 break;
					case(unitB): rm.ocrb=value;
								 break;
					case(unitC): rm.ocrc=value;
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
					case(unitA): return rm.ocra;
					case(unitB): return rm.ocrb;
					case(unitB): return rm.ocrc;
				}
			}
			
			template<OutputCompareUnits unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(unitA): rm.ociea=value;
								 break;
					case(unitB): rm.ocieb=value;
								 break;
					case(unitC): rm.ociec=value;
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
					case(unitA): return rm.ociea;
					case(unitB): return rm.ocieb;
					case(unitC): return rm.ociec;
				}
			}

			template<OutputCompareUnits unit>
			void setCompareMatchMode(CompareMatchModes value)
			{
				UseRegMap(rm, RegMap);
				switch(unit)
				{
					case(unitA): rm.coma=value;
								 break;
					case(unitB): rm.comb=value;
								 break;
					case(unitC): rm.comc=value;
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
					case(unitA): return rm.coma;
					case(unitB): return rm.comb;
					case(unitC): return rm.comc;
				}
			}

			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(T& obj)
			{
				switch(unit)
				{
					case(unitA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, T, F>(obj);
									break;
					case(unitB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, T, F>(obj);
									break;
					case(unitC):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchC_Int, T, F>(obj);
									break;
				}
			}

			template<OutputCompareUnits unit, typename T, void (T::*F)(void)>
			void registerCompareMatchCallback(const T& obj)
			{
				switch(unit)
				{
					case(unitA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, T, F>(obj);
									break;
					case(unitB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, T, F>(obj);
									break;
					case(unitC):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchC_Int, T, F>(obj);
									break;
				}
			}

			template<OutputCompareUnits unit, void (*F)(void)>
			void registerCompareMatchCallback()
			{
				switch(unit)
				{
					case(unitA):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchA_Int, F>();
									break;
					case(unitB):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchB_Int, F>();
									break;
					case(unitC):	interrupts::Interrupt<IntMap>::template setInt<IntMap::compareMatchC_Int, F>();
									break;
				}
			}
	};
}
}
}
