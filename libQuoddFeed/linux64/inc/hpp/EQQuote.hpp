/******************************************************************************
*
*  EQQuote.hpp
*     QuoddFeed EQQuote Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
*******************************************************************************/
#ifndef __QUODD_EQQuote_H
#define __QUODD_EQQuote_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   E Q Q u o t e
//
////////////////////////////////////////////////

/**
 * \class EQQuote
 * \brief This class encapsulates an ::EQQuote (Equity Market Center Quote) 
 * message structure.
 *
 * A EQQuote object is driven into your application via the
 * Channel::OnUpdate( EQQuote & ) virtual method that is called
 * asynchronously when a Equity Market Center Quote message arrives from 
 * the UltraCache.
 */
class EQQuote : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::EQQuote struct
	 *
	 * \return ::EQQuote encapsulated by this object
	 */
	::EQQuote &operator()()
	{
		return qm()._v._EQQuote;
	}

	/**
	 * \brief Returns true if a one-sided quote
	 *
	 * \return True if a one-sided quote
	 */
	bool IsOneSided()
	{
		return ULTRAFEED::EquitySpec::IsOneSided( (*this)() );
	}

	/**
	 * \brief Returns true if a one-sided ASK quote (cached BID)
	 *
	 * \return True if a one-sided ASK quote (cached BID)
	 */
	bool IsCachedBid()
	{
		return ULTRAFEED::EquitySpec::IsCachedBid( (*this)() );
	}

	/**
	 * \brief Returns true if a one-sided BID quote (cached ASK)
	 *
	 * \return True if a one-sided BID quote (cached ASK)
	 */
	bool IsCachedAsk()
	{
		return ULTRAFEED::EquitySpec::IsCachedAsk( (*this)() );
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
		::EQQuote &q = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f; %dx%d\n",
		                  q._mktCtr, q._bid, q._ask,
 		                  q._bidSize, q._askSize );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class EQQuote

} // namespace QUODD

#endif // __QUODD_EQQuote_H 
