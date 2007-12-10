
template <class length_t, length_t bufLen> class CButton : public CDev
{
protected:
	Buffer<char, length_t, bufLen> buffer;
	ButtonBlock buttonb;

public:
	CButton(ButtonBlock & buttons) : buttonb(buttons){}


	/**
	 * putc ber nimmt ein Zeichen als Parameter 
	 */
	void putc (const char c){}
		
	/**
	 * getc gibt wenn noch vorhanden ein Zeichen 
	 */ 
	char getc(){
		return buffer.get();
	}
	void step(){
  	buffer.put(buttonb.isPressedPattern());
	}
};