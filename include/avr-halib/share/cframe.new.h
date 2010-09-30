/** \addtogroup share */
/*@{*/
/**
 *	\file	avr-halib/share/cframe.new.h
 *	\brief	Defines CFrame
 *	\author	Karl Fessel
 *	\see 	doc_cdevices
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
 */


/*! \brief Modifiers of the <code>CFrame</code>*/

struct CFrameModifierBase
{
	enum
	{
		none = 0x00   /*NUL*/,
		xoff = 0x13   /*DC3 ^S*/,
		xon  = 0x11   /*DC1 ^Q*/
	};
	enum{useNoneChar = false, useXonXoff=false};
};

struct CFrameOldReadable:public CFrameModifierBase
{
	enum {esc = 'e', sofr = 'a', eofr = 's', escmod = 0x01};
};

struct CFrameReadable:public CFrameModifierBase
{
	enum {esc = 'e', sofr = 'a', eofr = 's', escmod = 0x20};
};

struct CFrameAscii:public CFrameModifierBase
{
	enum
	{
		esc  = 0x1b  /*Escape '/e'   ^[ */,
		sofr = 0x02  /*Start of Text ^A */,
		eofr = 0x03  /*End of Text   ^C */,
		escmod = 0x20 
	};
};



template < class stuffingbytes = struct CFrameReadable > class CFrame
{
	
	typedef stuffingbytes conf;
	typedef CFrame<stuffingbytes> thistype;
	
	enum state_t {
		//using only lower halfbyte
		invalid  = 0x00,
		started  = 0x01,
		regular  = 0x02,
		stuff    = 0x04,
		fine = 0x08
	};
	
	struct
	{
		state_t tx:4;
		state_t rx:4;
	} state;  /*!< state of Machine*/
	
	public:
		
		CFrame()
		{
			reset();
		}
		
		inline void reset()
		{
			resetTx();
			resetRx();
			
		}
		
		inline void resetTx()
		{
			state.tx = invalid;
		}
		
		inline void resetRx()
		{
			state.rx = invalid;
		}
		
		/**
		Usage:
		out << startFrame();
		//next line depends on your datastorage 
		//eg.: char c; for(int i = 0,c = data[i]; i < size; i++, c = data[i])
		//or   for(char c = *str; c != 0; str++ , c = *str)
		for char c in datalist  
		{
			do {
				out << transformOut(c)
			}while(again());
		}
		out << endFrame()
		*/
		
		inline bool sending()
		{
			return (state.tx != invalid && state.tx != fine);
		}
		
		inline bool readyToStart()
		{
			return (state.tx == invalid);
		}
		
		inline char startFrame()
		{
			if (state.tx == invalid) state.tx = started;
			return conf::sofr;
		}
		
		inline char endFrame()
		{
			if (state.tx == regular) state.tx = fine;
			return conf::eofr;
		}
		
		inline bool tx_complete()
		{
			return (state.tx == fine);
		}
		
		inline bool again()
		{
			return (state.tx != regular );
		}
		
		inline char noneChar()
		{
			return conf::none;
		}
		
		inline char xonChar()
		{
			return conf::xon;
		}
		
		inline char xoffChar()
		{
			return conf::xoff;
		}
		
		
		inline char transformOut(char c)
		{
			if ( state.tx == started ) state.tx = regular;
			
			if ( state.tx == regular )
			{
				if( c == conf::eofr || c == conf::sofr || c == conf::esc
					||( conf::useNoneChar && c == conf::none)
					||( conf::useXonXoff  )&&( c == conf::xon || c == conf::xoff ) )
				{
					c = conf::esc;
					state.tx = stuff;
				}else
				{
					c = c;
				}
			} else if( state.tx == stuff )
			{
				c = (conf::escmod!=0)?(c^conf::escmod):c;
				state.tx = regular;
			}
			return c;
		}
		
		/**
		for(char * buffer;;)
		{
			if(buffer.full()) buffer.handlefull();
			char c;
			if(transformIn(c))
			{
				buffer << c;
			}else if ( restarted() )
			{
				buffer.reset();
			}else if ( finished() )
			{
				buffer.save();
				break;
			}
		}
		*/
		
		
		inline bool receiving()
		{
			return (state.rx != invalid);
		}
		
		inline bool restarted()
		{	
			return (state.rx == started);
		}
		
		inline bool finished()
		{
			return (state.rx == fine);
		}
		
		inline bool transformIn(char & c)
		{
			if ( state.rx == started ) state.rx = regular;
			if ( state.rx == invalid )
			{
				if ( c == conf::sofr )
				{
					state.rx = started;
				}
				
			} else if ( state.rx == regular )
			{
				if ( c == conf::eofr )
				{
					//this frame is complete
					state.rx = fine;
				}
				else if ( c == conf::sofr )
				{
					//may be missed previous eofr start an other frame
					state.rx = started;
				}else if ( c == conf::esc )
				{
					// next character was stuffed
					state.rx = stuff;
				}else if ( ( conf::useNoneChar ) && ( c == conf::none ))
				{
					return false;
				}else if ( ( conf::useXonXoff ) && ( c == conf::xon ))
				{
					return false;
				}else if ( ( conf::useXonXoff ) && ( c == conf::xoff ))
				{
					return false;
				}
				else
				{
					// any non control character
					c = c;
					state.rx = regular;
				}
				
			} else if( state.rx == stuff )
			{
				c = ( conf::escmod != 0x00 )?( c^conf::escmod ): c;
				state.rx = regular;
				
			}
			return (state.rx == regular);
		}
		
		
};
/*@}*/
