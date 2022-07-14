/******************************************************************************
*
*  Status.hpp
*     QuoddFeed Status Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_Status_H
#define __QUODD_Status_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//        c l a s s   S t a t u s
//
////////////////////////////////////////////////

/**
 * \class Status
 * \brief This class encapsulates an ::Status (Stream DEAD) message structure.
 *
 * A Status object is driven into your application via the
 * Channel::OnUpdate( Status & ) virtual method that is called
 * asynchronously when a DEAD status message arrives from the UltraCache.
 */
class Status : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::Status struct
	 *
	 * \return ::Status encapsulated by this object
	 */
	::Status &operator()()
	{
		return qm()._v._Status;
	}

	/**
	 * \brief Returns status description
	 *
	 * \return Status description
	 */
	const char *Reason()
	{
		return (*this)()._msg;
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
		char buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "%s\n", Reason() );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class Status

} // namespace QUODD

#endif // __QUODD_Status_H 
