/******************************************************************************
*
*  Image.hpp
*     QuoddFeed Image Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*     17 APR 2015 jcs  Build 16: TradeTimeMs()
*     28 JUL 2015 jcs  Build 19: Dump protocol num
*     19 JUL 2018 jcs  Build 23: FuturesExpiration()
*
*  (c) 1994-2018 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_Image_H
#define __QUODD_Image_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   I m a g e
//
////////////////////////////////////////////////

/**
 * \class Image
 * \brief This class encapsulates an ::Image (Initial Image) message structure.
 *
 * A Image object is driven into your application via the
 * Channel::OnImage( Image & ) virtual method that is called
 * asynchronously when a Index Value message arrives from the UltraCache.
 */

class Image : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::Image struct
	 *
	 * \return ::Image encapsulated by this object
	 */
	::Image &operator()()
	{
	   return qm()._v._Image;
	}

	/**
	 * \brief Returns true if this is an EQUITY ticker
	 * 
	 * \return true if this is an EQUITY ticker
	 */
	bool IsEquity()
	{
	   return( mt() == _mtEQUITY );
	}

	/**
	 * Returns true if this is an OPTION ticker
	 * 
	 * \return true if this is an OPTION ticker
	 */
	bool IsOption()
	{
	   return( mt() == _mtOPTION );
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
	   return (*this)()._close;
	}

	/**
	 * \brief Returns Volume Weighted Average Price
	 *
	 * \return Volume Weighted Average Price
	 */
	double vwap()
	{
	   ::Image &i = (*this)();
	   double   rtn;

	   rtn = i._acVol ? ( ( 1.0 * i._tnOvr ) / i._acVol ) : 0.0;
	   return rtn;
	}

	/**
	 * \brief Returns textual description of the Ticker Tier
	 *
	 * \return Textual description of the Ticker Tier
	 */
	const char *Tier()
	{
	   return ULTRAFEED::EquitySpec::Tier( (*this)() );
	}

	/**
	 * \brief Ticker description; For options, it is encoded in msg 
	 * name and (private) _desc fields.
	 *
	 * \return Ticker description
	 */
	const char *Description()
	{
	   ::Image    &i = (*this)();
	   const char *rtn;

	   rtn = i._desc;
	   if ( mt() == _mtOPTION )
	      rtn = ULTRAFEED::OptionSpec::Description( tkr(), i._desc, _desc );
	   return rtn;
	}

	/**
	 * \brief Returns textual description of the Market Category
	 *
	 * \return Textual description of the Market Category
	 */
	const char *MktCategory()
	{
	   return ULTRAFEED::EquitySpec::MktCategory( (*this)() );
	}

	/**
	 * \brief Returns textual Limit Up / Down Indicator
	 *
	 * \return textual Limit Up / Down Indicator
	 */
	const char *LimitUpDown()
	{
	   return ULTRAFEED::EquitySpec::LimitUpDown( (*this)() );
	}

	/**
	 * Returns Futures Expiration Date as YYYYMMDD
	 * 
	 * \return Futures Expiration Date as YYYYMMDD
	 */
	int FuturesExpiration()
	{
	   if ( mt() == _mtFUTURE )
	      return ULTRAFEED::FuturesSpec::Expiration( tkr() );
	   return 0;
	}

	/**
	 * Returns option Expiration Date as YYYYMMDD
	 * 
	 * \return option Expiration Date as YYYYMMDD
	 */
	int OptionExpiration()
	{
	   if ( IsOption() )
	      return ULTRAFEED::OptionSpec::Expiration( tkr() );
	   return 0;
	}

	/**
	 * Returns option strike price
	 * 
	 * \return Option strike price
	 */
	double OptionStrike()
	{
	   if ( IsOption() )
	      return ULTRAFEED::OptionSpec::Strike( tkr() );
	   return 0.0;
	}

	/**
	 * Returns option put/call type
	 * 
	 * \return Option put/call type
	 */
	const char *OptionPutOrCall()
	{
	   if ( IsOption() )
	      return ULTRAFEED::OptionSpec::PutOrCall( tkr() );
	   return "Undefined";
	}

	/**
	 * Returns option open interest
	 * 
	 * \return Option open interest
	 */
	long OptionOpenInterest()
	{
	   ::Image &i = (*this)();

	   return IsOption() ? i._openVol : 0;
	}

	/**
	 * \brief Returns textual desctiption of Mutual Fund Type
	 *
	 * \return Textual desctiption of Mutual Fund Type
	 */
	const char *FundType()
	{
	   ::Image &i = (*this)();

	   return ULTRAFEED::FundSpec::FundType( i._fundType );
	}

	/**
	 * \brief Returns textual desctiption of Mutual Fund Code
	 *
	 * \return Textual desctiption of Mutual Fund Code
	 */
	const char *FundCode()
	{
	   ::Image &i = (*this)();

	   return ULTRAFEED::FundSpec::FundCode( i._fundCode );
	}

	/**
	 * \brief Returns textual description of price direction
	 *
	 * \return Textual description of price direction
	 */
	const char *prcTck()
	{
	   ::Image &i = (*this)();

	   switch( i._prcTck ) {
	      case '^': return "UP";
	      case 'v': return "DOWN";
	      case '-': return "UNCHANGED";
	   }
	   return "?";
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
	   ::Image    &i = (*this)();
	   ::qHdr     &h = i._hdr;
	   std::string md, ut;
	   const char *pp;
	   char        buf[4*K], *cp;

	   // Header

	   Message::Dump();

	   // Guts

	   cp  = buf;
	   *cp = '\0';
	   ut  = TimeMs( i._tUpd );
	   pp  = ProtocolName();
	   cp += sprintf( cp,  "   Desc      = %s\n", Description() );
	   cp += sprintf( cp,  "   Protocol  = [%d] %s\n", protocol(), pp );
	   cp += sprintf( cp,  "   ChanIdx   = %d\n", i._nChan );
	   cp += sprintf( cp,  "   Exchange  = %s\n", i._priMktCtr );
	   cp += sprintf( cp,  "   RTL       = %d\n", h._RTL );
	   cp += sprintf( cp,  "   _tUpd     = %s\n", ut.data() );
	   cp += sprintf( cp,  "   _nChan    = %d\n", i._nChan );
	   cp += sprintf( cp,  "   _nUpd     = %d\n", i._nUpd );
	   cp += sprintf( cp,  "   _nTrade   = %d\n", i._nTrade );
	   cp += sprintf( cp,  "   _locCode  = %d\n", i._locCode );
	   cp += sprintf( cp,  "   _dbIdx    = %d\n", i._dbIdx );
	   if ( ( mt() == _mtEQUITY ) || ( mt() == _mtOPTION ) ) {
	      cp += sprintf( cp, "   _lotSize     = %d\n", i._lotSize );
	      cp += sprintf( cp, "   _qteFlags    = 0x%02x\n", i._qteFlags );
	      cp += sprintf( cp, "   _issueTy     = <%02x>\n", i._issueType );
	      cp += sprintf( cp, "   _finSts      = <%02x>\n", i._financialSts );
	      cp += sprintf( cp, "   _tier        = [%02d] %s\n", i._tier, Tier() );
/*
	      cp += sprintf( cp, "   _disclSts    = <%02x>\n", i._discloseSts );
	      cp += sprintf( cp, "   _assetCls    = <%02x>\n", i._assetClass );
	      cp += sprintf( cp, "   _authenti    = <%02x>\n", i._authenticity );
	      cp += sprintf( cp, "   _isShortR    = <%02x>\n", i._isShortSaleRestr );
	      cp += sprintf( cp, "   _isCaveat    = <%02x>\n", i._isCaveatEmptor );
	      cp += sprintf( cp, "   _TSEclass    = <%02x>\n", i._TSEclass );
	      cp += sprintf( cp, "   _regSHO      = <%02x>\n", i._regSHO );
	      cp += sprintf( cp, "   _unsolicited = <%02x>\n", i._unsolicited );
 */
	      cp += sprintf( cp, "   _halted      = <%02x>\n", i._halted );
	      cp += sprintf( cp, "   _BBquoted    = <%02x>\n", i._BBquoted );
/*
	      cp += sprintf( cp, "   _msgDisabled = <%02x>\n", i._msgDisabled );
	      cp += sprintf( cp, "   _qteCond     = <%02x>\n", i._qteCond );
 */
	      cp += sprintf( cp, "   _mktCategory = %s\n", MktCategory() );
	      cp += sprintf( cp, "   _limitUpDown = %s\n", LimitUpDown() );
	      cp += sprintf( cp, "%s", DumpMktData( md ) );
	      if ( mt() == _mtOPTION ) {
	         cp += sprintf( cp, "   _optExpire  = %d\n", OptionExpiration() );
	         cp += sprintf( cp, "   _optStrike  = %.2f\n", OptionStrike() );
	         cp += sprintf( cp, "   _optPutCall = %s\n", OptionPutOrCall() );
	         cp += sprintf( cp, "   _optOpenInt = %ld\n", OptionOpenInterest() );
	      }  
	   }
	   else if ( mt() == _mtINDEX ) {
	      cp += sprintf( cp, "   _value         = %.6f\n", i._value );
	      cp += sprintf( cp, "   _close         = %.6f\n", i._close );
	      cp += sprintf( cp, "   _closeDate     = %d\n", i._closeDate );
	      cp += sprintf( cp, "   _netChg        = %.6f\n", i._netChg );
	      cp += sprintf( cp, "   _pctChg        = %.6f\n", i._pctChg );
	      cp += sprintf( cp, "%s", DumpMktData( md ) );
/*
	      cp += sprintf( cp, "   _calcMethod    = <%02x>\n", (int)_calcMethod );
	      cp += sprintf( cp, "   _isHeld        = <%02x>\n", (int)_isHeld );
	      cp += sprintf( cp, "   _asofOpen      = %.6f\n", _asofOpen );
	      cp += sprintf( cp, "   _asofHigh      = %.6f\n", _asofHigh );
	      cp += sprintf( cp, "   _asofLow       = %.6f\n", _asofLow );
	      cp += sprintf( cp, "   _asofClose     = %.6f\n", _asofClose );
	      cp += sprintf( cp, "   _asofNetChg    = %.6f\n", _asofNetChg );
	      cp += sprintf( cp, "   _asofDate      = %08d\n", _asofDate );
	      cp += sprintf( cp, "   _asofAction    = <%02x>\n", (int)_asofAction );
	      cp += sprintf( cp, "   _dividend      = %.6f\n", _dividend );
	      cp += sprintf( cp, "   _settleValue   = %.6f\n", _settleValue );
	      cp += sprintf( cp, "   _settleTime    = %d\n", _settleTime );
	      cp += sprintf( cp, "   _settleSumm    = %.6f\n", _settleSumm );
 */
	      cp += sprintf( cp, "   _ETPIntraDay   = %.6f\n", i._ETPIntraDay );
	      cp += sprintf( cp, "   _ETPisHeld     = <%02x>\n", i._ETPisHeld );
	      cp += sprintf( cp, "   _ETPEstCashPCU = %.6f\n", i._ETPEstCashPCU );
	      cp += sprintf( cp, "   _ETPTotCashPCU = %.6f\n", i._ETPTotCashPCU );
	      cp += sprintf( cp, "   _ETPNav        = %.6f\n", i._ETPNav );
	      cp += sprintf( cp, "   _ETPSharesOut  = %ld\n", i._ETPSharesOut );
	      cp += sprintf( cp, "   _ETPDividend    = %.6f\n", i._ETPDividend );
	   }
	   else if ( mt() == _mtFUND ) {
	      cp += sprintf( cp, "   _fundType          = %s\n", FundType() );
	      cp += sprintf( cp, "   _fundCode          = %s\n", FundCode() );
	      cp += sprintf( cp, "   _frequency         = <%02x>\n", i._frequency );
	      cp += sprintf( cp, "   _reportType        = <%02x>\n", i._reportType );
	      cp += sprintf( cp, "   _parValue          = %.6f\n", i._parValue );
	      cp += sprintf( cp, "   _reportType        = <%02x>\n", i._reportType );
	      cp += sprintf( cp, "   _footnotes         = %s\n", i._footnotes );
	      cp += sprintf( cp, "   _fundNav           = %.6f\n", i._fundNav );
	      cp += sprintf( cp, "   _fundPrc           = %.6f\n", i._fundPrc );
	      cp += sprintf( cp, "   _fundWrapPrc       = %.6f\n", i._fundWrapPrc );
	      cp += sprintf( cp, "   _fundNetAssets     = %ld\n", i._fundNetAssets );
	      cp += sprintf( cp, "   _yield             = %.6f\n", i._yield );
	      cp += sprintf( cp, "   _eltr              = %.6f\n", i._eltr );
	      cp += sprintf( cp, "   _accruedInt        = %.6f\n", i._accruedInt );
	      cp += sprintf( cp, "   _dailyDivInd       = <%02x>\n", i._dailyDivInd );
	      cp += sprintf( cp, "   _dailyDiv          = %.6f\n", i._dailyDiv );
	      cp += sprintf( cp, "   _entryDate         = %08ld\n", i._entryDate );
	      cp += sprintf( cp, "   _avgMaturity       = %d\n", i._avgMaturity );
	      cp += sprintf( cp, "   _avgLife           = %d\n", i._avgLife );
	      cp += sprintf( cp, "   _yield7DayGross    = %.6f\n", i._yield7DayGross );
	      cp += sprintf( cp, "   _yield7DaySubsidiz = %.6f\n", i._yield7DaySubsidized );
	      cp += sprintf( cp, "   _yield7DayAnnualiz = %.6f\n", i._yield7DayAnnualized );
	      cp += sprintf( cp, "   _yield30Day        = %.6f\n", i._yield30Day );
	      cp += sprintf( cp, "   _yield30DayDate    = %08ld\n", i._yield30DayDate );
	      cp += sprintf( cp, "   _shortTermGain     = %.6f\n", i._shortTermGain );
	      cp += sprintf( cp, "   _longTermGain      = %.6f\n", i._longTermGain );
	      cp += sprintf( cp, "   _unallocatedDist   = %.6f\n", i._unallocatedDist );
	      cp += sprintf( cp, "   _returnOnCapital   = %.6f\n", i._returnOnCapital );
	      cp += sprintf( cp, "   _exDate            = %08ld\n", i._exDate );
	      cp += sprintf( cp, "   _recordDate        = %08ld\n", i._recordDate );
	      cp += sprintf( cp, "   _paymentDate       = %08ld\n", i._paymentDate );
	      cp += sprintf( cp, "   _reinvestDate      = %08ld\n", i._reinvestDate );
	      cp += sprintf( cp, "   _exDate            = %08ld\n", i._exDate );
/*
 * 12-08-01 jcs  Build 35
 *
	      cp += sprintf( cp, "   _totalCashDist     = %.6f\n", _totalCashDist );
	      cp += sprintf( cp, "   _nonQualCashDist   = %.6f\n", _nonQualCashDist );
	      cp += sprintf( cp, "   _qualCashDist      = %.6f\n", _qualCashDist );
	      cp += sprintf( cp, "   _taxFreeCashDist   = %.6f\n", _taxFreeCashDist );
	      cp += sprintf( cp, "   _ordForgnTaxCredit = %.6f\n", _ordForeignTaxCredit );
	      cp += sprintf( cp, "   _qualForgnTaxCred  = %.6f\n", _qualForeignTaxCredit);
	      cp += sprintf( cp, "   _stockDividend     = %.6f\n", _stockDividend );
 */
	      cp += sprintf( cp, "   _fundClose         = %.6f\n", i._fundClose );
	      cp += sprintf( cp, "   _fundNetChg        = %.6f\n", i._fundNetChg );
	      cp += sprintf( cp, "   _fundPctChg        = %.6f\n", i._fundPctChg );
	   }
	   else if ( mt() == _mtFUTURE ) {
#ifdef TODO_IMAGE
	      cp += sprintf( cp, "   _prodClass    = <%02x>\n", i._prodClass );
	      cp += sprintf( cp, "   _expireMDY    = %d\n", i._expireMDY );
	      cp += sprintf( cp, "   _settleDate   = %d\n", i._settleDate );
	      cp += sprintf( cp, "   _strikePrc    = %.6f\n", i._strikePrc );
	      cp += sprintf( cp, "   _putCall      = <%02x>\n", i._putCall );
	      cp += sprintf( cp, "   _undMDY       = %d\n", i._undMDY );
	      cp += sprintf( cp, "   _lastMDY      = %d\n", i._lastMDY );
	      cp += sprintf( cp, "   _strikePrcInd = <%02x>\n", i._strikePrcInd );
	      cp += sprintf( cp, "   _descr        = %s\n", i._descr );
	      cp += sprintf( cp, "   _comboType    = %d\n", i._comboType );
	      cp += sprintf( cp, "   _nComboLeg    = %d\n", i._nComboLeg );
	      cp += sprintf( cp, "   _qteSessID    = <%02x>\n", i._qteSessID );
	      cp += sprintf( cp, "   _trdSessID    = <%02x>\n", i._trdSessID );
	      cp += sprintf( cp, "   _typeCode     = <%02x>\n", i._typeCode );
	      cp += sprintf( cp, "   _open2        = %.6f\n", i._open2 );
	      cp += sprintf( cp, "   _close2       = %.6f\n", i._close2 );
#endif // TODO_IMAGE
	      cp += sprintf( cp, "%s", DumpMktData( md ) );
	      cp += sprintf( cp, "   _futExpire  = %d\n", FuturesExpiration() );
	   }
	   else if ( mt() == _mtBOND )
	      cp += sprintf( cp, "%s", DumpMktData( md ) );
	   dump_ += "\n";
	   dump_ += buf;
	   return dump_.data();
	}

	///////////////////////////////
	// Helpers
	///////////////////////////////
private:
	const char *DumpMktData( std::string &rtn )
	{
	   ::Image    &i = (*this)();
	   std::string bt, at, tt, ot, ht, lt;
	   char        buf[4*K], *cp;

	   cp  = buf;
	   *cp = '\0';
	   bt  = TimeMs( i._bidTime );
	   at  = TimeMs( i._askTime );
	   tt  = TradeTimeMs( i._trdTime );
	   ot  = TimeMs( i._openTime );
	   ht  = TimeMs( i._highTime );
	   lt  = TimeMs( i._lowTime );
	   cp += sprintf( cp, "   _bid      = %.6f\n", i._bid );
	   cp += sprintf( cp, "   _bidSize  = %d\n", i._bidSize );
	   cp += sprintf( cp, "   _bidTime  = %s\n", bt.data() );
	   cp += sprintf( cp, "   _bidMmid  = %s\n", i._bidMktCtr );
	   cp += sprintf( cp, "   _ask      = %.6f\n", i._ask );
	   cp += sprintf( cp, "   _askSize  = %d\n", i._askSize );
	   cp += sprintf( cp, "   _askTime  = %s\n", at.data() );
	   cp += sprintf( cp, "   _askMmid  = %s\n", i._askMktCtr );
//      cp += sprintf( cp, "   _trdMmid  = <%02x>\n", (int)i._trdMmid );
	   cp += sprintf( cp, "   _trdMmid  = %s\n", i._trdMktCtr );
	   cp += sprintf( cp, "   _trdCond  = <%02x>\n", (int)i._trdCond );
	   cp += sprintf( cp, "   _prcTck   = %s\n", prcTck() );
	   cp += sprintf( cp, "   _trdPrc   = %.6f\n", i._trdPrc );
	   cp += sprintf( cp, "   _trdVol   = %d\n", i._trdVol );
	   cp += sprintf( cp, "   _trdTime  = %s\n", tt.data() );
	   cp += sprintf( cp, "   _acVol    = %ld\n", i._acVol );
	   cp += sprintf( cp, "   _tnOvr    = %ld\n", i._tnOvr );
	   cp += sprintf( cp, "   _vwap     = %.6f\n", vwap() );
	   cp += sprintf( cp, "   _open     = %.6f\n", i._open );
	   cp += sprintf( cp, "   _openVol  = %d\n", i._openVol );
	   cp += sprintf( cp, "   _openTime = %s\n", ot.data() );
	   cp += sprintf( cp, "   _high     = %.6f\n", i._high );
	   cp += sprintf( cp, "   _highTime = %s\n", ht.data() );
	   cp += sprintf( cp, "   _low      = %.6f\n", i._low );
	   cp += sprintf( cp, "   _lowTime  = %s\n", lt.data() );
	   cp += sprintf( cp, "   _close    = %.6f\n", i._close );
	   cp += sprintf( cp, "   _closeDate= %d\n", i._closeDate );
	   cp += sprintf( cp, "   _netChg   = %.6f\n", i._netChg );
	   cp += sprintf( cp, "   _pctChg   = %.6f\n", i._pctChg );
	   rtn = buf;
	   return rtn.data();
	}

	std::string TradeTimeMs( QFTime qt )
	{
	   time_t     now;
	   QFTime     qn;
	   struct tm *tm, l;
	   char       buf[K];
	   int        tMs;

	   // Millis since midnight Jan 1, 1970

	   qn  = qt / 1000;
	   now = (time_t)qn;
	   tMs = qt % 1000;
	   tm  = ::localtime( &now );
	   l   = *tm;
	   sprintf( buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
	                 l.tm_year+1900, l.tm_mon+1, l.tm_mday,
	                 l.tm_hour, l.tm_min, l.tm_sec, tMs );
	   return std::string( buf );
	}

	////////////////////////
	// private Members
	////////////////////////
private:
	std::string _desc;

};  // class Image

} // namespace QUODD

#endif // __QUODD_Image_H 
