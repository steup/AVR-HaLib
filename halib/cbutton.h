
template <class length_t, length_t bufLen> class CButton : public CDev
{
protected:
	Buffer<char, length_t, bufLen> buffer;
	ButtonBlock buttons;

public:
	CButton() : buttons(PINA, PORTA, DDRA, _BV(4), 0){}


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
  	buffer.put(buttons.isPressedPattern());
	}
};