/** \brief Modifiers of the CFrame */
struct CFrameModifierBase
{
    /** \brief TODO \todo */
    enum
    {
        none = 0x00 /*NUL*/,
        xoff = 0x13 /*DC3 ^S*/,
        xon  = 0x11 /*DC1 ^Q*/
    };
    /** \brief TODO \todo */
    enum{useNoneChar = false, useXonXoff=false};
};

/** \brief TODO \todo */
struct CFrameOldReadable:public CFrameModifierBase
{
    /** \brief TODO \todo */
    enum {esc = 'e', sofr = 'a', eofr = 's', escmod = 0x01};
};

/** \brief TODO \todo */
struct CFrameReadable:public CFrameModifierBase
{
    /** \brief TODO \todo */
    enum {esc = 'e', sofr = 'a', eofr = 's', escmod = 0x20};
};

/** \brief TODO \todo */
struct CFrameAscii:public CFrameModifierBase
{
    /** \brief TODO \todo */
    enum
    {
        esc  = 0x1b /*Escape '/e'   ^[ */,
        sofr = 0x02 /*Start of Text ^A */,
        eofr = 0x03 /*End of Text   ^C */,
        escmod = 0x20
    };
};

/** \brief TODO \todo */
template < class stuffingbytes = struct CFrameReadable > class CFrame
{
    /** \brief TODO \todo */
    typedef stuffingbytes conf;
    /** \brief TODO \todo */
    typedef CFrame<stuffingbytes> thistype;

    /** \brief TODO \todo */
    enum state_t
    {
        //using only lower halfbyte
        invalid  = 0x00,
        started  = 0x01,
        regular  = 0x02,
        stuff    = 0x04,
        fine = 0x08
    };

    /** \brief TODO \todo */
    struct
    {
        state_t tx:4;
        state_t rx:4;
    } state; /*!< state of Machine*/

    public:
        /** \brief TODO \todo */
        CFrame()
        {
            reset();
        }

        /** \brief TODO \todo */
        inline void reset()
        {
            resetTx();
            resetRx();
        }
        
        /** \brief TODO \todo */
        inline void resetTx()
        {
            state.tx = invalid;
        }
        
        /** \brief TODO \todo */
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
        
        /** \brief TODO \todo */
        inline bool sending()
        {
            return (state.tx != invalid && state.tx != fine);
        }
        
        /** \brief TODO \todo */
        inline bool readyToStart()
        {
            return (state.tx == invalid);
        }
        
        /** \brief TODO \todo */
        inline char startFrame()
        {
            if(state.tx == invalid) state.tx = started;
            return conf::sofr;
        }
        
        /** \brief TODO \todo */
        inline char endFrame()
        {
            if(state.tx == regular) state.tx = fine;
            return conf::eofr;
        }
        
        /** \brief TODO \todo */
        inline bool tx_complete()
        {
            return (state.tx == fine);
        }
        
        /** \brief TODO \todo */
        inline bool again()
        {
            return (state.tx != regular );
        }
        
        /** \brief TODO \todo */
        inline char noneChar()
        {
            return conf::none;
        }
        
        /** \brief TODO \todo */
        inline char xonChar()
        {
            return conf::xon;
        }
        
        /** \brief TODO \todo */
        inline char xoffChar()
        {
            return conf::xoff;
        }
        
        /** \brief TODO \todo
         *
         * \param c TODO \todo
         */
        inline char transformOut(char c)
        {
            if(state.tx == started) state.tx = regular;
        
            if(state.tx == regular)
            {
                if(c == conf::eofr || c == conf::sofr || c == conf::esc
                    ||( conf::useNoneChar && c == conf::none)
                    ||(( conf::useXonXoff  )&&( c == conf::xon || c == conf::xoff )))
                {
                    c = conf::esc;
                    state.tx = stuff;
                }
                else
                {
                    c = c;
                }
            }
            else if(state.tx == stuff)
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
        
        /** \brief TODO \todo */
        inline bool receiving()
        {
            return (state.rx != invalid);
        }
        
        /** \brief TODO \todo */
        inline bool restarted()
        {
            return (state.rx == started);
        }
        
        /** \brief TODO \todo */
        inline bool finished()
        {
            return (state.rx == fine);
        }
        
        /** \brief TODO \todo
         *
         * \param c TODO \todo
         */
        inline bool transformIn(char & c)
        {
            if(state.rx == started)state.rx = regular;
            if(state.rx == invalid)
            {
                if(c == conf::sofr)
                {
                    state.rx = started;
                }
            }
            else if(state.rx == regular)
            {
                if(c == conf::eofr)
                {
                    //this frame is complete
                    state.rx = fine;
                }
                else if(c == conf::sofr)
                {
                    //may be missed previous eofr start an other frame
                    state.rx = started;
                }
                else if(c == conf::esc)
                {
                    // next character was stuffed
                    state.rx = stuff;
                }
                else if(( conf::useNoneChar ) && ( c == conf::none ))
                {
                    return false;
                }
                else if(( conf::useXonXoff ) && ( c == conf::xon ))
                {
                    return false;
                }
                else if(( conf::useXonXoff ) && ( c == conf::xoff ))
                {
                    return false;
                }
                else
                {
                    // any non control character
                    c = c;
                    state.rx = regular;
                }
            }
            else if(state.rx == stuff)
            {
                c = ( conf::escmod != 0x00 )?( c^conf::escmod ): c;
                state.rx = regular;
            }
            return (state.rx == regular);
        }
};
