/******************************************************************************
*
*  OptionSpec.hpp
*     UltraFeed Option Appendices
*
*  REVISION HISTORY:
*      5 SEP 2014 jcs  Created (from QuoddFeed.Enum.OptionSpec.java)
*     15 MAR 2015 jcs  Build 14: Description()
*     21 APR 2016 jcs  Build 22: strtok() from copy - DUH
*     14 JUL 2022 jcs  Build 25: De-lint
*
*  (c) 2011-2022, Quodd Financial
*******************************************************************************/
#ifndef __ULTRA_OPTION_SPEC_H
#define __ULTRA_OPTION_SPEC_H
#include <Messages.h>
#include <hpp/Enum/UltraFeedSpec.hpp>

#ifdef WIN32
#define strtok_r(a,b,c) strtok(a,b)
#endif // WIN32

/*
 * Appx A - Quote Flags
 */
static int BestBidValid  = 0x0002;
static int BestAskValid  = 0x0004;
/*
 * Appx B - Trade / Trade Cxl Flags
 */
static int eligibleLast    = 0x0001;
static int eligibleVolume  = 0x0002;
static int late            = 0x0004;
static int outOfSequence   = 0x0008;
static int resumed         = 0x0010;

static const char  _sep[]  = { 0x5c, 0x00 };
static const char *_mons[] = { "JAN", "FEB", "MAR", "APR",
	                            "MAY", "JUN", "JUL", "AUG",
	                            "SEP", "OCT", "NOV", "DEC" };

namespace ULTRAFEED
{

/////////////////////////////////////////////////////////////////
//
//                c l a s s    O p t i o n S p e c
//
/////////////////////////////////////////////////////////////////

/**
 * \class OptionSpec
 * \brief This class encapsulates the UltraFeed Option appendices.
 *
 * Higher-order classes such as OPBbo use the services of OptionSpec.
 * You typically do not call this class directly.
 */

class OptionSpec : public UltraFeedSpec
{
	//////////////////////////////////
	//
	//         O P B B o
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns true if the OPBbo is one-sided
	 *
	 * An OPBbo calls this method to determine if the quote is
	 * one-sided based on UltraFeed Options Specification Appendix A.
	 *
	 * \param qte - OPBbo to interrogate
	 * \return true if we can display ASK
	 */
	static bool IsOneSided( OPBbo &bbo )
	{
	   return IsCachedBid( bbo ) || IsCachedAsk( bbo );
	}

	/**
	 * \brief Returns true if the OPBbo is one-sided ASK (cached BID).
	 *
	 * An OPBbo calls this method to determine if the quote is
	 * one-sided ASK based on UltraFeed Options Specification Appendix A.
	 *
	 * \param qte - OPBbo to interrogate
	 * \return true if one-sided ASK
	 */
	static bool IsCachedBid( OPBbo &bbo )
	{
	   return !_IsSet( bbo._qteFlags, BestBidValid );
	}

	/**
	 * \brief Returns true if the OPBbo is one-sided BID (cached ASK).
	 *
	 * An OPBbo calls this method to determine if the quote is
	 * one-sided BID based on UltraFeed Options Specification Appendix A.
	 *
	 * \param qte - OPBbo to interrogate
	 * \return true if one-sided BID
	 */
	static bool IsCachedAsk( OPBbo &bbo )
	{
	   return !_IsSet( bbo._qteFlags, BestAskValid );
	}


	//////////////////////////////////
	//
	//       O P T r a d e
	//
	//////////////////////////////////

	/////////////////////////
	// Class-wide
	/////////////////////////
public:
	/**
	 * \brief Returns comma-separated list of Trade Flags
	 *
	 * An OPTrade calls this method to get the list from UltraFeed
	 * Options Specification Appendix B.
	 *
	 * \param trd - OPTrade with trade flags to interrogate
	 * \param rtn - string to hold return value
	 * \return Comma-separated list of Quote flag descriptions
	 */
	static const char *TradeFlags( OPTrade &trd, std::string &rtn )
	{
	   std::vector<const char *> v;
	   int                       i, n, flg;

	   // Bitmask

	   flg = trd._trdFlags;
	   if ( PriceIsValid( trd ) )
	      v.push_back( "Last" );
	   if ( VolumeIsValid( trd ) )
	      v.push_back( "Volume" );
	   if ( _IsSet( flg, late ) )
	      v.push_back( "Late" );
	   if ( _IsSet( flg, outOfSequence ) )
	      v.push_back( "OutOfSequence" );
	   if ( _IsSet( flg, resumed ) )
	      v.push_back( "Resumed" );
	   n   = (int)v.size();
	   rtn = "";
	   for ( i=0; i<n; i++ ) {
	      rtn += ( i>0 ) ? "," : "";
	      rtn += v[i];
	   }
	   return rtn.c_str();
	}

	/**
	 * \brief Returns true if price in trade is valid
	 *
	 * \param trd - OPTrade with trade flags to interrogate
	 * \return true if price is valid
	 */
	static bool PriceIsValid( OPTrade &trd )
	{
	   return _IsSet( trd._trdFlags, eligibleLast );
	}

	/**
	 * \brief Returns true if volume in trade is valid
	 *
	 * \param trd - OPTrade with trade flags to interrogate
	 * \return true if volume is valid
	 */
	static bool VolumeIsValid( OPTrade &trd )
	{
	   return _IsSet( trd._trdFlags, eligibleVolume );
	}



	//////////////////////////////////
	//                                
	//       N a m i n g
	//
	//////////////////////////////////
	/**
	 * UltraCache AIG2 Naming : <CONTRACT_SIZE=100, ... >
	 */
	static const char *Description( const char *tkr, 
	                                const char *aig2,
	                                std::string &rtn )
	{
	   std::string s1, s2, s3, s, re1, re2, shr;
	   const char *root, *desc, *und;
	   char        ch, buf[K];
	   int         rSz, cSz;
	   size_t      pos;
	   const char *_U = "<U>";
	   const char *_C = "<C>";
	   const char *_S = "<S>";

	   /*
	    * 14-06-16 jcs  Build 78 : Contract Size from root.
	    *               From Hanji - AAPL7 / GOGL8 : Size = 10
	    */
	   root = OptionRoot( tkr, s1 );
	   rSz  = strlen( root );
	   ch   = root[rSz-1];
	   cSz  = 100;
	   switch( ch ) {
	      case '7':
	      case '8': cSz = 10; break;
	   }

	   /*
	    * 100<U>, 6.1<C>, 53<S>AIGWS ->
	    *    (100 shares of AIG, 6.1 cash, 53 shares of AIGWS)
	    */
	   desc = DescriptionFromName( tkr, s2 );
	   und  = OptionRoot( tkr, s3 );
	   for ( ch='0'; ch<'9'; ch++ )
	      for ( pos=0; (pos=s3.find( ch )) != s3.npos; s3.erase( pos,pos+1 ) );
	   und = s3.data();
	   shr  = " shares of ";
	   shr += und;
	   re1  = und;
	   re2  = und;
	   re1 += "WS";
	   re2 += "+";
	   s    = aig2;
	   if ( (pos=s.find( _U )) != s.npos )
	      s.replace( pos, strlen( _U ), shr );
	   if ( (pos=s.find( _C )) != s.npos )
	      s.replace( pos, strlen( _C ), " cash" );
	   if ( (pos=s.find( _S )) != s.npos )
	      s.replace( pos, strlen( _S ), " shares of " );
	   if ( (pos=s.find( re1 )) != s.npos )
	      s.replace( pos, re1.length(), re2 );
	   sprintf( buf, "%s (CONTRACT_SIZE=%d, %s)", desc, cSz, s.data() );
	   rtn = buf;
	   return rtn.data();
	}


	/**
	 * UltraCache Naming Convention : O:<root>\\...
	 */
	static const char *DescriptionFromName( const char *tkr, 
	                                        std::string &rtn )
	{
	   std::string s1, tmp( tkr );;
	   const char *und;
	   char       *p1, *p2, *rp;
	   char       *exp, *prc, y0, y1, mon;
	   char        buf[K];
	   const char *pc, *pm;
	   int         m;

	   /*
	    * O:AIG\14M18\32.00 -> AIG Jan 2014 32.00 PUT 
	    *    (The 18th day is included - Expires 18 Jan 2014)
	    */

	   und = OptionRoot( tkr, s1 );
	   rtn = und;
	   p1  = ::strtok_r( (char *)tmp.data(), _sep, &rp );
	   p2  = ::strtok_r( NULL, _sep, &rp );
	   if ( p2 ) {
	      exp = p1;
	      prc = p2;
	      y0  = exp[0];
	      y1  = exp[1];
	      mon = exp[2];
	      m   = ( mon - 'A' ) % 12;
	      pc  = InRange( 'A', mon, 'M' ) ?  "CALL" : "PUT";
	      pm  = _mons[m];
	      sprintf( buf, "%s %s 20%c%c %s %s", und, pm, y0,y1, prc, pc );
	      rtn = buf;
	   }
	   return rtn.data();
	}

	/**
	 * UltraCache Naming Convention : O:<root>\\...
	 */
	static const char *OptionRoot( const char *tkr, std::string &rtn )
	{
	   std::string tmp( tkr );
	   char       *p1, *rp;

	   // O:AIG\14M18\32.00 -> AIG

	   p1  = ::strtok_r( (char *)tmp.data(), _sep, &rp );
	   p1 += 2;
	   rtn = p1;
	   return rtn.data();
	}

	static int Expiration( const char *tkr )
	{
	   std::string tmp( tkr );
	   char       *p1, *p2, *p3, *rp;
	   int         y0, y1, d0, d1, m, rtn;

	   // O:AIG\14M18\32.00 -> 32.00

	   rtn = 0;
	   p1  = ::strtok_r( (char *)tmp.data(), _sep, &rp );
	   p2  = ::strtok_r( NULL, _sep, &rp );
	   p3  = ::strtok_r( NULL, _sep, &rp );
	   if ( !p1 )
	      ::Quodd_breakpoint();
	   if ( p3 ) {
	      y0   = p2[0] - '0';
	      y1   = p2[1] - '0';
	      m    = p2[2] - 'A';
	      d0   = p2[3] - '0';
	      d1   = p2[4] - '0';
	      m   %= 12;
	      rtn  = ( ( y0*10 ) + y1 );
	      rtn += 2000;
	      rtn *= 10000;
	      rtn += ( (m+1) * 100 );
	      rtn += ( ( d0*10 ) + d1 );
	   }
	   return rtn;
	}

	static double Strike( const char *tkr )
	{
	   std::string tmp( tkr );
	   char       *p1, *p2, *p3, *rp;

	   // O:AIG\14M18\32.00 -> 32.00

	   p1 = ::strtok_r( (char *)tmp.data(), _sep, &rp );
	   p2 = ::strtok_r( NULL, _sep, &rp );
	   p3 = ::strtok_r( NULL, _sep, &rp );
	   if ( !p1 || !p2 )
	      ::Quodd_breakpoint();
	   if ( p3 )
	      return atof( p3 );
	   return 0.0;
	}

	static const char *PutOrCall( const char *tkr )
	{
	   std::string tmp( tkr );
	   char       *p1, *p2, *p3, *rp, ch;

	   // O:AIG\14M18\32.00 -> PUT

	   p1 = ::strtok_r( (char *)tmp.data(), _sep, &rp );
	   p2 = ::strtok_r( NULL, _sep, &rp );
	   p3 = ::strtok_r( NULL, _sep, &rp );
	   if ( !p1 || !p2 )
	      ::Quodd_breakpoint();
	   if ( p3 ) {
	      ch = p3[2];
	      if ( InRange( 'A', ch, 'L' ) )
	         return "CALL";
	      if ( InRange( 'M', ch, 'X' ) )
	         return "PUT";
	   }
	   return "Undefined";
	}

}; // class OptionSpec

} // namespace ULTRAFEED

#endif// __ULTRA_OPTION_SPEC_H
