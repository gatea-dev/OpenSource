/******************************************************************************
*
*  EQQuoteMM.hpp
*     QuoddFeed EQQuoteMM Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
*******************************************************************************/
#ifndef __QUODD_EQQuoteMM_H
#define __QUODD_EQQuoteMM_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   E Q Q u o t e M M
//
////////////////////////////////////////////////

/**
 * \class EQQuoteMM
 * \brief This class encapsulates an ::EQQuoteMM (Equity Market Maker Quote) 
 * message structure.
 *
 * A EQQuoteMM object is driven into your application via the
 * Channel::OnUpdate( EQQuoteMM & ) virtual method that is called
 * asynchronously when a Equity Market Maker Quote message arrives from 
 * the UltraCache.
 */

class EQQuoteMM : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::EQQuoteMM struct
	 *
	 * \return ::EQQuoteMM encapsulated by this object
	 */
	::EQQuoteMM &operator()()
	{
		return qm()._v._EQQuoteMM;
	}

	/**
	 * \brief Returns true if this quote is open
	 *
	 * \return true if this quote is open
	 */
	bool IsOpen()
	{
		return ULTRAFEED::EquitySpec::IsOpen( (*this)() );
	}

	/**
	 * \brief Returns true if this quote contains displayable bid
	 *
	 * \return true if this quote contains displayable bid
	 */
	bool CanDisplayBid()
	{
		return ULTRAFEED::EquitySpec::CanDisplayBid( (*this)() );
	}

	/**
	 * \brief Returns true if this quote contains displayable ask
	 *
	 * \return true if this quote contains displayable ask
	 */
	bool CanDisplayAsk()
		// Returns true if this quote contains displayable ask
	{
		return ULTRAFEED::EquitySpec::CanDisplayAsk( (*this)() );
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
		::EQQuoteMM &mm = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f; %dx%d {%s}\n",
		                  mm._mktCtr, mm._bid, mm._ask,
		                  mm._bidSize, mm._askSize, mm._mmid );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class EQQuoteMM

} // namespace QUODD

#endif // __QUODD_EQQuoteMM_H 
