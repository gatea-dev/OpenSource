/******************************************************************************
*
*  EQLimitUpDn.hpp
*     QuoddFeed EQLimitUpDn Message
*
*  REVISION HISTORY:
*     15 DEC 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
*******************************************************************************/
#ifndef __QUODD_EQLimitUpDn_H
#define __QUODD_EQLimitUpDn_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//         c l a s s   E Q T r a d e
//
////////////////////////////////////////////////

/**
 * \class EQLimitUpDn
 * \brief This class encapsulates an ::EQLimitUpDn (Equity Limit Up / Down) 
 *  message structure.
 *
 * A EQLimitUpDn object is driven into your application via the
 * Channel::OnUpdate( EQLimitUpDn & ) virtual method that is called
 * asynchronously when a Equity Trade message arrives from the UltraCache.
 */

class EQLimitUpDn : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::EQLimitUpDn struct
	 *
	 * \return ::EQLimitUpDn encapsulated by this object
	 */
	::EQLimitUpDn &operator()()
	{
		return qm()._v._EQLimitUpDn;
	}

	/**
	 * \brief  Returns textual description of the Indicator Flag
	 *
	 * \return Textual description of the Indicator Flag
	 */
	const char *Indicator()
	{
		return ULTRAFEED::EquitySpec::Indicator( (*this)(), _rtn );
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
		::EQLimitUpDn &lu = (*this)();
		char           buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f\n",
		         Indicator(), lu._lowerPrice, lu._upperPrice );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class EQLimitUpDn

} // namespace QUODD

#endif // __QUODD_EQLimitUpDn_H 
