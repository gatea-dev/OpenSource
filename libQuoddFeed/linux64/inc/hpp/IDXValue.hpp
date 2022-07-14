/******************************************************************************
*
*  IDXValue.hpp
*     QuoddFeed IDXValue Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_IDXValue_H
#define __QUODD_IDXValue_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//     c l a s s   I D X V a l u e
//
////////////////////////////////////////////////

/**
 * \class IDXValue
 * \brief This class encapsulates an ::IDXValue (Index Value) message structure.
 *
 * A IDXValue object is driven into your application via the
 * Channel::OnUpdate( IDXValue & ) virtual method that is called
 * asynchronously when a Index Value message arrives from the UltraCache.
 */

class IDXValue : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::IDXValue struct
	 *
	 * \return ::IDXValue encapsulated by this object
	 */
	::IDXValue &operator()()
	{
		return qm()._v._IDXValue;
	}

	/**
	 * \brief Returns true if this is a summary message.
	 *
	 * \return true if this is a summary message.
	 */
	bool IsSummary()
	{
		return( mtSub() == _ixSubSUMM );
	}

	/**
	 * \brief Returns index value
	 *
	 * \return Index value
	 */
	double Value()
	{
		return (*this)()._value;
	}

	/**
	 * \brief Returns last index value
	 *
	 * \return Last Index value
	 */
	double LastPrice()
	{
		return Value();
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
	 * \brief Returns net change from previous day close
	 *
	 * \return Net change from previous day
	 */
	double NetChange()
	{
		return (*this)()._netChg;
	}

	/**
	 * \brief Returns percent change from previous day
	 *
	 * \return Percent change from previous day
	 */
	double PctChange()
	{
		return (*this)()._pctChg;
	}

	/**
	 * \brief Returns previous day close
	 *
	 * \return Previous day close
	 */
	double PrevClose()
	{
		return( LastPrice() - NetChange() );
	}

	/**
	 * \brief Returns textual description of how index is calculated
	 *
	 * \return Textual description of how index is calculated
	 */
	const char *CalcMethod()
	{
		return (*this)()._calcMethod;
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
		cp += sprintf( cp, "{%s} %8.4f\n", CalcMethod(), Value() );
		cp += sprintf( cp, "   _netChg = %.4f; ", NetChange() );
		cp += sprintf( cp, "_pctChg = %.4f\n", PctChange() );
		cp += sprintf( cp, "   _open = %.4f; ", Open() );
		cp += sprintf( cp, "_high = %.4f;", High() );
		cp += sprintf( cp, "_low = %.4f\n", Low() );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class IDXValue

} // namespace QUODD

#endif // __QUODD_IDXValue_H 
