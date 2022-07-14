/******************************************************************************
*
*  Heartbeat.hpp
*     QuoddFeed Heartbeat Message
*
*  REVISION HISTORY:
*     18 JAN 2018 jcs  Created.
*
*  (c) 1994-2019 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_Heartbeat_H
#define __QUODD_Heartbeat_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//        c l a s s   H e a r t b ea t
//
////////////////////////////////////////////////

/**
 * \class Heartbeat
 * \brief This class encapsulates an ::Heartbeat message struct.
 *
 * A Heartbeat object is driven into your application via the
 * Channel::OnUpdate( Heartbeat & ) virtual method that is called
 * asynchronously when a Heartbeat message arrives from the UltraCache.
 */
class Heartbeat : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::Heartbeat struct
	 *
	 * \return ::Heartbeat encapsulated by this object
	 */
	::Heartbeat &operator()()
	{
	   return qm()._v._Heartbeat;
	}

	////////////////////////////////////
	// QUODD::Message Overrides
	////////////////////////////////////
public:
	/**
	 * \brief Dumps formatted message header
	 *
	 * \return Pointer to formatted message header
	 */
	virtual const char *Dump()
	{
	   // Header Only

	   Message::Dump();
	   dump_ += "\n";
	   return dump_.data();
	}

};  // class Heartbeat

} // namespace QUODD

#endif // __QUODD_Heartbeat_H 
