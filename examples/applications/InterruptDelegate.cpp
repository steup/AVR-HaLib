#include <stdio.h>


class delegate
{
    typedef void (*invoke_stub)(void const *);
    void const  *obj_ptr_;
    invoke_stub stub_ptr_;

    template<typename T, void (T::*Fxn)()>
    struct mem_fn_stub
    {
        static void invoke(void const * obj_ptr)
        {
            T * obj = static_cast<T *>( const_cast<void *>( obj_ptr ) );
            (obj->*Fxn)();
        }
    };

    template<typename T, void (T::*Fxn)() const>
    struct mem_fn_const_stub
    {
        static void invoke(void const * obj_ptr)
        {
            T const * obj = static_cast<T const *>( obj_ptr );
            (obj->*Fxn)();
        }
    };

    template<void (*Fxn)()>
    struct function_stub
    {
        static void invoke(void const *)
        {
            (*Fxn)( );
        }
    };

public:
    delegate() : obj_ptr_( 0 ), stub_ptr_( 0 ) { }

    template<typename T, void (T::*Fxn)()>
    void from_function(T * obj)
    {
        obj_ptr_ = const_cast<T const *>( obj );
        stub_ptr_ = &mem_fn_stub<T, Fxn>::invoke;
    }

    template<typename T, void (T::*Fxn)() const>
    void from_function(T const * obj)
    {
        obj_ptr_ = obj;
        stub_ptr_ = &mem_fn_const_stub<T, Fxn>::invoke;
    }

    template<void (*Fxn)()>
    void from_function()
    {
        obj_ptr_ = 0;
        stub_ptr_ = &function_stub<Fxn>::invoke;
    }

    void operator ()() const
    {
        ( *stub_ptr_ )( obj_ptr_);
    }
};

//template <class T>
struct foobar
{
    void foo() { printf("%s\n",__PRETTY_FUNCTION__);}
    void bar() const { printf("%s\n",__PRETTY_FUNCTION__);}
};

void hello() { printf("%s\n",__PRETTY_FUNCTION__);}

delegate dg, &d=dg;

int main()
{
    foobar fb;
    foobar *pfb = &fb;


    dg.from_function<foobar, &foobar::foo>( pfb );
    dg(); // (pfb->*&foobar::foo)( 1 );


    dg.from_function<foobar , &foobar::bar>( pfb );
    d(); // (pfb->*&foobar::bar)( 1 );


    dg.from_function<&hello>();
    d(); // hello( 1 );

return 0;
}
