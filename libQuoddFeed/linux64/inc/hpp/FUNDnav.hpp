/******************************************************************************
*
*  FUNDnav.hpp
*     QuoddFeed FUNDnav Message
*
*  REVISION HISTORY:
*     13 MAR 2015 jcs  Created.
*     13 APR 2015 jcs  Build 16: _netAssets / _totalNetAssets
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_FUNDnav_H
#define __QUODD_FUNDnav_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//       c l a s s   F U N D n a v
//
////////////////////////////////////////////////

/**
 * \class FUNDnav
 * \brief This class encapsulates an ::FUNDnav (Mututal Fund Net Asset Value)
 * message structure.
 *
 * A FUNDnav object is driven into your application via the
 * Channel::OnUpdate( FUNDnav & ) virtual method that is called
 * asynchronously when a Mutual Fund message arrives from the UltraCache.
 */
class FUNDnav : public Message
{
private:
	std::string _rtn;

public:
	/**
	 * \brief Returns a reference to the associated ::FUNDnav struct
	 *
	 * \return ::FUNDnav encapsulated by this object
	 */
	::FUNDnav &operator()()
	{
		return qm()._v._FUNDnav;
	}

	/**
	 * \brief Returns a textual description of the Fund Type
	 *
	 * \return Textual description of the Fund Type
	 */
	const char *FundType()
	{
		return ULTRAFEED::FundSpec::FundType( (*this)()._fundType );
	}

	/**
	 * \brief Returns a textual description of the Fund Code
	 *
	 * \return Textual description of the Fund Code
	 */
	const char *FundCode()
	{
		return ULTRAFEED::FundSpec::FundCode( (*this)()._fundCode );
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
		::FUNDnav &nav = (*this)();
		char       buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "   " );
		cp += sprintf( cp, "DT=%08ld ", nav._entryDate );
		cp += sprintf( cp, " NAV=%.2f; ", nav._nav );
		cp += sprintf( cp, "PRC=%.2f; ", nav._price );
		cp += sprintf( cp, "NET-AST=%.1f; ", nav._netAssets );
		cp += sprintf( cp, "TOT-AST=%.1f; ", nav._totalNetAssets );
		cp += sprintf( cp, " YLD=%.3f\n", nav._yield );
		cp += sprintf( cp, "   Type = %s\n", FundType() );
		cp += sprintf( cp, "   Code = %s\n", FundCode() );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class FUNDnav

} // namespace QUODD

#endif // __QUODD_FUNDnav_H 
