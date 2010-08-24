#pragma once
#include "avr-halib/share/delegate.h"

#warning Do not use this File, it will be replaced very soon


/*! \brief Modifiers of the <code>CFrame</code>*/
struct CFrameModifier
{
    enum {esc='e',sofr='a',eofr='s', escmod=0x01};
};

/*! \class  CFrameBase CFrame.h "avr-halib/share/CFrame.h"
 *  \brief  Base class of the CFrame implementation.
 *
 *  \tparam character device the CFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam usable payload size
 */
template <class BaseCDevice, class FLT = uint8_t, FLT PL = 255>
class CFrameBase: public BaseCDevice
{
    protected:
        /*! \brief  states used by the internal state machine*/
        enum state_t {valid,invalid,regular,stuff};
    public:
        /*! \brief  layer specific information*/
        typedef struct {
            enum {
                payload = PL                /*!< available payload*/
            };
        } info;
        /*! \brief  layer specific message object*/
        typedef struct {
            FLT size;                       /*!< number of data bytes*/
            uint8_t payload[info::payload]; /*!< data of the message object*/
        } mob_t;
};

/*! \class  CFrame CFrame.h "avr-halib/share/CFrame.h"
 *  \brief  This class realizes a bit stuffing by implementing a micro layer.
 *
 *  \tparam character device the CFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam frame modifier used see CFrameModifier
 *  \tparam usable payload size
 */
template <class BaseCDevice, class FLT = uint8_t, class CFM = struct CFrameModifier,FLT PL = 255>
class CFrameNoInt: public CFrameBase<BaseCDevice, FLT, PL>
{
    protected:
        typedef CFrameBase<BaseCDevice, FLT, PL> basetype;
    public:
        /*! \brief  type of the class*/
        typedef CFrameNoInt<BaseCDevice,FLT,CFM,PL> type;
        typedef typename basetype::info info;
        typedef typename basetype::mob_t mob_t;

        CFrameNoInt()  {}
        ~CFrameNoInt() {}

        /*! \brief  Send a message.
         *  \param[in] data buffer to be send
         *  \param[in] size size of the data
         *  \return Returns the size of the data send (zero if unsuccessfull).
         */
        FLT send(const uint8_t* data, FLT size)
        {
            if ( size <= 0 ) return 0;  // stop if there is no data
            typename basetype::state_t aState = basetype::valid;
            FLT result    = size;
            do {                        // implement state machine
                switch ( aState ) {
                    case basetype::valid:         // start frame
                        while( !basetype::ready() );
                        basetype::put( ((char)(CFM::sofr)) );
                        aState = basetype::regular;
                        break;
                    case basetype::regular:       // regular data
                        if ( !size ) {  // stop frame
                            while( !basetype::ready() );
                            basetype::put( (char)(CFM::eofr) );
                            aState = basetype::invalid;
                        } else {
                            switch ( *data ) {
                                case (CFM::sofr):
                                case (CFM::eofr):
                                case (CFM::esc):
                                    while( !basetype::ready() );
                                    basetype::put( (char)(CFM::esc) );
                                    aState = basetype::stuff;
                                    break;
                                default:
                                    while( !basetype::ready() );
                                    basetype::put( (char)(*data) );
                                    data++;
                                    size--;
                                    break;
                            }
                        }
                        break;
                    case basetype::stuff:         // stuff a byte
                        while( !basetype::ready() );
                        basetype::put( (char)((CFM::escmod != 0) ? (*data^CFM::esc) : *data) );
                        data++;
                        size--;
                        aState = basetype::regular;
                        break;
                    default:
                        return 0;
                        break;
                }
            } while( aState != basetype::invalid );
            return result;
        }
        /*! \brief  Sends a message.
         *  \param[in] message source message object
         *  \return Returns the size of the data send (zero if unsuccessfull).
         */
        FLT send(const mob_t& message)
        {
            return send(message.payload, message.size);
        }
        /*! \brief  Reads the last message received.
         *  \param[out] data buffer for received data
         *  \param[in]  size available size of the provided buffer
         *  \return Returns the size of the message payload (zero if unsuccessfull).
         */
        FLT recv(uint8_t* data, FLT size)
        {
            char aChar    = '\0';
            FLT count     = 0;
            typename basetype::state_t aState = basetype::invalid;

            do {
                while( !basetype::ready() );//TODO is this necessary here???
                aState = ( count > size ) ? basetype::invalid : aState;
                if ( basetype::get( aChar ) ) {
                    switch( aChar ) {
                        case (CFM::sofr):
                            if (aState == basetype::invalid) {
                                aState = basetype::regular;
                                count = 0;
                            } else aState = basetype::invalid;
                            break;
                        case (CFM::eofr):
                            aState = (aState == basetype::regular) ? basetype::valid : basetype::invalid;
                            break;
                        case (CFM::esc):
                            aState = (aState == basetype::regular) ? basetype::stuff : basetype::invalid;
                            break;
                        default:
                            if (aState == basetype::regular || aState == basetype::stuff) {
                                aChar = (aState == basetype::stuff && CFM::escmod != 0) ? (aChar^CFM::escmod) : aChar;
                                data[count++] = (uint8_t)aChar;
                                aState  = basetype::regular;
                            } else aState = basetype::invalid;
                            break;
                    }
                }
            } while ( aState != basetype::valid );
            return count;
        }
        /*! \brief  Reads the last message received.
         *  \param[out] message destination message object
         *  \return Returns the size of the message payload (zero if unsuccessfull).
         */
        FLT recv(mob_t& message)
        {
            message.size = recv(message.payload, info::payload);
            return message.size;
        }
};

/*! \class  CFrame CFrame.h "avr-halib/share/CFrame.h"
 *  \brief  This class realizes a bit stuffing by implementing a micro layer.
 *
 *  \tparam character device the CFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam frame modifier used see CFrameModifier
 *  \tparam usable payload size
 */
template <class BaseCDevice, class FLT = uint8_t, class CFM = struct CFrameModifier,FLT PL = 255>
class CFrame: public CFrameBase<BaseCDevice, FLT, PL>
{
    protected:
        typedef CFrameBase<BaseCDevice, FLT, PL> basetype;
    public:
        /*! \brief  type of the class*/
        typedef CFrame<BaseCDevice,FLT,CFM,PL> type;
        typedef typename basetype::info info;
        typedef typename basetype::mob_t mob_t;
    protected:
        /*! \brief  layer specific data object*/
        typedef struct {
            typename basetype::state_t state;  /*!< state of the message object*/
            FLT position;   /*!< current position in the message*/
            mob_t data;     /*!< data packet including size and payload*/
        } mobState_t;

        mobState_t recvMob;
        mobState_t sendMob;

        void getonReceive()
        {
            char aChar = '\0';

            if ( recvMob.position > info::payload ) recvMob.state = basetype::invalid;
            basetype::get( aChar );
            switch( aChar ) {
                case (CFM::sofr):
                    recvMob.data.size = 0;
                    recvMob.state = (recvMob.state == basetype::invalid) ? basetype::regular : basetype::invalid;
                    break;
                case (CFM::eofr):
                    if (recvMob.state == basetype::regular)
                    {
                        recvMob.state = basetype::valid;
                        this->sendonReceive();
                    } else recvMob.state = basetype::invalid;
                    break;
                case (CFM::esc):
                    recvMob.state = (recvMob.state == basetype::regular) ? basetype::stuff : basetype::invalid;
                    break;
                default:
                    if (recvMob.state == basetype::regular || recvMob.state == basetype::stuff) {
                        aChar = (recvMob.state == basetype::stuff && CFM::escmod != 0) ? (aChar^CFM::escmod) : aChar;
                        recvMob.data.payload[recvMob.data.size++] = (uint8_t)aChar;
                        recvMob.state  = basetype::regular;
                    } else recvMob.state = basetype::invalid;
                    break;
            }
        }
        void putonReady()
        {
            uint8_t aByte = 0;

            switch ( sendMob.state ) {              // use state machine
                case basetype::valid:
                    while( !basetype::ready() );
                    basetype::put( ((char)(CFM::sofr)) );
                    sendMob.position = 0;
                    sendMob.state = basetype::regular;
                    break;
                case basetype::regular:
                    if ( sendMob.position == sendMob.data.size ) {
                        while( !basetype::ready() );
                        basetype::put( (char)(CFM::eofr) );
                        sendMob.state = basetype::invalid;
                    } else {
                        aByte = sendMob.data.payload[sendMob.position];
                        switch ( aByte ) {
                            case (CFM::sofr):
                            case (CFM::eofr):
                            case (CFM::esc):
                                while( !basetype::ready() );
                                basetype::put( (char)(CFM::esc) );
                                sendMob.state = basetype::stuff;
                                break;
                            default:
                                while( !basetype::ready() );
                                basetype::put( (char)(aByte) );
                                sendMob.position++;
                                break;
                        }
                    }
                    break;
                case basetype::stuff:
                    while( !basetype::ready() );
                    aByte = sendMob.data.payload[sendMob.position++];
                    basetype::put( (char)((CFM::escmod != 0) ? (aByte^CFM::escmod) : aByte) );
                    sendMob.state = basetype::regular;
                    break;
                default:
                    if ( sendMob.state == basetype::invalid ) {
                        basetype::disableonReady();
                        this->sendonReady();
                    }
                    break;
            }
        }

        void sendonReady()
        {
            while ( sendMob.state == basetype::invalid )
                if( this->onReady.isEmpty() ) break; else { this->onReady();break; }
        }
        void sendonReceive()
        {
            while ( recvMob.state == basetype::valid )
                if( this->onReceive.isEmpty() ) break; else { this->onReceive();break; }
        }
    public:
        Delegate<> onReady;
        Delegate<> onReceive;

        CFrame()
        {
            recvMob.position  = 0;
            recvMob.data.size = 0;
            recvMob.state     = basetype::invalid;
            sendMob.position  = 0;
            sendMob.data.size = 0;
            sendMob.state     = basetype::invalid;
            //-------------------------------------------------------
            basetype::onReady.template bind< type, & type::putonReady >(this);
            basetype::onReceive.template bind<type ,& type::getonReceive>(this);
            basetype::enableonReceive();
        }
        ~CFrame() {}

        void enableonReady()   { sendonReady();   }
        void enableonReceive() { sendonReceive(); }

        /*! \brief  Send a message.
         *  \param[in] data buffer to be send
         *  \param[in] size size of the data
         *  \return Returns the size of the data send (zero if unsuccessfull).
         */
        FLT send(const uint8_t* data, FLT size)
        {
            if ( sendMob.state != basetype::invalid ) return 0;
            sendMob.data.size = 0;
            for (;sendMob.data.size < size; sendMob.data.size++) {
                sendMob.data.payload[sendMob.data.size] = data[sendMob.data.size];
            }
            sendMob.state = basetype::valid;
            // use delegates of BaseCDevice to put data on medium
            basetype::enableonReady();
            return sendMob.data.size;
        }
        /*! \brief  Sends a message.
         *  \param[in] message source message object
         *  \return Returns the size of the data send (zero if unsuccessfull).
         */
        FLT send(const mob_t& message)
        {
            return send(message.payload, message.size);
        }
        /*! \brief  Reads the last message received.
         *  \param[out] data buffer for received data
         *  \param[in]  size available size of the provided buffer
         *  \return Returns the size of the message payload (zero if unsuccessfull).
         */
        FLT recv(uint8_t* data, FLT size)
        {
            FLT count = 0;

            if ( (recvMob.state == basetype::valid) && (recvMob.data.size <= size) )
            {
                for (;count < recvMob.data.size; count++)
                    data[count] = recvMob.data.payload[count];
                recvMob.state = basetype::invalid;
            }
            return count;
        }
        /*! \brief  Reads the last message received.
         *  \param[out] message destination message object
         *  \return Returns the size of the message payload (zero if unsuccessfull).
         */
        FLT recv(mob_t& message)
        {
            message.size = recv(message.payload, info::payload);
            return message.size;
        }
};
