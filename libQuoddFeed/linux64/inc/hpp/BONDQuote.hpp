/******************************************************************************
*
*  BONDQuote.hpp
*     QuoddFeed BONDQuote Message
*
*  REVISION HISTORY:
*     10 FEB 2016 jcs  Created.
*
*  (c) 1994-2016 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_BONDQuote_H
#define __QUODD_BONDQuote_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   B O N D Q u o t e
//
////////////////////////////////////////////////

/**
 * \class BONDQuote
 * \brief This class encapsulates an ::BONDQuote (Bond Quote)
 * message structure.
 *
 * A BONDQuote object is driven into your application via the
 * Channel::OnUpdate( BONDQuote & ) virtual method that is called
 * asynchronously when a Bond Quote message arrives from the UltraCache.
 */
class BONDQuote : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::BONDQuote struct
	 *
	 * \return ::BONDQuote encapsulated by this object
	 */
	::BONDQuote &operator()()
	{
		return qm()._v._BONDQuote;
	}

	/**
	 * \brief  Returns comma-separated list of quote flag descriptions
	 *
	 * \return Comma-separated list of quote flag descriptions
	 */
	const char *QuoteFlags()
	{
		return ULTRAFEED::BondSpec::QuoteFlags( (*this)(), _rtn );
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
		::BONDQuote &bq = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "%8.6f x %8.6f; ", bq._bid, bq._ask );
		cp += sprintf( cp, "%dx%d; ", bq._bidSize, bq._askSize );
		cp += sprintf( cp, "{%s}; YLD : ", bq._mktCtr );
		cp += sprintf( cp, "%.f x %.f\n", bq._bidYield, bq._askYield );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class BONDQuote

} // namespace QUODD

#endif // __QUODD_BONDQuote_H 
