/******************************************************************************
*
*  EQBboMM.hpp
*     QuoddFeed EQBboMM Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     17 MAR 2015 jcs  Build 14: Dump()
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_EQBboMM_H
#define __QUODD_EQBboMM_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//        c l a s s   E Q B b o M M
//
////////////////////////////////////////////////

/**
 * \class EQBboMM
 * \brief This class encapsulates an ::EQBboMM (Equity BBO Market Maker) 
 * message structure.
 *
 * A EQBboMM object is driven into your application via the
 * Channel::OnUpdate( EQBboMM & ) virtual method that is called
 * asynchronously when a Equity BBO Market Maker message arrives from 
 * the UltraCache.
 */

class EQBboMM : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::EQBboMM struct
	 *
	 * \return ::EQBboMM encapsulated by this object
	 */
	::EQBboMM &operator()()
	{
		return qm()._v._EQBboMM;
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
		::EQBboMM &b = (*this)();
		char       buf[4*K], *cp;

		// Header

		Message::Dump();

		// Guts

		cp  = buf;
		*cp = '\0';
		cp += sprintf( cp, "{%s} %6.2f x %6.2f {%s}; %dx%d {%s}\n",
		               b._bidMmid, b._bid,
		               b._ask, b._askMmid,
		               b._bidSize, b._askSize, b._mktCtr );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class EQBboMM

} // namespace QUODD

#endif // __QUODD_EQBboMM_H 
