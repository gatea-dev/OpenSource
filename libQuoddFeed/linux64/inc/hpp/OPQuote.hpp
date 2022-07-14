/******************************************************************************
*
*  OPQuote.hpp
*     QuoddFeed OPQuote Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_OPQuote_H
#define __QUODD_OPQuote_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   O P Q u o t e
//
////////////////////////////////////////////////

/**
 * \class OPQuote
 * \brief This class encapsulates an ::OPQuote (Option Market Center Quote) 
 * message structure.
 *
 * A OPQuote object is driven into your application via the
 * Channel::OnUpdate( OPQuote & ) virtual method that is called
 * asynchronously when a Option Market Center Quote message arrives 
 * from the UltraCache.
 */
class OPQuote : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::OPQuote struct
	 *
	 * \return ::OPQuote encapsulated by this object
	 */
	::OPQuote &operator()()
	{
		return qm()._v._OPQuote;
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
		::OPQuote &oq = (*this)();
		char     buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f; %dx%d\n",
		               oq._mktCtr, oq._bid, oq._ask,
 		               oq._bidSize, oq._askSize );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class OPQuote

} // namespace QUODD

#endif // __QUODD_OPQuote_H 
