#ifndef __InterruptDelegate_h__
#define __InterruptDelegate_h__

#define UseInterrupt(X)	      __UseInterrupt(X)
#define __UseInterrupt(X)						\
  class X {								\
  public:								\
    typedef void (*invoke_stub)();					\
    static void const  *obj_ptr;					\
    static invoke_stub stub_ptr;					\
									\
    template<void (*Fxn)()>						\
      void from_function()						\
      {									\
	obj_ptr = 0;							\
	stub_ptr = Fxn;							\
      }									\
									\
    template<typename T, void (T::*Fxn)()>				\
      struct mem_fn_stub						\
      {									\
	static void invoke()						\
	{								\
	  T * obj = static_cast<T *>( const_cast<void *>(obj_ptr ) );	\
	  (obj->*Fxn)();						\
	}								\
      };								\
									\
    template<typename T, void (T::*Fxn)() const>			\
      struct mem_fn_const_stub						\
      {									\
	static void invoke()						\
	{								\
	  T const * obj = static_cast<T const *>(obj_ptr );		\
	  (obj->*Fxn)();						\
	}								\
      };								\
									\
    template<typename T, void (T::*Fxn)()>				\
      void from_function(T * obj)					\
      {									\
	obj_ptr = const_cast<T const *>(obj );				\
	stub_ptr = &mem_fn_stub<T, Fxn>::invoke;			\
      }									\
									\
    template<typename T, void (T::*Fxn)() const>			\
      void from_function(T const * obj)					\
      {									\
	obj_ptr = obj;							\
	stub_ptr = &mem_fn_const_stub<T, Fxn>::invoke;			\
      }									\
									\
    inline void __call() {						\
      (*stub_ptr)();							\
    }									\
  }

#define STORE(X)				\
  void const  *X::obj_ptr;			\
  void (*X::stub_ptr)();

#define redirectISRMF(vector,ObjType,func, obj) \
	__redirectISRMF(vector,ObjType,func, obj)
#define __redirectISRMF(vector,ObjType,func, obj)	\
  do {							\
    vector m;						\
    m.from_function<ObjType, func>(obj);		\
  } while(0)

#define redirectISRF(vector,func)        __redirectISRF(vector,func)
#define __redirectISRF(vector,func)		\
  do {						\
    vector m;					\
    m.from_function<func>();			\
  } while(0)

#define call(vector)				\
  do {						\
    vector m;					\
    m.__call();					\
  } while(0)

#define GenInterrupt(X)				\
  UseInterrupt(X);				\
  STORE(X)

#endif /*__InterruptDelegate_h__*/
