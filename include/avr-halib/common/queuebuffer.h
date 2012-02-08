/** \addtogroup share */
/*@{*/
/**
 *	\file	avr-halib/share/queuebuffer.h
 *	\brief	Implements a queue (FIFO)
 *	\author	Karl Fessel, Philipp Werner
 *
 *	This file is part of avr-halib. See COPYING for copyright details.
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
		
		bool isFull_i() const __attribute__ ((always_inline))
		{
// 			return (pose + 1) % Length == poss;  //dierekt aber ineffizient 
// 			return pose + 1 == Length?0:(pose + 1) == poss;
			return ((poss == 0)?Length-1:poss-1) == pose;
		}
		
		
		void add1overrun(Length_Type &pos)__attribute__ ((noinline))
		{
			pos = ((pos + 1) == Length)? 0 : pos+1;
		}
	public:
		/// Constructor
		QueueBuffer() :
			poss(0), pose(0)
		{}

		/**	\brief	Get (and remove) the first element of the queue
		 *	\param	e	Reference to a variable which is used for returning the element
		 *	\returns	false, if the queue is empty
		 */
		bool get(Data_Type & e)
		{
			if(pose != poss)
			{
				e = bu[poss];
				add1overrun(poss);
				return true;
			}
			else
				return false;
		}

		/**	\brief	Get the first element of the queue without removing is
		 *	\param	e	Reference to a variable which is used for returning the element
		 *	\returns	false, if the queue is empty
		 */
		bool look(Data_Type & e)
		{
			if(pose != poss)
			{
				e = bu[poss];
				return true;
			}
			else
				return false;
		}
		
		/**	\brief	Puts an element at the end of the queue
		 *	\param	e	Element to add
		 *	\attention	If the queue is full the first (oldest) element is lost!
		 */
		void put(Data_Type e)
		{
			if (isFull())
			{
				// queue is full -> delete first element
// 				poss++;poss %=Length;
				add1overrun(poss);
				
			}
			bu[pose]= e;
			add1overrun(pose);
		}

		
		/// Makes the queue empty
		void clear()
		{
			poss = pose;
		}
		
		/// Returns true if the queue is empty
		bool isEmpty() const
		{
			return poss == pose;
		}
		
		/// Returns true if the queue is full
		bool isFull() const
		{
			return isFull_i();
		}
		
		/// Returns the number of elements
		Length_Type count() const
		{
			return (pose > poss)?(pose - poss):((pose + Length) - poss);
		}
};

/*@}*/
