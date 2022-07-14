/******************************************************************************
*
*  OPBbo.hpp
*     QuoddFeed OPBbo Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_OPBbo_H
#define __QUODD_OPBbo_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   O P B b o
//
////////////////////////////////////////////////

/**
 * \class OPBbo
 * \brief This class encapsulates an ::OPBbo (Option BBO) message structure.
 *
 * A OPBbo object is driven into your application via the
 * Channel::OnUpdate( OPBbo & ) virtual method that is called
 * asynchronously when a Option BBO message arrives from the UltraCache.
 */
class OPBbo : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::OPBbo struct
	 *
	 * \return ::OPBbo encapsulated by this object
	 */
	::OPBbo &operator()()
	{
		return qm()._v._OPBbo;
	}

	/**
	 * \brief Returns true if a one-sided quote
	 *
	 * \return True if a one-sided quote
	 */
	bool IsOneSided()
	{
		return ULTRAFEED::OptionSpec::IsOneSided( (*this)() );
	}

	/**
	 * \brief Returns true if a one-sided ASK quote (cached BID)
	 *
	 * \return True if a one-sided ASK quote (cached BID)
	 */
	bool IsCachedBid()
	{
		return ULTRAFEED::OptionSpec::IsCachedBid( (*this)() );
	}

	/**
	 * \brief Returns true if a one-sided BID quote (cached ASK)
	 *
	 * \return True if a one-sided BID quote (cached ASK)
	 */
	bool IsCachedAsk()
		// Returns true if one-sided quote - New Bid + Cached Ask
	{
		return ULTRAFEED::OptionSpec::IsCachedAsk( (*this)() );
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
		::OPBbo &ob = (*this)();
		char     buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f {%s}; %dx%d\n",
		               ob._bidMktCtr, ob._bid,
		               ob._ask, ob._askMktCtr,
		               ob._bidSize, ob._askSize );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class OPBbo

} // namespace QUODD

#endif // __QUODD_OPBbo_H 
