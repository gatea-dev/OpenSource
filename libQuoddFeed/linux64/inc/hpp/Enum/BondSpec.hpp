/******************************************************************************
*
*  BondSpec.hpp
*     UltraFeed Bond Appendices
*
*  REVISION HISTORY:
*     10 FEB 2016 jcs  Created (from QuoddFeed.Enum.BondSpec.java)
*
*  (c) 1994-2016 Gatea Ltd.
*******************************************************************************/
#ifndef __ULTRA_BOND_SPEC_H
#define __ULTRA_BOND_SPEC_H
#include <Messages.h>
#include <hpp/Enum/UltraFeedSpec.hpp>

namespace ULTRAFEED
{

/*
 * Appx A - Quote Flags
 */
static int QTE_BENCHMARK = 0x0001;
static int QTE_DELETED   = 0x0002;
static int QTE_NONTRADE  = 0x0004;
static int QTE_CLOSING   = 0x0008;
static int QTE_OPENING   = 0x0010;
static int QTE_SLOWBID   = 0x0020;
static int QTE_SLOWASK   = 0x0040;
static int QTE_LRP       = 0x0080;
static int QTE_FLATPRICE = 0x0100;
static int QTE_NONFIRM   = 0x0200;
static int QTE_SLOWLIST  = 0x0400;


/////////////////////////////////////////////////////////////////
//
//                c l a s s    B o n d S p e c
//
/////////////////////////////////////////////////////////////////

/**
 * \class BondSpec
 * \brief This class encapsulates the UltraFeed Bond appendices.
 *
 * You typically do not call this class directly.  Higher-order
 * classes such as BONDTrade and BONDQuote use the services of 
 * BondSpec.
 */
class BondSpec : public UltraFeedSpec
{
	//////////////////////////////////
	//
	//        B O N D Q u o t e
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns comma-separated list of Quote Flags
	 *
	 * An BONDQuote calls this method to get the list from UltraFeed
	 * Bond Specification Appendix A.
	 *
	 * \param qte - BONDQuote with quote conditions to interrogate
	 * \param rtn - string to hold return value
	 * \return Comma-separated list of Quote Flags
	 */
	static const char *QuoteFlags( ::BONDQuote &qte, std::string &rtn )
	{
		std::vector<const char *> v;
		int                       i, n, flg;

		// Bitmask

		if ( _IsSet( flg, QTE_BENCHMARK ) )
			v.push_back( "Benchmark" );
		if ( _IsSet( flg, QTE_DELETED ) )
			v.push_back( "Deleted" );
		if ( _IsSet( flg, QTE_NONTRADE ) )
			v.push_back( "NonTradable" );
		if ( _IsSet( flg, QTE_CLOSING ) )
			v.push_back( "Closing" );
		if ( _IsSet( flg, QTE_OPENING ) )
			v.push_back( "Opening" );
		if ( _IsSet( flg, QTE_SLOWBID ) )
			v.push_back( "SlowBid" );
		if ( _IsSet( flg, QTE_SLOWASK ) )
			v.push_back( "SlowAsk" );
		if ( _IsSet( flg, QTE_LRP ) )
			v.push_back( "LRP" );
		if ( _IsSet( flg, QTE_FLATPRICE ) )
			v.push_back( "FlatPricing" );
		if ( _IsSet( flg, QTE_NONFIRM ) )
			v.push_back( "NonFirm" );
		if ( _IsSet( flg, QTE_SLOWLIST ) )
			v.push_back( "SlowList" );
		n   = (int)v.size();
		rtn = "";
		for ( i=0; i<n; i++ ) {
			rtn += ( i>0 ) ? "," : "";
			rtn += v[i];
		}
		return rtn.data();
	}

	/**
	 * \brief Returns true if BONDTrade is a Cancel Message
	 *
	 * \param trd - BONDTrade with message type to interrogate
	 * \return true if BONDTrade is Cancel Message
	 */
	static bool IsCancel( BONDTrade &trd )
	{
		u_char mt2;

		mt2 = trd._hdr._mtSub;
		return( mt2 == _boSubTRDCXL );
	}

	/**
	 * \brief Returns true if BONDTrade is a Cancel Message
	 *
	 * \param trd - BONDTrade with message type to interrogate
	 * \return true if BONDTrade is Cancel Message
	 */
	static bool IsCorrection( BONDTrade &trd )
	{
		u_char mt2;

		mt2 = trd._hdr._mtSub;
		return( mt2 == _boSubTRDCORR );
	}

}; // class BondSpec

} // namespace ULTRAFEED

#endif // __ULTRA_BOND_SPEC_H
