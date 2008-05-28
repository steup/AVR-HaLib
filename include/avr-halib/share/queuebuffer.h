/**
 *	\file avr-halib/share/queuebuffer.h
 *	\brief	Implements a queue (FIFO)
 *	\author	Karl Fessel, Philipp Werner
 */


#pragma once

/*!	\brief  Queue
*	\param	Data_Type	Type of elements to store
*	\param	Length_Type	Type used for indexing and length
*	\param	Length		Maximum number of elements (buffer size)
*
*/
template <class Data_Type, class Length_Type, Length_Type Length>
		class QueueBuffer
{
	private:
		Data_Type bu[Length];	// ring buffer
		Length_Type poss;	// first Element
		Length_Type pose;	// last Element

	public:
		/// Constructor
		QueueBuffer() :
			poss(0), pose(0)
		{}

		/**	\brief	Get (and remove) the first element of the queue
		 *	\param	e	Reference to a variable which is used for returning the element
		 *	\returns	false, if the queue is empty
		 */
		bool get(Data_Type & e);

		/**	\brief	Get the first element of the queue without removing is
		 *	\param	e	Reference to a variable which is used for returning the element
		 *	\returns	false, if the queue is empty
		 */
		bool look(Data_Type & e);
		
		/**	\brief	Puts an element at the end of the queue
		 *	\param	e	Element to add
		 *	\attention	If the queue is full the first (oldest) element is lost!
		 */
		void put(Data_Type e);
		
		/// Makes the queue empty
		void clear()
		{
			poss = pose;
		}
		
		/// Returns true if the queue is empty
		bool isEmpty()
		{
			return poss == pose;
		}
		
		/// Returns true if the queue is full
		bool isFull()
		{
			return (pose + 1) % Length == poss;
		}
		
		/// Returns the number of elements
		Length_Type count()
		{
			return ((pose + Length) - poss) % Length;
		}
};


template <class Data_Type, class Length_Type, Length_Type Length>
		void QueueBuffer<Data_Type,Length_Type,Length>::put(Data_Type c)
{
	if ((pose + 1) % Length == poss)
	{
		// queue is full -> delete first element
		poss++;
		poss %=Length;
	}
	bu[pose]=c;
	pose++;
	pose %=Length;
}

template <class Data_Type, class Length_Type, Length_Type Length>
		bool QueueBuffer<Data_Type,Length_Type,Length>::get(Data_Type & e)
{
	if(pose != poss)
	{
		e = bu[poss];
		poss++;
		poss %=Length;
		return true;
	}
	else
		return false;
}

template <class Data_Type, class Length_Type, Length_Type Length>
		bool QueueBuffer<Data_Type,Length_Type,Length>::look(Data_Type & e)
{
	if(pose != poss)
	{
		e = bu[poss];
		return true;
	}
	else
		return false;
}


