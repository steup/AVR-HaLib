#pragma once

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
    /** \brief TODO \todo
    *
    * \tparam config Configuration
    * \tparam BaseDriver TODO \todo
    */
    template<typename config, template<typename> class BaseDriver>
    struct DynamicPrescaler : public BaseDriver<config>
    {
        private:
            /** \brief TODO \todo */
            typedef typename config::RegMap RegMap;
            /** \brief TODO \todo */
            typedef typename RegMap::Prescalers Prescalers;
            /** \brief TODO \todo */
            Prescalers ps;

        public:
            /** \brief TODO \todo */
            DynamicPrescaler()
            {
                ps=config::ps;
            }

            /** \brief TODO \todo
             *
             * \param newPs TODO \todo
             */
            void setPrescaler(Prescalers newPs)
            {
                ps=newPs;
                start();
            }

            /** \brief TODO \todo */
            Prescalers getPrescaler() const
            {
                return ps;
            }

            /** \brief TODO \todo */
            void start()
            {
                UseRegMap(rm, RegMap);
                rm.cs=ps;
                SyncRegMap(rm);
                this->sync();
            }
    };
}
}
}
}
