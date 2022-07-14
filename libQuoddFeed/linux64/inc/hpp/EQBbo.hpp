/******************************************************************************
*
*  EQBbo.hpp
*     QuoddFeed EQBbo Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_EQBbo_H
#define __QUODD_EQBbo_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//        c l a s s   E Q B b o
//
////////////////////////////////////////////////

/**
 * \class EQBbo
 * \brief This class encapsulates an ::EQBbo (Equity BBO) message structure.
 *
 * A EQBbo object is driven into your application via the
 * Channel::OnUpdate( EQBbo & ) virtual method that is called
 * asynchronously when a Equity BBO message arrives from the UltraCache.
 */

class EQBbo : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::EQBbo struct
	 *
	 * \return ::EQBbo encapsulated by this object
	 */
	::EQBbo &operator()()
	{
		return qm()._v._EQBbo;
	}

	/**
	 * \brief Returns the Limit Up / Limit Down description
	 *
	 * Call this funciton when the LULD indicator is in the 
	 * ::EQBbo._luldFlags field.
	 *
	 * \return Limit Up / Down description in A thru G format
	 */
	const char *LimitUpDown()
	{
		return ULTRAFEED::EquitySpec::LimitUpDown( (*this)() );
	}

	/**
	 * \brief Returns the Limit Up / Limit Down description
	 *
	 * Call this funciton when the LULD indicator is in the 
	 * ::EQBbo._flags field.
	 *
	 * \return Limit Up / Down description in A thru G format
	 */
	const char *LimitUpDownR()
	{
		return ULTRAFEED::EquitySpec::LimitUpDownR( (*this)() );
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
		::EQBbo &b = (*this)();
		char     buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f {%s}; %dx%d\n",
		                  b._bidMktCtr, b._bid,
		                  b._ask, b._askMktCtr,
		                  b._bidSize, b._askSize );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class EQBbo

} // namespace QUODD

#endif // __QUODD_EQBbo_H 
