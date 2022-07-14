/******************************************************************************
*
*  FUTRTrade.hpp
*     QuoddFeed FUTRTrade Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_FUTRTrade_H
#define __QUODD_FUTRTrade_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   F U T R T r a d e
//
////////////////////////////////////////////////

/**
 * \class FUTRTrade
 * \brief This class encapsulates an ::FUTRTrade (Futures Trade)
 * message structure.
 *
 * A FUTRTrade object is driven into your application via the
 * Channel::OnUpdate( FUTRTrade & ) virtual method that is called
 * asynchronously when a Futures Trade message arrives from the UltraCache.
 */

class FUTRTrade : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::FUTRTrade struct
	 *
	 * \return ::FUTRQuote encapsulated by this object
	 */
	::FUTRTrade &operator()()
	{
		return qm()._v._FUTRTrade;
	}

	/**
	 * \brief Returns previous day closing price
	 *
	 * \return Previous day closing price
	 */
	double PrevClose()
	{
		::FUTRTrade &ft = (*this)();

		return( ft._trdPrc - ft._netChg ); 
	}

	/**
	 * \brief  Returns comma-separated list of trade flag descriptions
	 *
	 * \return Comma-separated list of trade flag descriptions
	 */
	const char *TradeFlags()
	{
		return ULTRAFEED::FuturesSpec::TradeFlags( (*this)(), _rtn );
	}

	/**
	 * \brief Returns description of sale condition
	 *
	 * \return Description of sale condition
	 */
	const char *SaleCondition()
	{
		return ULTRAFEED::FuturesSpec::SaleCondition( (*this)() );
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
		::FUTRTrade &ft = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "%ld @ %8.6f", ft._trdVol, ft._trdPrc );
		cp += sprintf( cp, " {%s}\n", ft._mktCtr );
		cp += sprintf( cp, "   _openPrc = %.6f; ", ft._openPrc );
		cp += sprintf( cp, "_openVol = %ld\n", ft._openVol );
		cp += sprintf( cp, "   _high = %.6f; _low=%.6f\n", ft._high, ft._low );
		cp += sprintf( cp, "   _netChg = %.6f; ", ft._netChg );
		cp += sprintf( cp, "_pctChg = %.6f\n", ft._pctChg );
		cp += sprintf( cp, "   _tnOvr = %ld; ", ft._tnOvr );
		cp += sprintf( cp, "_acVol = %ld\n", ft._acVol );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class FUTRTrade

} // namespace QUODD

#endif // __QUODD_FUTRTrade_H 
