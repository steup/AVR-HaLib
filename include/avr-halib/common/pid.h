#pragma once

#include <avr-halib/common/fixPoint.h>

/** AVR-HaLib */
namespace avr_halib
{
/** AVR-HaLib object */
namespace object
{
    /** \brief PIDControl
     *
     * \tparam config PIDControl configuration
     */
    template<typename config>
    class PIDControl
    {
        public:
            /** \brief TODO \todo */
            typedef typename config::BaseType InputType;
            /** \brief TODO \todo */
            static const InputType kp=config::kp;
            /** \brief TODO \todo */
            static const InputType kd=config::kd;
            /** \brief TODO \todo */
            static const InputType ki=config::ki;
            /** \brief TODO \todo */
            static const InputType min=config::min;
            /** \brief TODO \todo */
            static const InputType max=config::max;

            /** \brief TODO \todo */
            typedef FixPointValue<typename config::BaseType, config::exponent> CalcType;

        private:
            /** \brief TODO \todo */
            static const uint8_t windowSize=8;
            /** \brief TODO \todo */
            CalcType e[windowSize];
            /** \brief TODO \todo */
            uint8_t index;
            /** \brief TODO \todo */
            CalcType sumE;
            /** \brief TODO \todo */
            CalcType targetValue;
            /** \brief TODO \todo */
            CalcType currentValue;

        public:
            /** \brief TODO \todo */
            PIDControl() : index(0), sumE(0), targetValue(0), currentValue(0)
            {
                for(uint8_t i=0;i<windowSize;i++)
                    e[i]=0;
            }

            /** \brief TODO \todo
             *
             * \param target TODO \todo
             */
            void target(InputType target)
            {
                this->targetValue=CalcType(1)*target;
            }

            /** \brief TODO \todo */
            InputType target() const
            {
                return targetValue.trunc();
            }

            /** \brief TODO \todo */
            const CalcType& current() const
            {
                return currentValue;
            }

            /** \brief TODO \todo
             *
             * \param input TODO \todo
             */
            InputType control(InputType input)
            {
                CalcType calcKp=CalcType::epsilon()*this->kp;
                CalcType calcKi=CalcType::epsilon()*this->ki;
                CalcType calcKd=CalcType::epsilon()*this->kd;
                CalcType newE=targetValue-input;

                CalcType oldE=e[index];

                CalcType dE= newE - oldE;

                if(index >= windowSize)
                    index=0;
                else
                    index++;

                sumE+=newE-e[index];

                e[index]=newE;

                currentValue+=(newE * calcKp + dE * calcKd + sumE * calcKi / windowSize);

                if(currentValue > CalcType(max))
                    currentValue = CalcType(max);
                if(currentValue < CalcType(min))
                    currentValue = CalcType(min);

                return currentValue.trunc();
            }
    };
}
}
