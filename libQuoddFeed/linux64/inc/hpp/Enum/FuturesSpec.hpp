/******************************************************************************
*
*  FuturesSpec.hpp
*     UltraFeed Futures Appendices
*
*  REVISION HISTORY:
*      5 SEP 2014 jcs  Created (from QuoddFeed.Enum.FuturesSpec.java)
*     19 JUL 2018 jcs  Build 23: Expiration()
*
*  (c) 1994-2018 Gatea Ltd.
*******************************************************************************/
#ifndef __ULTRA_FUTURES_SPEC_H
#define __ULTRA_FUTURES_SPEC_H
#include <Messages.h>
#include <hpp/Enum/UltraFeedSpec.hpp>

namespace ULTRAFEED
{
/*
 * Appx A - Quote Flags
 */
static int unpricedBid = 0x0001;
static int unpricedAsk = 0x0002;
/*
 * Appx B - Trade Flags
 */
static int tradeOutOfSequence         = 0x0001;
static int tradeNoVolume              = 0x0002;
static int tradeEstimatedVolume       = 0x0004;
static int tradeVolumeNotUpdated      = 0x0008;
static int tradeBeginnigOfGlobexEvent = 0x0010;
static int tradeEndOfGlobexEvent      = 0x0020;
static int midEvent                   = 0x0040;
static int openingTrade               = 0x0080;
static int priceCalculatedByGlobex    = 0x0100;

/////////////////////////////////////////////////////////////////
//
//               c l a s s    F u t u r e s S p e c
//
/////////////////////////////////////////////////////////////////

/**
 * \class FuturesSpec
 * \brief This class encapsulates the UltraFeed Futures appendices.
 *
 * You typically do not call this class directly.
 * Higher-order classes such as FUTRTrade use the services of FuturesSpec.
 */

class FuturesSpec : public UltraFeedSpec
{
	//////////////////////////////////
	//
	//     F U T R Q u o t e
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns comma-separated list of Quote Flags
	 *
	 * An FUTRQuote calls this method to get the list from UltraFeed
	 * Futures Specification Appendix A.
	 *
	 * \param qte - FUTRQuote with quote flags to interrogate
	 * \param rtn - string to hold return value
	 * \return Comma-separated list of Quote flag descriptions
	 */
	static const char *QuoteFlags( FUTRQuote &qte, std::string &rtn )
	{
	   std::vector<const char *> v;
	   int                       i, n, qFlg;

	   // Bitmask

	   qFlg = qte._qteFlags;
	   if ( _IsSet( qFlg, unpricedBid ) )
	      v.push_back( "unpricedBid" );
	   if ( _IsSet( qFlg, unpricedAsk ) )
	      v.push_back( "unpricedAsk" );
	   n   = (int)v.size();
	   rtn = "";
	   for ( i=0; i<n; i++ ) {
	      rtn += ( i>0 ) ? "," : "";
	      rtn += v[i];
	   }
	   return rtn.c_str();
	}

	/**
	 * \brief Returns Exceptional Indicator textual description
	 *
	 * An FUTRQuote calls this method to get the list from UltraFeed
	 * Futures Specification Appendix A.
	 *
	 * \param qte - FUTRQuote with exceptional indicator to interrogate
	 * \param rtn - string to hold return value
	 * \return Exceptional Indicator textual description
	 */
	static const char *ExceptionalIndicator( FUTRQuote &qte )
	{
	   switch( qte._excInd ) {
	      case 'A': return "Asset Allocation";
	      case 'B': return "Wholesale (Block) Trading";
	      case 'E': return "Exchange for Physical";
	      case 'F': return "Average price for five minute session";
	      case 'G': return "Against Actual";
	      case 'H': return "Match/Cross Trade";
	      case 'O': return "Average price for one minute session";
	      case 'P': return "Exchange for Physical";
	      case 'R': return "Exchange for Risk";
	      case 'S': return "Basis";
	      case 'U': return "Exchange for Option";
	      case 'W': return "Exchange for Swaps";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of volume indicator
	 *
	 * \param qte - FUTRQuote with volume indicator to interrogate
	 * \return Description of volume indicator
	 */
	static const char *VolumeIndicator( FUTRQuote &qte )
	{
	   switch( qte._volInd ) {
	      case ' ': return "Volume field not updated";
	      case 'A': return "Actual";
	      case 'E': return "Estimated";
	   }
	   return "";
	}


	//////////////////////////////////
	//
	//     F U T R T r a d e
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns comma-separated list of Trade Flags
	 *
	 * An FUTRTrade calls this method to get the list from UltraFeed
	 * Futures Specification Appendix A.
	 *
	 * \param trd - FUTRTrade with trade flags to interrogate
	 * \param rtn - string to hold return value
	 * \return Comma-separated list of Quote flag descriptions
	 */
	static const char *TradeFlags( FUTRTrade &trd, std::string &rtn )
	{
	   std::vector<const char *> v;
	   int                       i, n, tFlg;

	   // Bitmask

	   tFlg = trd._trdFlags;
	   if ( _IsSet( tFlg, tradeOutOfSequence ) )
	      v.push_back( "tradeOutOfSequence" );
	   if ( _IsSet( tFlg, tradeNoVolume ) )
	      v.push_back( "tradeNoVolume" );
	   if ( _IsSet( tFlg, tradeEstimatedVolume ) )
	      v.push_back( "tradeEstimatedVolume" );
	   if ( _IsSet( tFlg, tradeVolumeNotUpdated ) )
	      v.push_back( "tradeVolumeNotUpdated" );
	   if ( _IsSet( tFlg, tradeBeginnigOfGlobexEvent ) )
	      v.push_back( "tradeBeginnigOfGlobexEvent" );
	   if ( _IsSet( tFlg, tradeEndOfGlobexEvent ) )
	      v.push_back( "tradeEndOfGlobexEvent" );
	   if ( _IsSet( tFlg, midEvent ) )
	      v.push_back( "midEvent" );
	   if ( _IsSet( tFlg, openingTrade ) )
	      v.push_back( "openingTrade" );
	   if ( _IsSet( tFlg, priceCalculatedByGlobex ) )
	      v.push_back( "priceCalculatedByGlobex" );
	   n   = (int)v.size();
	   rtn = "";
	   for ( i=0; i<n; i++ ) {
	      rtn += ( i>0 ) ? "," : "";
	      rtn += v[i];
	   }
	   return rtn.c_str();
	}

	/**
	 * \brief Returns description of the sale condition
	 *
	 * \param trd - FUTRTrade to interrogate
	 * \return Description of sale condition
	 */
	static const char *SaleCondition( FUTRTrade &trd )
	{
	   switch( trd._trdCond ) {
	      case ' ': return "Normal";
	      case '?': return "Indeterminate";
	      case 'B': return "Blank out the associated price";
	      case 'C': return "Cabinet";
	      case 'D': return "Differential";
	      case 'E': return "Exchange for Physical";
	      case 'F': return "Fast";
	      case 'G': return "Exchange for Physical / Cross Trade";
	      case 'H': return "Hit";
	      case 'I': return "Implied";
	      case 'J': return "Large Order";
	      case 'K': return "Small Order";
	      case 'L': return "Late (Time may not be exact)";
	      case 'M': return "Match/Cross Trade";
	      case 'N': return "Nominal / Notional";
	      case 'O': return "Option Exercise";
	      case 'P': return "Percentage";
	      case 'Q': return "Auto Quotes";
	      case 'R': return "Indicative";
	      case 'S': return "Exchange for Swaps";
	      case 'T': return "Take";
	      case 'U': return "Exchange for Options";
	      case 'V': return "Nominal Cabinet";
	      case 'X': return "Changing Transaction";
	   }
	   return "";
	}


	//////////////////////////////////
	//
	//       e t c . . .
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * Returns futures Expiration Date as YYYYMMDD
	 *
	 * \return futures Expiration Date as YYYYMMDD
	 */
	static int Expiration( const char *tkr )
	{
	   char cm;
	   char mons[] = { 'F', 'G', 'H', 'J', 'K', 'M',
	                   'N', 'Q', 'U', 'V', 'X', 'Z' };
	   int  y, m, sz, rc;

	   // /ESU18

	   sz = strlen( tkr );
	   rc = 0;
	   if ( tkr[0] != 0x2f )
	      return 0;
	   if ( sz < 4 )
	      return 0;

	   // U : Sep = 9

	   cm = tkr[sz-3];
	   for ( m=0; m<12 && ( cm != mons[m] ); m++ );
	   m  += 1;
	   y   = tkr[sz-2] - '0';
	   y  *= 10;
	   y  += tkr[sz-1] - '0';
	   y  += 2000;
	   rc  = ( y * 10000 );
	   rc += ( m * 100 );
	   return rc;
	}

	/**
	 * \brief Returns description of market condition
	 *
	 * \param cnd - Market condition indicator
	 * \return Description of market condition
	 */
	static const char *MarketCondition( char ind )
	{
	   switch( ind ) {
	      case ' ':  return "Unspecified";
	      case 'A':  return "Halt Trading";
	      case 'B':  return "Resume Trading";
	      case 'C':  return "No Cancel";
	      case 'E':  return "End Fast Market";
	      case 'F':  return "Start Fast Market";
	      case 'I':  return "Price Indication";
	      case 'L':  return "Start Late Market (Time may not be exact)";
	      case 'M':  return "End Late Market";
	      case 'N':  return "Not Available";
	      case 'O':  return "Pre-Open";
	      case 'P':  return "Start Post Suspension/Close/Settle Session";
	      case 'Q':  return "End Post Suspension/Close/Settle Session";
	      case 'R':  return "Pre-cross";
	      case 'U':  return "Unknown";
	      case 'X':  return "Cross";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of market direction
	 *
	 * \param cnd - Market direction indicator
	 * \return Description of market direction
	 */
	static const char *MarketDirection( char ind )
	{
	   switch( ind ) {
	      case '+': return "Up";
	      case '-': return "Down";
	      case 'S': return "Stable";
	      case ' ': return "Market direction not indicated by Exchange";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of open interest
	 *
	 * \param ind - Open interest indicator
	 * \return Description of open interest
	 */
	static const char *OpenInterest( char ind )
	{
	   switch( ind ) {
	      case ' ': return "Open Interest Field Not Updated";
	      case 'A': return "Actual";
	      case 'E': return "Estimated";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of product classification
	 *
	 * \param ind - Product classification indicator
	 * \return Description of product classification
	 */
	static const char *ProductClassification( char ind )
	{   
	   switch( ind ) { 
	      case ' ': return "Unspecified";
	      case 'D': return "Delta Options";
	      case 'F': return "Flexible Options";
	      case 'I': return "Index Values";
	      case 'S': return "Short-dated Options";
	      case 'V': return "Volatility Options";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of range
	 *
	 * \param ind - Range indicator
	 * \return Description of range
	 */
	static const char *RangeIndicator( char ind )
	{
	   switch( ind ) {
	      case 'B': return "Indicative Bid/Ask with Delta";
	      case 'C': return "Close";
	      case 'D': return "Day Open";
	      case 'I': return "Indicative Open";
	      case 'O': return "Open";
	      case 'P': return "Post Close / Suspension";
	      case 'R': return "Resumption of Trading";
	      case 'S': return "Suspension of Trading";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of request indicator
	 *
	 * \param ind - Request indicator indicator
	 * \return Description of request indicator
	 */
	static const char *RequestIndicator( char ind )
	{
	   switch( ind ) {
	      case 'D': return "End Request for Quote";
	      case 'R': return "Start Request for Quote";
	   }
	   return "";
	}

	/**
	 * \brief Returns description of type code
	 *
	 * \param ind - Type code indicator
	 * \return Description of type code
	 */
	static const char *TypeCode( char ind )
	{
	   switch( ind ) {
	      case ' ': return "General Text";
	      case 'A': return "Trading Authorized in New Delivery Months";
	      case 'D': return "Deliveries and Intentions";
	      case 'E': return "Option Exercises and Assignments";
	      case 'F': return "Fix High Low";
	      case 'G': return "Good Morning ";
	      case 'I': return "Volume & Open Interest (Text format)";
	      case 'K': return "Lead Month Identification";
	      case 'L': return "Last Day of Trading in Delivery Months";
	      case 'M': return "Margin Changes";
	      case 'N': return "New Commodity Listings";
	      case 'P': return "Cash Prices";
	      case 'R': return "Receipts and Shipments";
	      case 'S': return "System Changes";
	      case 'T': return "Opening/Closing Time Changes, Permanent and Emergency";
	      case 'U': return "Crop Reports";
	      case 'V': return "Variable Limits Effective";
	      case 'W': return "Warehousing Information";
	      case 'X': return "Globex";
	      case 'Z': return "Flexible Options Text";
	   }
	   return "";
	}

}; // class FuturesSpec

} // namespace ULTRAFEED

#endif // __ULTRA_FUTURES_SPEC_H
