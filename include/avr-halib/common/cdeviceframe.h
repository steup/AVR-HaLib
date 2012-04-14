#pragma once
#include "delegate.h"
#include "cframe.new.h"

/*! \class  CDeviceFrameBase CFrame.h "avr-halib/share/CFrame.h"
 *  \brief  Base class of the CFrame implementation.
 *
 *  \tparam character device the CFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam usable payload size
 */
template <class BaseCDevice, class FLT = uint8_t, FLT PL = 255>
class CDeviceFrameBase: public BaseCDevice
{
	public:
		/*! \brief  layer specific information*/
		typedef struct
		{
			enum
			{
				payload = PL                /*!< available payload*/
			};
		} info;
		/*! \brief  layer specific message object*/
		typedef struct
		{
			FLT size;                       /*!< number of data bytes*/
			uint8_t payload[info::payload]; /*!< data of the message object*/
		} mob_t;
};

/*! \class  CDeviceFrame CDeviceFrame.h "avr-halib/share/CDeviceFrame.h"
 *  \brief  This class realizes a bit stuffing by implementing a micro layer.
 *
 *  \tparam character device the CDeviceFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam usable payload size
 *  \tparam State Machine to use
 */
template <class BaseCDevice, class FLT = uint8_t, FLT PL = 255, class StateMachine = CFrame<> >
class CDeviceFrameNoInt: public CDeviceFrameBase<BaseCDevice, FLT, PL>
{
	protected:
		typedef CDeviceFrameBase<BaseCDevice, FLT, PL> basetype;
	public:
		/*! \brief  type of the class*/
		typedef CDeviceFrameNoInt<BaseCDevice, FLT, PL, StateMachine> type;
		typedef typename basetype::info info;
		typedef typename basetype::mob_t mob_t;

		CDeviceFrameNoInt()  {}
		~CDeviceFrameNoInt() {}

		/*! \brief  Send a message.
		 *  \param[in] data buffer to be send
		 *  \param[in] size size of the data
		 *  \return Returns the size of the data send (zero if unsuccessfull).
		 */
		FLT send(const uint8_t* data, FLT size)
		{
			if ( size <= 0 ) return 0;  // stop if there is no data
			StateMachine cframe;
			FLT result    = size;
			basetype::put( cframe.startFrame());
			for(int i = 0 ; i < size; i++)
			{
				do
				{   
					while( !this->ready() );
					basetype::put(cframe.transformOut(data[i]));
				}
				while(cframe.again());
			}
			
			while( !this->ready() );
			basetype::put(cframe.endFrame());
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
			FLT count     = 0;
			StateMachine cframe;

			for (char * buffer = (char *) data;/*cframe.finished() breaks loop */;)
			{
				char c;
				while( !basetype::get(c) );
				if( cframe.transformIn(c) )
				{
					if(count == size)
					{
						cframe.resetRx();
						count = 0;
						buffer = (char *) data;
					}
					else
					{
						* buffer = c;
						buffer++;
						count++;
					}
				}
				else if ( cframe.restarted() )
				{
					count = 0;
					buffer = (char *) data;
				}
				else if ( cframe.finished() )
				{
					break;
				}
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

/*! \class  CDeviceFrame CDeviceFrame.h "avr-halib/share/CDeviceFrame.h"
 *  \brief  This class realizes a bit stuffing by implementing a micro layer.
 *
 *  \tparam character device the CDeviceFrame is based on
 *  \tparam type to determin the size of a frame (default <code>uint8_t</code>)
 *  \tparam usable payload size
 *  \tparam State Machine to use
 */
template <class BaseCDevice,  class FLT = uint8_t, FLT PL = 255, class StateMachine= CFrame<> >
class CDeviceFrame: public CDeviceFrameBase<BaseCDevice, FLT, PL>
{
	protected:
		typedef CDeviceFrameBase<BaseCDevice, FLT, PL> basetype;
	public:
		/*! \brief  type of the class*/
		typedef CDeviceFrame<BaseCDevice, FLT, PL, StateMachine> type;
		typedef typename basetype::info info;
		typedef typename basetype::mob_t mob_t;
	protected:
		/*! \brief  layer specific data object*/
		typedef struct
		{
			FLT position;   /*!< current position in the message*/
			mob_t data;     /*!< data packet including size and payload*/
		} mobState_t;

		mobState_t recvMob;
		mobState_t sendMob;
		StateMachine cframe;

		void getonReceive()
		{
			char c;
			if ( recvMob.position == info::payload ) cframe.resetRx();
			basetype::get( c );
			if(cframe.transformIn(c))
			{
				recvMob.data.payload[recvMob.data.size++] = (uint8_t) c;
			}
			else if ( cframe.restarted() )
			{
				recvMob.data.size = 0;
			}
			else if ( cframe.finished() )
			{
				this->sendonReceive();
			}
		}

		void putonReady()
		{
			if (cframe.readyToStart() &&  sendMob.position == 0 && sendMob.data.size > 0)
				basetype::put(cframe.startFrame());
			else if (!cframe.sending())
			{
				basetype::disableonReady();
				this->sendonReady();
			}
			else
			{
				if ( sendMob.position == sendMob.data.size )
					basetype::put(cframe.endFrame());
				else
				{
					basetype::put(cframe.transformOut(sendMob.data.payload[sendMob.position]));
					if (!cframe.again())
						sendMob.position++;
				}
			}
		}


		void sendonReady()
		{
			while ( !cframe.sending())
					if( this->onReady.isEmpty() ) break; else { this->onReady(); break; }
		}
		void sendonReceive()
		{
			while ( cframe.finished() )
					if( this->onReceive.isEmpty() ) break; else { this->onReceive(); break; }
		}
	public:
		Delegate<> onReady;
		Delegate<> onReceive;

		CDeviceFrame()
		{
			recvMob.position  = 0;
			recvMob.data.size = 0;
			sendMob.position  = 0;
			sendMob.data.size = 0;
			//-------------------------------------------------------
			basetype::onReady.template bind< type, & type::putonReady >(this);
			basetype::onReceive.template bind<type ,& type::getonReceive>(this);
			basetype::enableonReceive();
		}
		~CDeviceFrame() {}

		void enableonReady()   { sendonReady();   }
		void enableonReceive() { sendonReceive(); }

		/*! \brief  Send a message.
		 *  \param[in] data buffer to be send
		 *  \param[in] size size of the data
		 *  \return Returns the size of the data send (zero if unsuccessfull).
		 */
		FLT send(const uint8_t* data, FLT size)
		{
			if ( cframe.sending() ) return 0;
			sendMob.data.size = 0;
			sendMob.position  = 0;
			for (; sendMob.data.size < size; sendMob.data.size++)
			{
				sendMob.data.payload[sendMob.data.size] = data[sendMob.data.size];
			}
			cframe.resetTx();
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

			if ( cframe.finished() && (recvMob.data.size <= size) )
			{
				for (; count < recvMob.data.size; count++)
					data[count] = recvMob.data.payload[count];
				cframe.resetRx();
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
