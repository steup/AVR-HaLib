#pragma once

#include <avr-halib/regmaps/regmaps.h>

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
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;

		public:

            struct Units
            {
			    enum UnitType
                {
                    matchA = 0
                };
            };
            typedef typename Units::UnitType UnitType;

		public:

			OutputCompareUnit()
			{
				UseRegMap(rm, RegMap);
				rm.ociea=config::ocmAInt;
				rm.coma=config::ocmAMode;
			}

			template<UnitType unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);
				rm.ocra=value;
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<UnitType unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.ocra;
			}
			
			template<UnitType unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);
				rm.ociea=value;
                this->syncOCUs();
			}
			
			template<UnitType unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.ociea;
			}

			template<UnitType unit>
			void setCompareMatchMode(CompareMatchModes newCMMode)
			{
				UseRegMap(rm, RegMap);
				rm.coma=newCMMode;
				SyncRegMap(rm);
				this->syncOCUs();
			}

			template<UnitType unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				return rm.coma;
			}
	};

	template<typename config, bool async>
	struct OutputCompareUnit<config, async, 2> : 
		protected helpers::OCUAsyncHelper<typename config::RegMap, 
										  async, 2>
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;

		public:

            struct Units
            {
                enum UnitType
                {
                    matchA=0,
                    matchB
                };
            };
            typedef typename Units::UnitType UnitType;

		public:

			OutputCompareUnit()
			{
				UseRegMap(rm, RegMap);
				rm.ociea=config::ocmAInt;
				rm.ocieb=config::ocmBInt;
				rm.coma=config::ocmAMode;
				rm.comb=config::ocmBMode;
			}

			template<UnitType unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(Units::matchA): rm.ocra=value;
								 break;
					case(Units::matchB): rm.ocrb=value;
								 break;
				}
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<UnitType unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(Units::matchA): return rm.ocra;
					case(Units::matchB): return rm.ocrb;
				}
			}
			
			template<UnitType unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(Units::matchA): rm.ociea=value;
								 break;
					case(Units::matchB): rm.ocieb=value;
								 break;
				}
			}
			
			template<UnitType unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(Units::matchA): return rm.ociea;
					case(Units::matchB): return rm.ocieb;
				}
			}

			template<UnitType unit>
			void setCompareMatchMode(CompareMatchModes value)
			{
				UseRegMap(rm, RegMap);
				switch(unit)
				{
					case(Units::matchA): rm.coma=value;
								 break;
					case(Units::matchB): rm.comb=value;
								 break;
				}
				SyncRegMap(rm);
				this->sync();
			}

			template<UnitType unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(Units::matchA): return rm.coma;
					case(Units::matchB): return rm.comb;
				}
			}
	};

	template<typename config, bool async>
	struct OutputCompareUnit<config, async, 3> : 
		protected helpers::OCUAsyncHelper<typename config::RegMap, 
										  async, 3>
	{
		private:
			typedef typename config::RegMap RegMap;
			typedef typename RegMap::ValueType ValueType;
			typedef typename RegMap::CompareMatchModes CompareMatchModes;

		public:

            struct Units
            {
                enum UnitType
                {
                    matchA=0,
                    matchB,
                    matchC
                };
            };
            typedef typename Units::UnitType UnitType;

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

			template<UnitType unit>
			void setOutputCompareValue(ValueType value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(Units::matchA): rm.ocra=value;
								 break;
					case(Units::matchB): rm.ocrb=value;
								 break;
					case(Units::matchC): rm.ocrc=value;
								 break;
				}
				SyncRegMap(rm);
				this->syncOCUs();
			}


			template<UnitType unit>
			ValueType getOutputCompareValue()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(Units::matchA): return rm.ocra;
					case(Units::matchB): return rm.ocrb;
					case(Units::matchB): return rm.ocrc;
				}
			}
			
			template<UnitType unit>
			void setOutputCompareInterrupt(bool value)
			{
				UseRegMap(rm, RegMap);

				switch(unit)
				{
					case(Units::matchA): rm.ociea=value;
								 break;
					case(Units::matchB): rm.ocieb=value;
								 break;
					case(Units::matchC): rm.ociec=value;
								 break;
				}
			}
			
			template<UnitType unit>
			bool getOutputCompareInterrupt()
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(Units::matchA): return rm.ociea;
					case(Units::matchB): return rm.ocieb;
					case(Units::matchC): return rm.ociec;
				}
			}

			template<UnitType unit>
			void setCompareMatchMode(CompareMatchModes value)
			{
				UseRegMap(rm, RegMap);
				switch(unit)
				{
					case(Units::matchA): rm.coma=value;
								 break;
					case(Units::matchB): rm.comb=value;
								 break;
					case(Units::matchC): rm.comc=value;
								 break;
				}
				SyncRegMap(rm);
				this->sync();
			}

			template<UnitType unit>
			CompareMatchModes setCompareMatchMode() const
			{
				UseRegMap(rm, RegMap);
				SyncRegMap(rm);
				switch(unit)
				{
					case(Units::matchA): return rm.coma;
					case(Units::matchB): return rm.comb;
					case(Units::matchC): return rm.comc;
				}
			}
	};
}
}
}
