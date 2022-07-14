/******************************************************************************
*
*  QoS.hpp
*     QuoddFeed QoS Message
*
*  REVISION HISTORY:
*      1 MAY 2015 jcs  Created.
*
*  (c) 1994-2015 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_QoS_H
#define __QUODD_QoS_H
#include <hpp/Message.hpp>

namespace QUODD
{

////////////////////////////////////////////////
//
//        c l a s s   Q o S
//
////////////////////////////////////////////////

/**
 * \class QoS
 * \brief This class encapsulates an ::QoS (Quality of Service) message struct.
 *
 * A QoS object is driven into your application by the library when a message 
 * gap is detected in the multicast stream.  You receive it via the
 * Channel::OnUpdate( QoS & ) virtual method that is called
 * asynchronously when the library detects a message gap in the multicast stream.
 */
class QoS : public Message
{
public:
	/**
	 * \brief Returns a reference to the associated ::QoS struct
	 *
	 * \return ::QoS encapsulated by this object
	 */
	::QoS &operator()()
	{
		return qm()._v._QoS;
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
		::QoS &qos = (*this)();
		char   buf[K], *cp;

		// Header

		Message::Dump();

		cp  = buf;
		cp += sprintf( cp, "seqNum=%" PRIu64 "; ", qos._seqNum );
		cp += sprintf( cp, "GapSiz = %d; ", qos._numDropped );
		cp += sprintf( cp, "Total  = %" PRIu64 "\n", qos._totalDropped );

		// Return

		dump_ += buf;
		return dump_.data();
	}
};  // class QoS

} // namespace QUODD

#endif // __QUODD_QoS_H 
