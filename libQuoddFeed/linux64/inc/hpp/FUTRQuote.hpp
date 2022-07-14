/******************************************************************************
*
*  FUTRQuote.hpp
*     QuoddFeed FUTRQuote Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_FUTRQuote_H
#define __QUODD_FUTRQuote_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   F U T R Q u o t e
//
////////////////////////////////////////////////

/**
 * \class FUTRQuote
 * \brief This class encapsulates an ::FUTRQuote (Futures Quote)
 * message structure.
 *
 * A FUTRQuote object is driven into your application via the
 * Channel::OnUpdate( FUTRQuote & ) virtual method that is called
 * asynchronously when a Futures Quote message arrives from the UltraCache.
 */
class FUTRQuote : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::FUTRQuote struct
	 *
	 * \return ::FUTRQuote encapsulated by this object
	 */
	::FUTRQuote &operator()()
	{
		return qm()._v._FUTRQuote;
	}

	/**
	 * \brief  Returns comma-separated list of quote flag descriptions
	 *
	 * \return Comma-separated list of quote flag descriptions
	 */
	const char *QuoteFlags()
	{
		return ULTRAFEED::FuturesSpec::QuoteFlags( (*this)(), _rtn );
	}

	/**
	 * \brief Returns textual description of Exception Indicator
	 *
	 * \return textual description of Exception Indicator
	 */
	const char *ExceptionalIndicator()
	{
		return ULTRAFEED::FuturesSpec::ExceptionalIndicator( (*this)() );
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
		::FUTRQuote &fq = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "%8.6f x %8.6f; %dx%d\n",
		               fq._bid, fq._ask, fq._bidSize, fq._askSize );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class FUTRQuote

} // namespace QUODD

#endif // __QUODD_FUTRQuote_H 
