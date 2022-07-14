/******************************************************************************
*
*  OPTrade.hpp
*     QuoddFeed OPTrade Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*      8 JUL 2015 jcs  Build 19: _opSubTRDCXL_ID
*     20 APR 2016 jcs  Build 22: Appendages
*
*  (c) 1994-2016 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_OPTrade_H
#define __QUODD_OPTrade_H
#include <hpp/Message.hpp>
#include <hpp/QFQuoteAppendage.hpp>

#define _QM_MINSZ           18
#define _OP_TRDSZ           _QM_MINSZ        + 70
#define _OP_TRDSZ_ID        _OP_TRDSZ        +  8
#define _OP_TRDSZ_ID_APP    _OP_TRDSZ_ID     + 32
#define _OP_TRDSZ_ID_APP_EQ _OP_TRDSZ_ID_APP + 32

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   O P T r a d e
//
////////////////////////////////////////////////

/**
 * \class OPTrade
 * \brief This class encapsulates an ::OPTrade (Option Trade) message structure.
 *
 * A OPTrade object is driven into your application via the
 * Channel::OnUpdate( OPTrade & ) virtual method that is called
 * asynchronously when a Equity Trade message arrives from the UltraCache.
 */

class OPTrade : public Message
{
friend class Channel;
   ////////////////////////////////////
   // Constructor / Destructor
   ////////////////////////////////////
public:
   OPTrade() :
	   opQte_( *this, false ),
	   eqQte_( *this, true ),
	   _rtn()
   { ; }

   ~OPTrade() { ; }

	/**
	 * \brief Called by Channel to reuse this message
	 *
	 * \param ch - Channel that message arrived on
	 * \param qm - New ::QuoddMsg struct
	 */
protected:
	virtual void Set( Channel *ch, ::QuoddMsg *qm )
	{
		Message::Set( ch, qm );
		opQte_.Reset();
		eqQte_.Reset();
		if ( HasQuote() )
			opQte_.Set( (*this)()._option );
		if ( EQ_HasQuote() )
			eqQte_.Set( (*this)()._equity );
   }


	/////////////////////////////////////
	// Access / Operations
	/////////////////////////////////////
public:
	/**
	 * \brief Returns a reference to the associated ::OPTrade struct
	 *
	 * \return ::OPTrade encapsulated by this object
	 */
	::OPTrade &operator()()
	{
		return qm()._v._OPTrade;
	}

	/**
	 * \brief Return true if this is a cancel trade message
	 *
	 * \return true if this is a cancel trade message
	 */
	 bool IsCxl()
	 {
		char mt2 = mtSub();

		return( ( mt2 == _opSubTRDCXL ) || ( mt2 == _opSubTRDCXL_ID ) );
	 }

	/**
	 * \brief Return true if this is a summary trade message
	 *
	 * \return true if this is a summary trade message
	 */
	 bool IsSummary()
	 {
		return( mtSub() == _opSubTRDSUMM );
	 }

	/**
	 * \brief Returns true if this has contract quote appendage
	 *
	 * \return true if this has contract quote appendage
	 */
	 bool HasQuote()
	 {
		return( len() >= _OP_TRDSZ_ID_APP );
	 }

	/**
	 * \brief Returns true if this has equity (underlyer) quote appendage
	 *
	 * \return true if this has equity (underlyer) quote appendage
	 */
	 bool EQ_HasQuote()
	 {
		return( len() >= _OP_TRDSZ_ID_APP_EQ );
	 }

	/**
	 * \brief Returns contract quote if HasQuote() else NULL
	 *
	 * \return Contract quote if HasQuote() else NULL
	 */
	QFQuoteAppendage *contract_quote()
	{
		return HasQuote() ? &opQte_ : (QFQuoteAppendage *)0;
	}

	/**
	 * \brief Returns underlyer quote if EQ_HasQuote() else NULL
	 *
	 * \return Underlyer quote if EQ_HasQuote() else NULL
	 */
	QFQuoteAppendage *underlyer_quote()
	{
		return EQ_HasQuote() ? &eqQte_ : (QFQuoteAppendage *)0;
	}


	/**
	 * \brief Returns description of price direction - UP, DOWN, UNCHANGED
	 *
	 * \return Description of price direction - UP, DOWN, UNCHANGED
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
	 * \brief Returns comma-separated list of trade flag descriptions
	 *
	 * \return Comma-separated list of trade flag descriptions
	 */
	 const char *TradeFlags()
	 {
		return ULTRAFEED::OptionSpec::TradeFlags( (*this)(), _rtn );
	 }

	/**
	 * \brief Returns true if this is an eligible trade
	 *
	 * \return true if this is an eligible trade
	 */
	 bool IsEligible()
	 {
		return ULTRAFEED::OptionSpec::PriceIsValid( (*this)() );
	 }

	/**
	 * \brief Returns IsEligible()
	 *
	 * \return IsEligible()
	 */
	bool PriceIsValid()
	{
		return IsEligible();
	}


	////////////////////////////////////
	// Polymorphic Message Dump
	////////////////////////////////////
public:
	/**
	 * \brief Return name of this message.  Called by Dump() to
	 * allow specific messages to override this value, for example
	 * trades w/ cancels
	 *
	 * \param str - Optional output buffer
	 * \return Name of this message
	 */
	virtual const char *pMsgName( std::string &s )
	{
		s = Message::pMsgName( s );
		s += IsCxl()     ? "-Cxl"  : ""; 
		s += IsSummary() ? "-Summ" : ""; 
		return s.data();
	}

	/**
	 * \brief Dumps formatted message header
	 *
	 * \return Pointer to formatted message header
	 */
	virtual const char *Dump()
	{
		::OPTrade  &ot = (*this)();
		char        buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "%ld @ %6.2f", ot._trdVol, ot._trdPrc );
		cp += sprintf( cp, " {%s}; ", ot._mktCtr );
		cp += sprintf( cp, " ID=%ld (uniq=%ld)\n", ot._trdID, ot._trdIDuniq );
		cp += sprintf( cp, "   _openPrc = %.4f; ", ot._openPrc );
		cp += sprintf( cp, "_openVol = %ld\n", ot._openVol );
		cp += sprintf( cp, "   _high = %.4f; _low=%.4f\n", ot._high, ot._low );
		cp += sprintf( cp, "   _netChg = %.4f; ", ot._netChg );
		cp += sprintf( cp, "_pctChg = %.4f\n", ot._pctChg );
		cp += sprintf( cp, "   _tnOvr = %ld; ", ot._tnOvr );
		cp += sprintf( cp, "_acVol = %ld; ", ot._acVol );
		cp += sprintf( cp, "_vwap = %.4f\n", ot._vwap );

		// Quote Appendages

		if ( HasQuote() )
			cp += sprintf( cp, opQte_.Dump() );
		if ( EQ_HasQuote() )
			cp += sprintf( cp, eqQte_.Dump() );

		// Return

		dump_ += buf;
		return dump_.data();
	}

	////////////////////////
	// private Members
	////////////////////////
private:
   QFQuoteAppendage opQte_;
   QFQuoteAppendage eqQte_;
	std::string      _rtn;

};  // class OPTrade

} // namespace QUODD

#endif // __QUODD_OPTrade_H 
