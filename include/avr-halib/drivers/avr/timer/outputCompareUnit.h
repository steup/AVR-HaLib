#pragma once

#include <avr-halib/regmaps/regmaps.h>

/** AVR-HaLib */
namespace avr_halib
{
/** Drivers */
namespace drivers
{
/** AVR-Drivers */
namespace avr
{
/** Timer */
namespace timer
{
    /** Timer-Helpers */
    namespace helpers
    {
        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam async TODO \todo
         * \tparam num TODO \todo
         */
        template<typename RegMap, bool async, unsigned int num>
        struct OCUAsyncHelper;

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         * \tparam num TODO \todo
         */
        template<typename RegMap, unsigned int num>
        struct OCUAsyncHelper<RegMap, false, num>
        {
            /** \brief TODO \todo */
            void syncOCUs(){}
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         */
        template<typename RegMap>
        struct OCUAsyncHelper<RegMap, true, 1>
        {
            /** \brief TODO \todo */
            void syncOCUs()
            {
                UseRegMap(rm, RegMap);
                while(rm.ocraub)
                    SyncRegMap(rm);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         */
        template<typename RegMap>
        struct OCUAsyncHelper<RegMap, true, 2>
        {
            /** \brief TODO \todo */
            void syncOCUs()
            {
                UseRegMap(rm, RegMap);
                while(rm.ocraub | rm.ocrbub)
                    SyncRegMap(rm);
            }
        };

        /** \brief TODO \todo
         *
         * \tparam RegMap TODO \todo
         */
        template<typename RegMap>
        struct OCUAsyncHelper<RegMap, true, 3>
        {
            /** \brief TODO \todo */
            void syncOCUs()
            {
                UseRegMap(rm, RegMap);
                while(rm.ocraub | rm.ocrbub | rm.ocrcub)
                    SyncRegMap(rm);
            }
        };
    }

    /** \brief TODO \todo
     *
     * \tparam config Configuration
     * \tparam async TODO \todo
     * \tparam num TODO \todo
     */
    template<typename config, bool async, unsigned int num>
    struct OutputCompareUnit;

    /** \brief TODO \todo
     *
     * \tparam config Configuration
     * \tparam async TODO \todo
     */
    template<typename config, bool async>
    struct OutputCompareUnit<config, async, 1> :
        protected helpers::OCUAsyncHelper<typename config::RegMap,
                                          async, 1>
    {
        private:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;
            /** \brief TODO \todo */
            typedef typename RegMap::ValueType ValueType;
            /** \brief TODO \todo */
            typedef typename RegMap::CompareMatchModes CompareMatchModes;

        public:
            /** \brief TODO \todo */
            struct Units
            {
                /** \brief TODO \todo */
                enum UnitType
                {
                    matchA = 0
                };
            };
            /** \brief TODO \todo */
            typedef typename Units::UnitType UnitType;

        public:
            /** \brief TODO \todo */
            OutputCompareUnit()
            {
                UseRegMap(rm, RegMap);
                rm.ociea=config::ocmAInt;
                rm.coma=config::ocmAMode;
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
            template<UnitType unit>
            void setOutputCompareValue(ValueType value)
            {
                UseRegMap(rm, RegMap);
                rm.ocra=value;
                SyncRegMap(rm);
                this->syncOCUs();
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
            template<UnitType unit>
            ValueType getOutputCompareValue()
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.ocra;
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
            template<UnitType unit>
            void setOutputCompareInterrupt(bool value)
            {
                UseRegMap(rm, RegMap);
                rm.ociea=value;
                this->syncOCUs();
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
            template<UnitType unit>
            bool getOutputCompareInterrupt()
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.ociea;
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param newCMMode TODO \todo
             */
            template<UnitType unit>
            void setCompareMatchMode(CompareMatchModes newCMMode)
            {
                UseRegMap(rm, RegMap);
                rm.coma=newCMMode;
                SyncRegMap(rm);
                this->syncOCUs();
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
            template<UnitType unit>
            CompareMatchModes setCompareMatchMode() const
            {
                UseRegMap(rm, RegMap);
                SyncRegMap(rm);
                return rm.coma;
            }
    };

    /** \brief TODO \todo
     *
     * \tparam config TODO \todo
     * \tparam async TODO \todo
     */
    template<typename config, bool async>
    struct OutputCompareUnit<config, async, 2> :
        protected helpers::OCUAsyncHelper<typename config::RegMap,
                                          async, 2>
    {
        private:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;
            /** \brief TODO \todo */
            typedef typename RegMap::ValueType ValueType;
            /** \brief TODO \todo */
            typedef typename RegMap::CompareMatchModes CompareMatchModes;

        public:
            /** \brief TODO \todo */
            struct Units
            {
                /** \brief TODO \todo */
                enum UnitType
                {
                    matchA=0,
                    matchB
                };
            };
            /** \brief TODO \todo */
            typedef typename Units::UnitType UnitType;

        public:
            /** \brief TODO \todo */
            OutputCompareUnit()
            {
                UseRegMap(rm, RegMap);
                rm.ociea=config::ocmAInt;
                rm.ocieb=config::ocmBInt;
                rm.coma=config::ocmAMode;
                rm.comb=config::ocmBMode;
            }

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
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

            /** \brief TODO \todo
            *
            * \tparam unit TODO \todo
            */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
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

    /** \brief TODO \todo
     *
     * \tparam config TODO \todo
     * \tparam async TODO \todo
     */
    template<typename config, bool async>
    struct OutputCompareUnit<config, async, 3> :
        protected helpers::OCUAsyncHelper<typename config::RegMap,
                                          async, 3>
    {
        private:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;
            /** \brief TODO \todo */
            typedef typename RegMap::ValueType ValueType;
            /** \brief TODO \todo */
            typedef typename RegMap::CompareMatchModes CompareMatchModes;

        public:
            /** \brief TODO \todo */
            struct Units
            {
                /** \brief TODO \todo */
                enum UnitType
                {
                    matchA=0,
                    matchB,
                    matchC
                };
            };
            /** \brief TODO \todo */
            typedef typename Units::UnitType UnitType;

        public:
            /** \brief TODO \todo */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             * \param value TODO \todo
             */
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

            /** \brief TODO \todo
             *
             * \tparam unit TODO \todo
             */
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
}
