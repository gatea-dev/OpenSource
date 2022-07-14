/******************************************************************************
*
*  Message.hpp
*     QuoddFeed Message
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     12 JAN 2015 jcs  Build 11: EQLimitUpDn; TimeMs()
*     12 MAR 2015 jcs  Build 14: qMsg_FUNDnav; Dump()
*     23 MAR 2015 jcs  Build 15: DumpLen()
*      1 MAY 2015 jcs  Build 17: QoS
*      6 JUL 2015 jcs  Build 19: namespace management
*     10 FEB 2016 jcs  Build 21: BONDQuote / BONDTrade
*     20 APR 2016 jcs  Build 22: virtual void Set()
*     18 JAN 2018 jcs  Build 23: Heartbeat
*
*  (c) 1994-2018 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_MESSAGE_H
#define __QUODD_MESSAGE_H
#include <hpp/Enum/UltraFeedSpec.hpp>
#include <hpp/Enum/BondSpec.hpp>
#include <hpp/Enum/EquitySpec.hpp>
#include <hpp/Enum/FundSpec.hpp>
#include <hpp/Enum/FuturesSpec.hpp>
#include <hpp/Enum/OptionSpec.hpp>

// Hard-coded

#define _MAX_BLOB_CEL 128  // < 128 bytes per cell

namespace QUODD
{

class Channel;   // Forward declaration

////////////////////////////////////////////////
//
//         c l a s s   M e s s a g e
//
////////////////////////////////////////////////

/**
 * \class Message
 * \brief The base class of all message encapsulation classes such as 
 * EQBbo, OPTrade, and FUTRQuote.
 *
 * This base class contains methods that are common to all message 
 * encapsulation classes, and primarily deals with the common qHdr 
 * struct found in all messages.
 */

class Message
{
friend class Channel;

	// This class encapsulates a single ::QuoddMsg from UltraCache

	////////////////////////////////////
	// Constructor / Destructor
	////////////////////////////////////
protected:
	Message()
	{
		Set( (Channel *)0, (::QuoddMsg *)0 );
	}

	virtual ~Message() { ; }


	/**
	 * \brief Called by Channel to reuse this message
	 *
	 * \param ch - Channel that message arrived on
	 * \param qm - New ::QuoddMsg struct
	 */
	virtual void Set( Channel *ch, ::QuoddMsg *qm )
	{
		ch_    = ch;
		qm_    = qm;
		tm_[0] = '\0';
		dt_[0] = '\0';
		dump_  = "";
	}


	////////////////////////////////////
	// Access
	////////////////////////////////////
public:
	/**
	 * \brief Returns length of this message
	 *
	 * \return Length of this message
	 */
	u_short len()
		// Returns message length
	{
		return qh()._len;
	}

	/**
	 * \brief Returns type of this ::QuoddMsg
	 *
	 * \return ::QuoddMsg Message Type
	 */
	QuoddMsgType MT()
	{
		return qm()._mt;
	}

	/**
	 * \brief Returns UltraFeed message type
	 *
	 * \return UltraFeed mesasge type
	 */
	u_char mt()
	{
		return qh()._mt;
	}

	/**
	 * \brief Returns UltraFeed message sub-type
	 *
	 * \return UltraFeed mesasge sub-type
	 */
	u_char mtSub()
	{
		return qh()._mtSub;
	}

	/**
	 * \brief Returns UltraFeed protocol of this message
	 *
	 * \return UltraFeed protocol of this message
	 * \see ProtocolName()
	 */
	u_char protocol()
	{
		return qh()._protocol;
	}

	/**
	 * \brief Returns name of protocol in this message
	 *
	 * \return Name of protocol in this message - OPRA, etc.
	 */
	const char *ProtocolName()
	{
		return ULTRAFEED::UltraFeedSpec::Protocol( qh() );
	}

	/**
	 * \brief Returns originating channel of this message
	 *
	 * \return Originating channel - OPRA line 1, etc.
	 */
	u_char chanIdx()
	{
		return qh()._chanIdx;
	}

	/**
	 * \brief Returns textual name of this message
	 *
	 * \return Textual name of this message
	 */
	const char *MsgType()
	{
		switch( MT() ) {
			case qMsg_undef:       return "Undefined   ";
			case qMsg_Status:      return "Status      ";
			case qMsg_UCStatus:    return "UCStatus    ";
			case qMsg_BlobList:    return "BlobList    ";
			case qMsg_BlobTable:   return "BlobTable   ";
			case qMsg_Image:       return "Image       ";
			case qMsg_EQBbo:       return "EQBbo       ";
			case qMsg_EQBboMM:     return "EQBboMM     ";
			case qMsg_EQQuote:     return "EQQte       ";
			case qMsg_EQQuoteMM:   return "EQQteMM     ";
			case qMsg_EQTrade:     return "EQTrade     ";
			case qMsg_EQLimitUpDn: return "EQLimitUpDn ";
			case qMsg_OPBbo:       return "OPBbo       ";
			case qMsg_OPQuote:     return "OPQte       ";
			case qMsg_OPTrade:     return "OPTrade     ";
			case qMsg_FUTRQuote:   return "FUTRQte     ";
			case qMsg_FUTRTrade:   return "FUTRTrade   ";
			case qMsg_IDXValue:    return "IDXValue    ";
			case qMsg_IDXSummary:  return "IDXSummary  ";
			case qMsg_IDXSetlValu: return "IDXSetlValue";
			case qMsg_IDXSetlSumm: return "IDXSetlSumm ";
			case qMsg_FUNDnav:     return "FUNDnav     ";
			case qMsg_FUTRMisc:    return "FUTRMisc    ";
			case qMsg_QoS:         return "QoS         ";
			case qMsg_BONDQuote:   return "BONDQte     ";
			case qMsg_BONDTrade:   return "BONDTrd     ";
			case qMsg_Heartbeat:   return "Heartbeat   ";
		}
		return "UNKNOWN";
	}

	/**
	 * \brief Returns ticker associated with this message
	 *
	 * \return Ticker associated with this message
	 */
	const char *tkr()
	{
		return qm()._tkr;
	}

	/**
	 * \brief Returns RTL (sequence number) contained in this message
	 *
	 * \return RTL (sequence number) contained in this message
	 */
	long RTL()
	{
		return qh()._RTL;
	}

	/**
	 * \brief Returns underlying ::QuoddMsg
	 *
	 * \return Underlying ::QuoddMsg
	 */
	::QuoddMsg &qm()
	{
		return *qm_;
	}

	/**
	 * \brief Returns QuoddFeed header part of this message
	 *
	 * \return QuoddFeed header part of this message
	 */
	qHdr &qh()
	{
		return qm()._v._BlobList._hdr;
	}

	/**
	 * \brief Returns message time formatted in HH:MM:SS.mmm
	 *
	 * \param tm - Time in millis since midnight
	 * \return Message time formatted in HH:MM:SS.mmm
	 */
	std::string TimeMs( long tm )
	{
		qHdr q;
		char buf[K];

		q._time = tm;
		return std::string( ::Quodd_pTimeMs( q, buf ) );
	}

	/**
	 * \brief Returns time formatted in YYYY-MM-DD HH:MM:SS.mmm
	 *
	 * \param tm - Time in millis since midnight
	 * \return Message time formatted in YYYY-MM-DD HH:MM:SS.mmm
	 */
	std::string DateTimeMs( long tm )
	{
		qHdr q;
		char buf[K];

		q._time = tm;
		return std::string( ::Quodd_pDateTimeMs( q, buf ) );
	}

	/**
	 * \brief Returns message time formatted in HH:MM:SS.mmm
	 *
	 * \return Message time formatted in HH:MM:SS.mmm
	 */
	char *MsgTimeMs()
	{
		if ( tm_[0] == '\0' )
			return ::Quodd_pTimeMs( qh(), tm_ );
		return tm_;
	}

	/**
	 * \brief Returns message time formatted in YYYY-MM-DD HH:MM:SS.mmm
	 *
	 * \return Message time formatted in YYYY-MM-DD HH:MM:SS.mmm
	 */
	char *MsgDateTimeMs()
	{
		if ( dt_[0] == '\0' )
			return ::Quodd_pDateTimeMs( qh(), dt_ );
		return dt_;
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
		return MsgType();
   }

	/**
	 * \brief Dumps formatted message header
	 *
	 * \return Pointer to formatted message header
	 */
	virtual const char *Dump()
	{
		char        buf[K], *cp;
		std::string s;

		cp  = buf;
		cp += sprintf( cp, "[%s] ", MsgDateTimeMs() );
		cp += sprintf( cp, "%s : ", pMsgName( s ) );
		cp += sprintf( cp, "%-6s ", tkr() );
		cp += sprintf( cp, "<%06ld> ", RTL() );
		cp += sprintf( cp, "<<%02d>> : ", protocol() );
//		cp += sprintf( cp, "{%s} ", ProtocolName() );
		dump_ = buf;
		return dump_.data();
	}

	/**
	 * \brief Returns length of Dump() string
	 *
	 * \return Length of Dump() string
	 */
	virtual int DumpLen()
	{
		return dump_.length();
	}


	////////////////////////
	// private Members
	////////////////////////
private:
	Channel    *ch_;
	::QuoddMsg *qm_;
	char        tm_[K]; // MsgTimeMs()
	char        dt_[K]; // MsgDateTimeMs()
protected:
	std::string dump_;
};  // class Message

} // namespace QUODD

#endif // __QUODD_MESSAGE_H 
