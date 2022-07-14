/***************************************************************************** 
*
*  IDXSummary.hpp
*     QuoddFeed IDXSummary Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_IDXSummary_H
#define __QUODD_IDXSummary_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//     c l a s s   I D X S u m m a r y
//
////////////////////////////////////////////////

/**
 * \class IDXSummary
 * \brief This class encapsulates an ::IDXSummary (Index Summary) 
 * message structure.
 *
 * A IDXSummary object is driven into your application via the
 * Channel::OnUpdate( IDXSummary & ) virtual method that is called
 * asynchronously when a Index Summary message arrives from the UltraCache.
 */

class IDXSummary : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::IDXSummary struct
	 *
	 * \return ::IDXSummary encapsulated by this object
	 */
	::IDXSummary &operator()()
	{
		return qm()._v._IDXSummary;
	}

	/**
	 * \brief Returns daily open
	 *
	 * \return Daily open
	 */
	double Open()
	{
		return (*this)()._open;
	}

	/**
	 * \brief Returns daily high
	 *
	 * \return Daily high
	 */
	double High()
	{
		return (*this)()._high;
	}

	/**
	 * \brief Returns daily low
	 *
	 * \return Daily low
	 */
	double Low()
	{
		return (*this)()._low;
	}

	/**
	 * \brief Returns closing price
	 *
	 * \return Closing price
	 */
	double Close()
	{
		return (*this)()._close;
	}

	/**
	 * \brief Returns net change from previous day close
	 *
	 * \return Net change from previous day
	 */
	double NetChange()
	{
		return (*this)()._netChg;
	}

	/**
	 * \brief Returns textual description of summary type
	 *
	 * \return Textual description of summary type
	 */
	const char *SummType()
	{
		return (*this)()._summType;
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
		cp += sprintf( cp, "{%s }", SummType() );
		cp += sprintf( cp, "OPN=%.2f; HI=%.2f; ", Open(), High() );
		cp += sprintf( cp, "LO=%.2f; CLS=%.2f; ", Low(), Close() );
		cp += sprintf( cp, "NET=%.2f\n", NetChange() );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class IDXSummary

} // namespace QUODD

#endif // __QUODD_IDXSummary_H 
