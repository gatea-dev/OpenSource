/******************************************************************************
*
*  EquitySpec.hpp
*     UltraFeed Equity Appendices
*
*  REVISION HISTORY:
*      5 SEP 2014 jcs  Created (from QuoddFeed.Enum.EquitySpec.java)
*     15 DEC 2014 jcs  Build 11: EQLimitUpDn
*     28 JAN 2015 jcs  Build 12: IsSummary() / IsCancel()
*      7 MAY 2015 jcs  Build 17: IsEligible( bool )
*
*  (c) 1994-2015 Gatea Ltd.
*******************************************************************************/
#ifndef __ULTRA_EQUITY_SPEC_H
#define __ULTRA_EQUITY_SPEC_H
#include <Messages.h>
#include <hpp/Enum/UltraFeedSpec.hpp>

namespace ULTRAFEED
{
/*
 * Appendix C - Quote Condition Flags
 */
static int QTE_OPEN          = 0x0002;
// static int QTE_BID_PROTECTED = 0x0004;
// static int QTE_ASK_PROTECTED = 0x0008;
static int QTE_BID_UNPRICED  = 0x0010;
static int QTE_ASK_UNPRICED  = 0x0020;
static int QTE_BID_VALID     = 0x0040;
static int QTE_ASK_VALID     = 0x0080;
/*
 * Appendix D - Eligibility Flags
 */
static int eligibleConsVolume   = 0x0001;
static int eligibleConsLast     = 0x0002;
static int eligibleConsHighLow  = 0x0004;
static int eligiblePartVolume   = 0x0008;
static int eligiblePartLast     = 0x0010;
static int eligiblePartHighLow  = 0x0020;
static int eligiblePartOpen     = 0x0040;
static int eligiblePartClose    = 0x0080;
static int changedConsLast      = 0x0100;
static int changedConsLow       = 0x0200;
static int changedConsHigh      = 0x0400;
static int changedPartLast      = 0x0800;
static int changedPartLow       = 0x1000;
static int changedPartHigh      = 0x2000;
static int changedPartOpen      = 0x4000;
/*
 * Appendix G - Report Flags
 */
static int TRD_ODDLOT     = 0x0001;
static int TRD_EXTENDED   = 0x0002;
static int TRD_OUTOFSEQ   = 0x0004;
static int TRD_TRADETHRU  = 0x0008;
static int TRD_HELDTRADE  = 0x0010;
static int TRD_LATETRADE  = 0x0030;
static int TRD_ERROR      = 0x0040;
static int TRD_YELLOWFLAG = 0x0080;
/*
 * UltraCache Formatting - Limit Up / Down
 */
// static int LULD_NOCOND      = 0x0000;
static int LULD_EXECUTABLE  = 0x0001;
static int LULD_BID_NONEX   = 0x0002;
static int LULD_ASK_NONEX   = 0x0004;
static int LULD_BID_LIMIT   = 0x0008;
static int LULD_ASK_LIMIT   = 0x0010;
static int R_LULD_BID_NONEX = 0x0400;
static int R_LULD_ASK_NONEX = 0x0800;
static int R_LULD_BID_LIMIT = 0x1000;
static int R_LULD_ASK_LIMIT = 0x2000;


/////////////////////////////////////////////////////////////////
//
//               c l a s s    E q u i t y S p e c
//
/////////////////////////////////////////////////////////////////

/**
 * \class EquitySpec
 * \brief This class encapsulates the UltraFeed Equity appendices.
 *
 * You typically do not call this class directly.  Higher-order 
 * classes such as EQTrade use the services of EquitySpec.
 */
class EquitySpec : public UltraFeedSpec
{
	//////////////////////////////////
	//
	//       E Q T r a d e
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns comma-separated list of Eligibility Flags
	 *
	 * An EQTrade calls this method to get the list from UltraFeed 
	 * Equity Specification Appendix D.
	 *
	 * \param trd - EQTrade with eligibility flags to interrogate
	 * \param rtn - string to hold return value 
	 * \return Comma-separated list of Eligibility Flags
	 */
	static const char *EligibilityFlags( EQTrade &trd, std::string &rtn )
	{
		std::vector<const char *> v;
		int                       i, n, elg;

		// Bitmask

		elg = trd._eligFlags;
		if ( _IsSet( elg, eligibleConsVolume ) )
			v.push_back( "ConsVolume" );
		if ( _IsSet( elg, eligibleConsLast ) )
			v.push_back( "ConsLast" );
		if ( _IsSet( elg, eligibleConsHighLow ) )
			v.push_back( "ConsHighLow" );
		if ( _IsSet( elg, eligiblePartVolume ) )
			v.push_back( "PartVolume" );
		if ( _IsSet( elg, eligiblePartLast ) )
			v.push_back( "PartLast" );
		if ( _IsSet( elg, eligiblePartHighLow ) )
			v.push_back( "PartHighLow" );
		if ( _IsSet( elg, eligiblePartOpen ) )
			v.push_back( "PartOpen" );
		if ( _IsSet( elg, eligiblePartClose ) )
			v.push_back( "PartClose" );
		if ( _IsSet( elg, changedConsLast ) )
			v.push_back( "changedConsLast" );
		if ( _IsSet( elg, changedConsLow ) )
			v.push_back( "changedConsLow" );
		if ( _IsSet( elg, changedConsHigh ) )
			v.push_back( "changedConsHigh" );
		if ( _IsSet( elg, changedPartLast ) )
			v.push_back( "changedPartLast" );
		if ( _IsSet( elg, changedPartLow ) )
			v.push_back( "changedPartLow" );
		if ( _IsSet( elg, changedPartHigh ) )
			v.push_back( "changedPartHigh" );
		if ( _IsSet( elg, changedPartOpen ) )
			v.push_back( "changedPartOpen" );
		n   = (int)v.size();
		rtn = "";
		for ( i=0; i<n; i++ ) {
			rtn += ( i>0 ) ? "," : "";
			rtn += v[i];
		}
		return rtn.c_str();
	}

	/**
	 * \brief Returns textual description of Settlement Type
	 *
	 * An EQTrade calls this method to get the description from UltraFeed 
	 * Equity Specification Appendix G.
	 *
	 * \param trd - EQTrade with settlement type flags to interrogate
	 * \return Textual description of Settlement Type
	 */
	static const char *SettlementType( EQTrade &trd )
	{
		if ( IsCanadian( trd._hdr ) ) {
			switch( trd._setlType ) {
				case 1: return "Regular";
				case 2: return "Cash";
				case 3: return "NonNet";
				case 4: return "MS";
				case 5: return "CashToday";
				case 6: return "DelayedDelivery";
			}
		}         
		else {    
			switch( trd._setlType ) {
				case 1: return "Regular";
				case 2: return "Cash";
				case 3: return "NextDay";
				case 4: return "Seller";
			}    
		}
		return "Undefined";
	}

	/**
	 * \brief Returns textual description of Report Type
	 *
	 * An EQTrade calls this method to get the description from UltraFeed 
	 * Equity Specification Appendix G.
	 *
	 * \param trd - EQTrade with report type to interrogate
	 * \return Textual description of Report Type
	 */
	static const char *ReportType( EQTrade &trd )
	{
		if ( IsCanadian( trd._hdr ) ) {
			switch( trd._rptType ) {
				case 1: return "Regular";
				case 2: return "Opening";
				case 3: return "InternalCross";
				case 4: return "Basis";
				case 5: return "Contingent";
				case 6: return "STS";
				case 7: return "VWAP";
			}
		}
		else {
			switch( trd._rptType ) {
				case 1: return "Regular";
				case 2: return "Opening";
				case 3: return "Closing";
				case 4: return "Reopening";
				case 5: return "ISO";
				case 6: return "DerivitivelyPriced";
			}
		}
		return "";
	}

	/**
	 * \brief Returns textual description of Report Detail
	 *
	 * An EQTrade calls this method to get the description from UltraFeed 
	 * Equity Specification Appendix G.
	 *
	 * \param trd - EQTrade with report type to interrogate
	 * \return Textual description of Report Detail
	 */
	static const char *ReportDetail( EQTrade &trd )
	{
		if ( IsCanadian( trd._hdr ) ) {
			switch( trd._rptDetail ) {
				case  0: return "None";
				case  1: return "Cross";
			}
		}
		else {
			switch( trd._rptDetail ) {
				case  0: return "None";
				case  1: return "Cross";
				case  2: return "OfficialOpen";
				case  3: return "OfficialClose";
				case  4: return "Acquisition";
				case  5: return "Bunched";
				case  6: return "Distribution";
				case  7: return "PriceVariation";
				case  8: return "CAPElection";
				case  9: return "AveragePrice";
				case 10: return "AutomaticExecution";
				case 11: return "PriorReferencePrice";
				case 12: return "Rule155";
				case 13: return "OptionTrade";
				case 14: return "SplitTrade";
				case 15: return "StoppedStock";
			}
		}
		return "Undefined";
	}

	/**
	 * \brief Returns comma-separated list of Report Flags
	 *
	 * An EQTrade calls this method to get the list from UltraFeed 
	 * Equity Specification Appendix G.
	 *
	 * \param trd - EQTrade with report type to interrogate
	 * \param rtn - string to hold return value 
	 * \return Comma-separated list of Report Flags
	 */
	static const char *ReportFlags( EQTrade &trd, std::string &rtn )
	{
		std::vector<const char *> v;
		int                       i, n, rpt;

		// Bitmask

		rpt = trd._rptFlags;
		if ( IsCanadian( trd._hdr ) ) {
			if ( IsOddLot( trd ) )       v.push_back( "OddLot" );
			if ( IsExtended( trd ) )     v.push_back( "ExtendedHours" );
			if ( IsOutOfSeq( trd ) )     v.push_back( "OutOfSequence" );
			if ( IsTradeThru( trd ) )    v.push_back( "ByPass" );
			if ( IsHeldTrade( trd ) )    v.push_back( "MOC" );
			if ( IsLateTrade( trd ) )    v.push_back( "Basket" );
			if ( IsError( trd ) )        v.push_back( "Jitney" );
			if ( IsYellowFlag( trd ) )   v.push_back( "NonResident" );
			if ( _IsSet( rpt, 0x0100 ) ) v.push_back( "Principal" );
			if ( _IsSet( rpt, 0x0200 ) ) v.push_back( "Wash" );
			if ( _IsSet( rpt, 0x0400 ) ) v.push_back( "Anonymous" );
			if ( _IsSet( rpt, 0x0800 ) ) v.push_back( "SpecialTerms" );
		}
		else {
			if ( IsOddLot( trd ) )     v.push_back( "OddLot" );
			if ( IsExtended( trd ) )   v.push_back( "ExtendedHours" );
			if ( IsOutOfSeq( trd ) )   v.push_back( "OutOfSequence" );
			if ( IsTradeThru( trd ) )  v.push_back( "TradeThruExempt" );
			if ( IsHeldTrade( trd ) )  v.push_back( "HeldTrade" );
			if ( IsLateTrade( trd ) )  v.push_back( "Late" );
			if ( IsError( trd ) )      v.push_back( "Error" );
			if ( IsYellowFlag( trd ) ) v.push_back( "YellowFlag" );
		}
		n   = (int)v.size();
		rtn = "";
		for ( i=0; i<n; i++ ) {
			rtn += ( i>0 ) ? "," : "";
			rtn += v[i];
		}
		return rtn.c_str();
	}

	/**
	 * \brief Returns true if EQTrade is a Summary Message
	 *
	 * \param trd - EQTrade with message type to interrogate
	 * \return true if EQTrade is Summary Message
	 */
	static bool IsSummary( EQTrade &trd )
	{
		u_char mt2;

		mt2 = trd._hdr._mtSub;
		return( mt2 == _eqSubSUMMARY ); 
	}

	/**
	 * \brief Returns true if EQTrade is a Cancel Message
	 *
	 * \param trd - EQTrade with message type to interrogate
	 * \return true if EQTrade is Cancel Message
	 */
	static bool IsCancel( EQTrade &trd )
	{
		u_char mt2;

		mt2 = trd._hdr._mtSub;
		return( mt2 == _eqSubTRDCXL ); 
	}

	/**
	 * \brief Returns true if EQTrade is an odd lot
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is odd lot
	 */
	static bool IsOddLot( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_ODDLOT );
	}

	/**
	 * \brief Returns true if EQTrade is an extended trade
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is extended trade
	 */
	static bool IsExtended( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_EXTENDED );
	}

	/**
	 * \brief Returns true if EQTrade is an out of sequence
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is out of sequence
	 */
	static bool IsOutOfSeq( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_OUTOFSEQ );
	}

	/**
	 * \brief Returns true if EQTrade is an trade thru
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is trade thru
	 */
	static bool IsTradeThru( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_TRADETHRU );
	}

	/**
	 * \brief Returns true if EQTrade is an held trade
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is held trade
	 */
	static bool IsHeldTrade( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_HELDTRADE );
	}

	/**
	 * \brief Returns true if EQTrade is an late trade
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is late trade
	 */
	static bool IsLateTrade( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_LATETRADE );
	}

	/**
	 * \brief Returns true if EQTrade is an error
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is error
	 */
	static bool IsError( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_ERROR );
	}

	/**
	 * \brief Returns true if EQTrade is an yellow flag
	 *
	 * \param trd - EQTrade with report flags to interrogate
	 * \return true if EQTrade is yellow flag
	 */
	static bool IsYellowFlag( EQTrade &trd )
	{
		return _IsSet( trd._rptFlags, TRD_YELLOWFLAG );
	}

	/**
	 * \brief Returns textual description of the tier
	 *
	 * \param img - Image with tier to interrogate
	 * \return Textual description of the tier
	 */
	static const char *Tier( Image &img )
	{
		if ( IsEquity( img._hdr ) ) {
			switch( img._tier ) {
				case  0: return "No Tier";
				case  1: return "OTCQX U.S. Premier";
				case  2: return "OTCQX U.S.";
				case  5: return "OTCQX International Premier";
				case  6: return "OTCQX International";
				case 10: return "OTCQB";
				case 11: return "OTCBB Only";
				case 20: return "OTC Pink Current";
				case 21: return "OTC Pink Limited";
				case 22: return "OTC Pink No Information";
				case 30: return "Grey Market";
				case 50: return "OTC Yellow";
				case 51: return "OTC Bonds";
			}
		}
		return "";
	}

	/**
	 * \brief Returns textual description of market category
	 *
	 * \param img - Image with market category to interrogate
	 * \return Textual description of the market category
	 */
	static const char *MktCategory( Image &img )
	{
		switch( img._mktCat ) {
			case 'N': return "NYSE";
			case 'A': return "NYSE Markets (AMEX)";
			case 'P': return "NYSE Arca";
			case 'Q': return "NASDAQ Global Select";
			case 'G': return "NASDAQ Global";
			case 'S': return "NASDAQ Capital";
			case 'Z': return "BATS";
			case ' ': return "Not Available";
		}
		return "";
	}


	/////////////////////////
	// Angie / Chin Logic
	/////////////////////////
	/**
	 * \brief Returns true if EQTrade is eligible to update price and volume
	 *
	 * A combination of the EQTrade._setlType, EQTrade._rptType, and
	 * EQTrade._rptDetail determine if the trade is eligible to update 
	 * the last price, and last trade volume.  
	 *
	 * \param trd - EQTrade to interrogate
	 * \param bExt - True if extended trade
	 * \return true if eligible
	 */
	static bool IsEligible( EQTrade &trd, bool bExt )
	{
		bool bInElig;

		/*
		 * 12-07-26 jcs  Build 34: From Angie's markup of Appx G w/ Red Pen
		 */

		// 1) Settlement Type

		bInElig = false;
		switch( trd._setlType ) {
			case 2:  // Cash
			case 3:  // NextDay
			case 4:  // Seller
				bInElig = true;
				break;
		}

		// 2) Report Type

		switch( trd._rptType ) {
			case 6:  // Derivatively Prices
				bInElig = true;
				break;
		}

		// 3) Report Detail

		switch( trd._rptDetail ) {
			case  2: // Official Open
			case  3: // Official Close
			case  7: // Price Variation
			case  9: // Average Price
			case 11: // Prior Reference Price
				bInElig = true;
				break;
		}

		// 4) Report Flags

		bInElig |= IsOddLot( trd );
		bInElig |= IsOutOfSeq( trd );
		/*
		 * 15-05-07 jcs  Build 95: From Angie / Chin E-mail:
		 *    "We no longer want to filter the EQ_RPT_EXTENDED trade contidion
		 *     for extended fields; Those are the trades we need to process but
		 *     all other 'IsEligible' logic stays in place."
		 */
		if ( !bExt )
			bInElig |= IsExtended( trd );
		return !bInElig;
	}


	//////////////////////////////////
	//
	//       E Q Q u o t e
	//
	//////////////////////////////////

	/**
	 * \brief Returns true if the quote is open
	 *
	 * An Image calls this method to interrogate the quote conditions 
	 * based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param img - Image with quote flags to interrogate
	 * \return true if Open
	 */
	static bool IsOpen( Image &img )
	{
		return _IsOpen( img._qteFlags );
	}

	/**
	 * \brief Returns true if the quote is open
	 *
	 * An EQQuoteMM calls this method to interrogate the quote conditions 
	 * based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param qte - EQQuoteMM with quote flags to interrogate
	 * \return true if Open
	 */
	static bool IsOpen( EQQuoteMM &qte )
	{
		return _IsOpen( qte._flags );
	}

	/**
	 * \brief Returns true if we can display BID
	 *
	 * An Image calls this method to interrogate the quote flags  
	 * based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param img - Image with quote flags to interrogate
	 * \return true if we can display BID
	 */
	static bool CanDisplayBid( Image &img )
	{
		int flg = img._qteFlags;

		return _IsOpen( flg ) && _BidIsPriced( flg ); // && _IsValidBid( flg );
	}

	/**
	 * \brief Returns true we can display BID
	 *
	 * An EQQuoteMM calls this method to interrogate the quote flags  
	 * based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param qte - EQQuoteMM with quote flags to interrogate
	 * \return true if we can display BID
	 */
	static bool CanDisplayBid( EQQuoteMM &qte ) 
	{
		int flg = qte._flags;

		return _IsOpen( flg ) && _BidIsPriced( flg ); // && _IsValidBid( flg );
	}

	/**
	 * \brief Returns true if we can display ASK
	 *
	 * An Image calls this method to interrogate the quote flags  
	 * based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param img - Image with quote flags to interrogate
	 * \return true if we can display ASK
	 */
	static bool CanDisplayAsk( Image &img )
	{
		int flg = img._qteFlags;

		return _IsOpen( flg ) && _AskIsPriced( flg ); // && _IsValidAsk( flg );
	}

	/**
	 * \brief Returns true we can display ASK
	 *
	 * An EQQuoteMM calls this method to interrogate the quote flags  
	 * based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param qte - EQQuoteMM with quote flags to interrogate
	 * \return true if we can display ASK
	 */
	static bool CanDisplayAsk( EQQuoteMM &qte )
	{
		int flg = qte._flags;

		return _IsOpen( flg ) && _AskIsPriced( flg ); // && _IsValidAsk( flg );
	}

	/**
	 * \brief Returns true if the EQQuote is one-sided
	 *
	 * An EQQuote calls this method to determine if the quote is
	 * one-sided based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param qte - EQQuote to interrogate
	 * \return true if we can display ASK
	 */
	static bool IsOneSided( EQQuote &qte )
	{
		return IsCachedBid( qte ) || IsCachedAsk( qte );
	}

	/**
	 * \brief Returns true if the EQQuote is one-sided ASK (cached BID).
	 *
	 * An EQQuote calls this method to determine if the quote is
	 * one-sided ASK based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param qte - EQQuote to interrogate
	 * \return true if one-sided ASK
	 */
	static bool IsCachedBid( EQQuote &qte )
	{
		return !_IsSet( qte._flags, QTE_BID_VALID );
	}

	/**
	 * \brief Returns true if the EQQuote is one-sided BID (cached ASK).
	 *
	 * An EQQuote calls this method to determine if the quote is
	 * one-sided BID based on UltraFeed Equity Specification Appendix C.
	 *
	 * \param qte - EQQuote to interrogate
	 * \return true if one-sided BID
	 */
	static bool IsCachedAsk( EQQuote &qte )
	{
		return !_IsSet( qte._flags, QTE_ASK_VALID );
	}


	/////////////////////////
	// Helpers
	/////////////////////////
private:
	static bool _IsOpen( int flags )
	{
		return _IsSet( flags, QTE_OPEN );
	}

	static bool _BidIsPriced( int flags )
	{
		return !_IsSet( flags, QTE_BID_UNPRICED );
	}

	static bool _AskIsPriced( int flags )
	{
		return !_IsSet( flags, QTE_ASK_UNPRICED );
	}

	static bool _IsValidBid( int flags )
	{
		return _IsSet( flags, QTE_BID_VALID );
	}

	static bool _IsValidAsk( int flags )
	{
		return _IsSet( flags, QTE_ASK_VALID );
	}


	//////////////////////////////////
	//
	//         E Q B b o
	//
	//////////////////////////////////
public:
	/**
	 * \brief Returns the Limit Up / Limit Down description
	 *
	 * Call this funciton when the LULD indicator is in the
	 * EQBbo._luldFlags field.
	 *
	 * \param bbo - EQBbo to interrogate
	 * \return Limit Up / Down description in A thru G format
	 */
	static const char *LimitUpDown( EQBbo &bbo )
	{
		return _LimitUpDown( bbo._luldFlags );
	}

	/**
	 * \brief Returns the Limit Up / Limit Down description
	 *
	 * Call this funciton when the LULD indicator is in the
	 * EQBbo._flags field.
	 *
	 * \param bbo - EQBbo to interrogate
	 * \return Limit Up / Down description in A thru G format
	 */
	static const char *LimitUpDownR( EQBbo &bbo )
	{
		int flags;

		// Appx C

		flags = 0;
		if ( _IsSet( bbo._flags, R_LULD_BID_NONEX ) )
			flags |= LULD_BID_NONEX;
		if ( _IsSet( bbo._flags, R_LULD_BID_LIMIT ) )
			flags |= LULD_BID_LIMIT;
		if ( _IsSet( bbo._flags, R_LULD_ASK_NONEX ) )
			flags |= LULD_ASK_NONEX;
		if ( _IsSet( bbo._flags, R_LULD_ASK_LIMIT ) )
			flags |= LULD_ASK_LIMIT;
		if ( flags == 0 )
			flags = LULD_EXECUTABLE;
		return _LimitUpDown( flags );
	}

	/**
	 * \brief Returns the Limit Up / Limit Down description
	 *
	 * \param bbo - Image to interrogate
	 * \return Limit Up / Down description in A thru G format
	 */
	static const char *LimitUpDown( Image &img )
	{
		int flags;

		flags = (int)img._ETPSharesOut;
		return _LimitUpDown( flags );
	}


	//////////////////////////////////
	//
	//     E Q L i m i t U p D n
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns textual description of the Indicator Flag
	 *
	 * An EQLimitUpDn calls this method to get the text from UltraFeed 
	 * Equity Specification Appendix I.
	 *
	 * \param luld - EQLimitUpDn with indicator flag to interrogate
	 * \param rtn - string to hold return value 
	 * \return Textual description of the Indicator Flag
	 */
	static const char *Indicator( EQLimitUpDn &luld, std::string &rtn )
	{
		const char *pi;

		// Appendix I, pg 23 UltraFeed Equity Spec v1.2

		pi = "Undefined";
		switch( luld._indicator ) {
			case 0: pi = "No Indicator Provided"; break;
			case 1: pi = "Opening";               break;
			case 2: pi = "eIntra-Day";            break;
			case 3: pi = "Re-Stated";             break;
			case 4: pi = "Suspended";             break;
			case 5: pi = "Re-Opened";             break;
			case 6: pi = "Outside Rule Hours";    break;
		}
		rtn = pi;
		return rtn.c_str();
	}


	/////////////////////////
	// Helpers
	/////////////////////////
private:
	static const char *_LimitUpDown( int flags )
	{
		bool bExec, bBidLT, bBidEQ, bAskGT, bAskEQ;

		// Special case - BASIC
/*
 * Do we still use this??
 *
		if ( _bBasic )
			return " ";
 */

		// OK to process

		bExec  = _IsSet( flags, LULD_EXECUTABLE );
		bBidLT = _IsSet( flags, LULD_BID_NONEX );
		bBidEQ = _IsSet( flags, LULD_BID_LIMIT );
		bAskGT = _IsSet( flags, LULD_ASK_NONEX );
		bAskEQ = _IsSet( flags, LULD_ASK_LIMIT );
		if ( bExec )
			return "A";  // BBO Executable
		else if ( bBidLT ) {
			if ( bAskGT )
				return "D";  // BBO Non-Ex
			else if ( bAskEQ )
				return "H";  // ASK Limit / BID Non-Ex
			return "B";     // BID Non-Ex
		}
		else if ( bBidEQ ) {
			if ( bAskGT )
				return "G";  // BID Limit / ASK Non-Ex
			else if ( bAskEQ )
				return "I";  // BBO Limit
			return "E";     // BID Limit
		}
		else if ( bAskGT )
			return "C";     // ASK Non-Ex
		else if ( bAskEQ )
			return "F";     // ASK Limit
		return " ";
	}

}; // class EquitySpec

} // namespace ULTRAFEED

#endif // __ULTRA_EQUITY_SPEC_H
