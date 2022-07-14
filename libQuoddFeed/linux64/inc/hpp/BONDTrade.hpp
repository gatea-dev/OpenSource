/******************************************************************************
*
*  BONDTrade.hpp
*     QuoddFeed BONDTrade Message
*
*  REVISION HISTORY:
*     10 FEB 2016 jcs  Created.
*
*  (c) 1994-2016 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_BONDTrade_H
#define __QUODD_BONDTrade_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   B O N D T r a d e
//
////////////////////////////////////////////////

/**
 * \class BONDTrade
 * \brief This class encapsulates an ::BONDTrade (Bond Trade)
 * message structure.
 *
 * A BONDTrade object is driven into your application via the
 * Channel::OnUpdate( BONDTrade & ) virtual method that is called
 * asynchronously when a Bond Trade message arrives from the UltraCache.
 */

class BONDTrade : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::BONDTrade struct
	 *
	 * \return ::BONDQuote encapsulated by this object
	 */
	::BONDTrade &operator()()
	{
		return qm()._v._BONDTrade;
	}

	/**
	 * \brief Returns previous day closing price
	 *
	 * \return Previous day closing price
	 */
	double PrevClose()
	{
		::BONDTrade &bt = (*this)();

		return( bt._trdPrc - bt._netChg ); 
	}
#ifdef TODO
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
#endif // TODO


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
		::BONDTrade &bt = (*this)();
		char         buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "%ld @ %8.6f", bt._trdVol, bt._trdPrc );
		cp += sprintf( cp, " {%s}\n", bt._mktCtr );
		cp += sprintf( cp, "   _high = %.6f; _low=%.6f\n", bt._high, bt._low );
		cp += sprintf( cp, "   _netChg = %.6f; ", bt._netChg );
		cp += sprintf( cp, "_pctChg = %.6f\n", bt._pctChg );
		cp += sprintf( cp, "   _tnOvr = %ld; ", bt._tnOvr );
		cp += sprintf( cp, "_acVol = %ld\n", bt._acVol );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class BONDTrade

} // namespace QUODD

#endif // __QUODD_BONDTrade_H 
