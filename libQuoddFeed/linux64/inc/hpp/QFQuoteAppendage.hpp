/******************************************************************************
*
*  QFQuoteAppendage.hpp
*     QuoddFeed Quote Appendage
*
*  REVISION HISTORY:
*     20 APR 2016 jcs  Created.
*
*  (c) 1994-2016 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_QFQuoteAppendage_H
#define __QUODD_QFQuoteAppendage_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//   c l a s s   Q F Q u o t e A p p e n d a g e
//
////////////////////////////////////////////////

/**
 * \class QFQuoteAppendage
 * \brief This class encapsulates an ::OPTrade (Option Trade) message structure.
 *
 * A OPTrade object is driven into your application via the
 * Channel::OnUpdate( OPTrade & ) virtual method that is called
 * asynchronously when a Equity Trade message arrives from the UltraCache.
 */

class QFQuoteAppendage
{
friend class OPTrade;

	////////////////////////////////////
	// Constructor / Destructor
	////////////////////////////////////
protected:
	QFQuoteAppendage( Message &msg, bool bEquity ) :
	   msg_( msg ),
	   bEquity_( bEquity ),
	   bidTime_(),
	   askTime_(),
	   dump_()
	{
		Reset();
	}

	virtual ~QFQuoteAppendage() { ; }

	/**
	 * \brief Called by OPTrade to set contents of this appendage
	 *
	 * \param qm - New ::QFQuoteAppendage struct
	 */
	void Set( ::QFQuoteAppendage &qm )
	{
		Reset();
      qm_ = qm;
   }

	/**
	 * \brief Reset / zero out quote appendage values
	 */
	void Reset()
	{
		::memset( &qm_, 0, sizeof( qm_ ) );
		bidTime_ = "";
		askTime_ = "";
		dump_    = "";
	}

	////////////////////////////////////
	// Access / Operations
	////////////////////////////////////
public:
	/**
	 * \brief Returns string-ified BID time in HH:MM:SS.mmm
	 *
	 * \return string-ified BID time in HH:MM:SS.mmm
	 */
	const char *BidTime()
	{
		::QFQuoteAppendage &oa = (*this)();

		if ( !bidTime_.size() )
			bidTime_ = msg_.TimeMs( oa._bidTime );
		return bidTime_.data();
	}

	/**
	 * \brief Returns string-ified ASK time in HH:MM:SS.mmm
	 *
	 * \return string-ified ASK time in HH:MM:SS.mmm
	 */
	const char *AskTime()
	{
		::QFQuoteAppendage &oa = (*this)();

		if ( !askTime_.size() )
			askTime_ = msg_.TimeMs( oa._askTime );
		return askTime_.data();
	}

	/**
	 * \brief Returns a reference to the associated ::OPTrade struct
	 *
	 * \return ::OPTrade encapsulated by this object
	 */
	::QFQuoteAppendage &operator()()
	{
		return qm_;
	}

	/**
	 * \brief Dumps formatted message header
	 *
	 * \return Pointer to formatted message header
	 */
	const char *Dump()
	{
		::QFQuoteAppendage &oa = (*this)();
		char                buf[4*K], *cp, *mb, *ma;
		const char         *fmt, *pb, *pa, *ty;

		// Guts

		cp  = buf;
		*cp = '\0';
		pb  = BidTime();
		pa  = AskTime();
		mb  = oa._bidMktCtr;
		ma  = oa._askMktCtr;
		ty  = bEquity_ ? "EQ-" : "";
		fmt = "   %s%s %s : %4d @ %7.2f from {%s}\n";
		cp += sprintf( cp, fmt, ty, "BID", pb, oa._bidSize, oa._bid, mb );
		cp += sprintf( cp, fmt, ty, "ASK", pa, oa._askSize, oa._ask, ma );

		// Return

		dump_ += buf;
		return dump_.data();
	}

	////////////////////////
	// private Members
	////////////////////////
private:
	Message           &msg_;
	bool               bEquity_;
	::QFQuoteAppendage qm_;
	std::string        bidTime_;
	std::string        askTime_;
	std::string        dump_;

};  // class QFQuoteAppendage

} // namespace QUODD

#endif // __QUODD_QFQuoteAppendage_H 
