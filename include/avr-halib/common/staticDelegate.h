#pragma once

namespace Interrupt{
    template<uint16_t, typename>
    struct Slot;
}

namespace avr_halib{
namespace object
{
    template<typename Parameter>
    struct StaticDelegate
    {
        template<void (*f)(Parameter)>
        struct bind
        {
            private:
                static void invoke(Parameter p)
                {
                    (*f)(p);
                }

            public:
                void operator()(Parameter p)
                {
                    invoke(p);
                }
           template<uint16_t, typename> friend class Interrupt::Slot;
        };
    };

    template<>
    struct StaticDelegate<void>
    {
        template<void (*f)(void)>
        struct bind
        {
            private:
                static void invoke(void)
                {
                    (*f)();
                }
            public:
                void operator()(void)
                {
                    invoke();
                }
            template<uint16_t, typename> friend class Interrupt::Slot;
        };
    };
}
}
