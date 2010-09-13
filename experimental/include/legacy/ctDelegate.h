#pragma once

namespace halib_ext
{
namespace object
{

/**\brief Compile-time delegate selector
 * \ingroup object
 * \tparam type the type of delegate you want to use
 *
 * This selector can be used as an unified interface for the different
 * compile-time delegates.  The selected delegate is accessible as the subtype
 * Setup.
 **/
template<typename P>
struct CTDelegate;

/**\brief Compile-time delegate for const member functions
 * \ingroup object
 **/
template<typename P>
struct CTDelegate
{
	/**\brief delegate implementation for const member functions on const objects
	 * \tparam T the object type whose member should be called
	 * \tparam P the parameter type of the member function
	 * \tparam R the return type of the member function
	 * \tparam Fxn the member function, which should be called
	 **/
	template<typename T, void (T::*Fxn)(P) const>
	struct ConstBind
	{
		private:
			/**\brief internal reference to the instance of type T**/
			const T *t;
		public:
			ConstBind() : t(NULL){}
			/**\brief set the reference to the instance of type T
		 	* \param t the const instance of type T
		 	**/
			void bind(const T& t)
			{
				this->t=&t;
			}

			/**\brief Call the selected const member function
		 	* \param param the parameter of the call
		 	* \return the return value of the call
		 	**/
			void operator()(P param) const
			{
				if(t)
					return (t->*Fxn)(param);
			}
	};

	/**\brief delegate implementation for const member functions on const objects
	 * \tparam T the object type whose member should be called
	 * \tparam P the parameter type of the member function
	 * \tparam R the return type of the member function
	 * \tparam Fxn the member function, which should be called
	 **/
	template<typename T, void (T::*Fxn)(P)>
	struct Bind
	{
		private:
			/**\brief internal reference to the instance of type T**/
			T *t;
		public:

			Bind() : t(NULL){}
			/**\brief Constructor initialiasing the reference to the instance of type T
		 	* \param t the instance of type T
		 	**/
			void bind(T& t)
			{
				this->t=&t;
			}

			/**\brief Call the selected member function
		 	* \param param the parameter of the call
		 	* \return the return value of the call
		 	**/
			void operator()(P param)
			{
				if(t)
					return (t->*Fxn)(param);
			}
	};

	/**\brief delegate implementation for static or non-member functions
	 * \tparam P the parameter type of the function
	 * \tparam R the return type of the function
	 * \tparam Fxn the member function, which should be called
	 **/
	template<void (*Fxn)(P)>
	struct StaticBind
	{
			/**\brief Call the selected const member function
		 	* \param param the parameter of the call
		 	* \return the return value of the call
		 	**/
			void operator()(P param)
			{
				return (*Fxn)(param);
			}
	};
};
}
}

/**\example ctDelegate.cpp
 *
 * This example shows the usage of the 3 different types of compile-time
 * delegates.  Each delegate is used with an apropriate structure, that contain
 * similar functionality.  To view the resource usage, the Logging framework is
 * used, so output can be disabled.
 **/
