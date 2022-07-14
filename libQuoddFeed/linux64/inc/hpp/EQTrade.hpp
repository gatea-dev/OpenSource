/******************************************************************************
*
*  EQTrade.hpp
*     QuoddFeed EQTrade Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     27 JAN 2015 jcs  Build 11: IsCancel() / IsSummary()
*     17 MAR 2015 jcs  Build 14: Dump()
*      7 MAY 2015 jcs  Build 17: Extended Trade Fields
*
*  (c) 1994-2015 Gatea Ltd.
*******************************************************************************/
#ifndef __QUODD_EQTrade_H
#define __QUODD_EQTrade_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//         c l a s s   E Q T r a d e
//
////////////////////////////////////////////////

/**
 * \class EQTrade
 * \brief This class encapsulates an ::EQTrade (Equity Trade) message structure.
 *
 * A EQTrade object is driven into your application via the
 * Channel::OnUpdate( EQTrade & ) virtual method that is called
 * asynchronously when a Equity Trade message arrives from the UltraCache.
 */

class EQTrade : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::EQTrade struct
	 *
	 * \return ::EQTrade encapsulated by this object
	 */
	::EQTrade &operator()()
		// Returns reference to raw ::EQTrade data struct
	{
		return qm()._v._EQTrade;
	}

	/**
	 * \brief Returns yesterday's closing price
	 *
	 * \return Yesterday's closing price
	 */
	double PrevClose()
	{
		::EQTrade &tr = (*this)();

		return( tr._trdPrc - tr._netChg );
	}

	/**
	 * \brief Returns trade direction - UP, DOWN, UNCHANGED
	 *
	 * \return Trade direction - UP, DOWN, UNCHANGED
	 */
	const char *prcTck()
	{
		switch( (*this)()._prcTck ) {
			case '^': return "UP";
			case 'v': return "DOWN";
			case '-': return "UNCHANGED";
		}
		return "?";
	}

	/**
	 * \brief Returns extended trade direction - UP, DOWN, UNCHANGED
	 *
	 * \return Extended Trade direction - UP, DOWN, UNCHANGED
	 */
	const char *prcTck_ext()
	{
		switch( (*this)()._prcTck_ext ) {
			case '^': return "UP";
			case 'v': return "DOWN";
			case '-': return "UNCHANGED";
		}
		return "?";
	}

	bool HasExtendedFields()
	{
		return (*this)()._bExtFlds ? true : false;
	}

	/**
	 * \brief Returns true if this is an eligible trade
	 *
	 * \return true if this is an eligible trade
	 */
	bool IsEligible()
	{
		return ULTRAFEED::EquitySpec::IsEligible( (*this)(), false );
	}

	/**
	 * \brief Returns true if this is an eligible extended trade
	 *
	 * \return true if this is an eligible trade
	 */
	bool IsEligibleExtended()
	{
		return ULTRAFEED::EquitySpec::IsEligible( (*this)(), true );
	}


	/**
	 * \brief  Returns textual description of the Eligibility Flags
	 *
	 * \return  Textual description of the Eligibility Flags
	 */
	const char *EligibilityFlags()
		// Returns textual description of the Eligibility Flags
	{
		return ULTRAFEED::EquitySpec::EligibilityFlags( (*this)(), _rtn );
	}

	/**
	 * \brief  Returns textual description of the Settlement Type
	 *
	 * \return  Textual description of the Settlement Type
	 */
	const char *SettlementType()
	{
		return ULTRAFEED::EquitySpec::SettlementType( (*this)() );
	}

	/**
	 * \brief  Returns textual description of the Report Type
	 *
	 * \return  Textual description of the Report Type
	 */
	const char *ReportType()
	{
		return ULTRAFEED::EquitySpec::ReportType( (*this)() );
	}

	/**
	 * \brief  Returns textual description of the Report Detail
	 *
	 * \return  Textual description of the Report Detail
	 */
	const char *ReportDetail()
	{
		return ULTRAFEED::EquitySpec::ReportDetail( (*this)() );
	}

	/**
	 * \brief  Returns textual description of the Report Flags
	 *
	 * \return  Textual description of the Report Flags
	 */
	const char *ReportFlags()
	{
		return ULTRAFEED::EquitySpec::ReportFlags( (*this)(), _rtn );
	}

	/**
	 * \brief  Returns true if this is a Cancel Trade
	 *
	 * \return  true if Cancel Trade
	 */
	bool IsCancel()
	{
		return ULTRAFEED::EquitySpec::IsCancel( (*this)() );
	}

	/**
	 * \brief  Returns true if this is a Summary
	 *
	 * \return  true if Summary
	 */
	bool IsSummary()
	{
		return ULTRAFEED::EquitySpec::IsSummary( (*this)() );
	}

	/**
	 * \brief  Returns true if Odd Lot trade
	 *
	 * \return  true if Odd Lot trade
	 */
	bool IsOddLot()
	{
		return ULTRAFEED::EquitySpec::IsOddLot( (*this)() );
	}

	/**
	 * \brief  Returns true if extended hours trade
	 *
	 * \return  true if extended hours trade
	 */
	bool IsExtended()
	{
		return ULTRAFEED::EquitySpec::IsExtended( (*this)() );
	}

	/**
	 * \brief  Returns true if out of sequence trade
	 *
	 * \return  true if out of sequence trade
	 */
	bool IsOutOfSeq()
	{
		return ULTRAFEED::EquitySpec::IsOutOfSeq( (*this)() );
	}

	/**
	 * \brief  Returns true if trade thru
	 *
	 * \return true if trade thru
	 */
	bool IsTradeThru()
	{
		return ULTRAFEED::EquitySpec::IsTradeThru( (*this)() );
	}

	/**
	 * \brief  Returns true if held trade
	 *
	 * \return true if held trade
	 */
	bool IsHeldTrade()
	{
		return ULTRAFEED::EquitySpec::IsHeldTrade( (*this)() );
	}

	/**
	 * \brief  Returns true if Late trade
	 *
	 * \return true if Late trade
	 */
	bool IsLateTrade()
	{
		return ULTRAFEED::EquitySpec::IsLateTrade( (*this)() );
	}

	/**
	 * \brief  Returns true if error in trade
	 *
	 * \return true if error in trade
	 */
	bool IsError()
	{
		return ULTRAFEED::EquitySpec::IsError( (*this)() );
	}

	/**
	 * \brief  Returns true if Yellow Flag trade
	 *
	 * \return true if Yellow Flag trade
	 */
	bool IsYellowFlag()
	{
		return ULTRAFEED::EquitySpec::IsYellowFlag( (*this)() );
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
		::EQTrade  &tr = (*this)();
		char        buf[4*K], *cp;
		bool        bElig;
		size_t      pos;
		double      prc, net, pct;
		long        vol;
		std::string s, sm( MsgType() );
		const char *pt, *pm, *pc;

		// Header

		Message::Dump();

		// Processing ...

		bElig = IsEligible();
		pt    = prcTck();
		s     = MsgType();
		s    += !bElig ? "-InEligible" : "";
		s    += IsCancel() ? "-Cancel" : "";
		s    += IsSummary() ? "-Summary" : "";
		pm    = s.data();
		if ( (pos=dump_.find( sm.data() )) != dump_.npos )
			dump_.replace( pos, sm.length(), pm );

		// Guts

		cp  = buf;
		*cp = '\0';
		prc = tr._trdPrc;
		vol = tr._trdVol;
		net = tr._netChg;
		pct = tr._pctChg;
		cp += sprintf( cp, "(%s) %ld @ %8.4f", pt, vol, prc );
		cp += sprintf( cp, " {%s}; ID=%ld\n", tr._mktCtr, tr._trdID );
		cp += sprintf( cp, "   _openPrc = %.4f; ", tr._openPrc );
		cp += sprintf( cp, "_openVol = %ld\n", tr._openVol );
		cp += sprintf( cp, "   _high = %.4f; _low=%.4f\n", tr._high, tr._low );
		cp += sprintf( cp, "   _netChg = %.4f; _pctChg = %.4f\n", net, pct );
		cp += sprintf( cp, "   _tnOvr = %ld; ", tr._tnOvr );
		cp += sprintf( cp, "_acVol = %ld; ", tr._acVol );
		cp += sprintf( cp, "_vwap = %.4f\n", tr._vwap );
		cp += sprintf( cp, "   IsEligible?  = %s\n", bElig ? "YES" : "NO" );
		cp += sprintf( cp, "   EligFlags    = %s\n", EligibilityFlags() );
		cp += sprintf( cp, "   SettleType   = %s\n", SettlementType() );
		cp += sprintf( cp, "   ReportType   = %s\n", ReportType() );
		cp += sprintf( cp, "   ReportDetail = %s\n", ReportDetail() );
		cp += sprintf( cp, "   ReportFlags  = %s\n", ReportFlags() );
/*
		cp += sprintf( cp, "   Protocol     = %s\n", tr.Protocol() );
		cp += sprintf( cp, "   ChanIdx      = %s\n", tr.ChanIdx() );
		cp += sprintf( cp, "   Trade Time   = %s\n", pt );
 */
      // Extended Fields

		if ( HasExtendedFields() ) {
			pt  = prcTck_ext();
			pc  = tr._mktCtr_ext;
			prc = tr._trdPrc_ext;
			vol = tr._trdVol_ext;
			net = tr._netChg_ext;
			pct = tr._pctChg_ext;
			cp += sprintf( cp, "   *** Extended Trade Fields ***\n" );
			cp += sprintf( cp, "   (%s) %ld @ %8.4f {%s]n", pt, vol, prc, pc );
			cp += sprintf( cp, "   _net = %.4f; _pct = %.4f\n", net, pct );
		}

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class EQTrade

} // namespace QUODD

#endif // __QUODD_EQTrade_H 
