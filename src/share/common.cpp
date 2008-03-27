




/**
 *	Needed in case a pure virtual method method is called (what shouldn't happen, but the compiler and linker want it)
 */
extern "C" {

void __cxa_pure_virtual()
{
}

}

