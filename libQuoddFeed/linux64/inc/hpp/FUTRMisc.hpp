/******************************************************************************
*
*  FUTRMisc.hpp
*     QuoddFeed FUTRMisc Message
*
*  REVISION HISTORY:
*     24 MAR 2015 jcs  Created.
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_FUTRMisc_H
#define __QUODD_FUTRMisc_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//        c l a s s   F U T R M i s c
//
////////////////////////////////////////////////

/**
 * \class FUTRMisc
 * \brief This class encapsulates an ::FUTRMisc (Futures Hi/Lo/Last 
 * or Open Interest) message structure.
 *
 * A FUTRMisc object is driven into your application via the
 * Channel::OnUpdate( FUTRMisc & ) virtual method that is called
 * asynchronously when a Futures Quote message arrives from the UltraCache.
 */
class FUTRMisc : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::FUTRMisc struct
	 *
	 * \return ::FUTRMisc encapsulated by this object
	 */
	::FUTRMisc &operator()()
	{
		return qm()._v._FUTRMisc;
	}

	/**
	 * \brief  Returns true if this is Hi/Lo/Last msg
	 *
	 * \return true if this is Hi/Lo/Last msg
	 */
	bool IsHiLo()
	{
		return( (*this)()._bHiLo == 1 );
	}

	/**
	 * \brief  Returns true if this is Open Interest Msg
	 *
	 * \return true if this is Open Interest Msg
	 */
	bool IsOpenInterest()
	{
		return !IsHiLo();
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
		::FUTRMisc &q = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		if ( IsHiLo() ) {
			cp += sprintf( cp, "   HI/LO : %.6f, %.6f\n", q._highPrc, q._lowPrc );
			cp += sprintf( cp, "   LAST  : %.6f\n", q._lastPrc );
		}
		else
			cp += sprintf( cp, "   OPNINT : %ld\n", q._openInt );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class FUTRMisc

} // namespace QUODD

#endif // __QUODD_FUTRMisc_H 
