/******************************************************************************
*
*  Messages.h
*     UltraCache wire protocol - messages
*
*  REVISION HISTORY:
*     12 DEC 2012 jcs  Created
*     14 DEC 2012 jcs  Build  2: Market Maker
*     16 JAN 2013 jcs  Build  3: Image; No mo u_xxx
*      7 MAR 2013 jcs  Build  4: FUTRTrade / FUTRQuote
*     10 MAR 2013 jcs  Build  5: BlobRow / BlobTable
*     11 OCT 2013 jcs  Build  7: libmddWire impedence match
*     31 JAN 2014 jcs  Build  9: Status
*      3 SEP 2014 jcs  Build 10: IDXValue
*     15 DEC 2014 jcs  Build 11: EQLimitUpDn
*     13 MAR 2015 jcs  Build 14: FUNDnav
*     24 MAR 2015 jcs  Build 15: IDXSummary._volume; FUTRMisc
*      1 MAY 2015 jcs  Build 17: QoS; Extended trade fields
*     10 FEB 2016 jcs  Build 21: BONDTrade / BONDQuote
*     19 APR 2016 jcs  Build 22: QFQuoteAppendage
*     18 JAN 2018 jcs  Build 23: Heartbeat
*
*  (c) 1994-2018 Gatea Ltd.
*******************************************************************************/
#ifndef __QOD_MESSAGES_H
#define __QOD_MESSAGES_H

#ifdef WIN32
#include <winsock.h>
typedef __int64        u_int64_t;
#define PRIu64 "llu"
#define PRId64 "lld"
#else
#include <sys/types.h>
#if defined(__LP64__) || defined(_LP64)
#define PRIu64 "lu"
#define PRId64 "ld"
#else
#define PRIu64 "llu"
#define PRId64 "lld"
#endif /* 64-bit Linux */
#endif // WIN32

typedef u_int64_t  QFTime;

/**************************
 * Common Header
 *************************/
/**
 * \struct qHdr
 * \brief The header of any message received from the UltraCache.
 *
 * Every message received from the UltraCache contains the same header, 
 * which is stored in this structure.  It is accessible from the 
 * actual message, for example EQBbo._hdr.
 */
typedef struct {
   /**
    * \brief Message Length, including header
    */
   u_short _len;
   /**
    * \brief Raw Message Type from UltraCache 
    *
    * This differs from the QuoddMsg._mt.  You typically do not use 
    * ths qHdr._mt.
    * @see QuoddMsgType
    */
   u_char  _mt;
   /**
    * \brief Raw Message Sub-Type from UltraCache
    * 
    * You typically do not use this qHdr._mtSub.
    */
   u_char  _mtSub;
   /**
    * \brief UltraFeed protocol
    *
    * For example OPRA is protocol 50
    *
    * \see QUODD::Message::ProtocolName()
    */
   u_char  _protocol;
   /**
    * \brief UltraFeed channel index (incoming multicast feed)
    *
    * Each protocol is delivered on one or more multicast streams.  For 
    * example, OPRA has 48 multicast streams.  The qHdr._chanIdx defines 
    * which of the incoming multicast streams this message originated from.
    */
   u_char  _chanIdx;
   /**
    * \brief Message time in milliseconds since midnight
    */
   u_int   _time;
   /**
    * \brief Unique stream ID  
    */
   u_int   _tag;
   /**
    * \brief Record transaction level
    *
    * The UltraCache keeps a message count for each stream - e.g., I:DJI, 
    * CSCO, O:GOOG\14I20\575.00, etc. - and sends the count in each message 
    * as qHdr._RTL.  It is reset daily by the UltraCache at session open.
    */
   u_int   _RTL;
} qHdr;


/**************************
 * Common Msgs
 *************************/
/**
 * \struct Status
 * \brief A DEAD status message with description.
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt ==
 * QuoddMsgType::qMsg_Status.   You receive a Status message for 
 * non-existent streams such as DELL.  This is the last message you 
 * will receive for the (now dead) stream.
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Textual description of DEAD Status */
   char  *_msg;
} Status;

/**************************
 * Quote Appendage
 *************************/
/**
 * \struct QFQuoteAppendage
 * \brief Quote Appendage
 *
 * This is appended to Trade Data, for example OPTrade
 */
typedef struct {
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Bid Time - Millis since Midnight */
   int    _bidTime;
   /** \brief Ask Time - Millis since Midnight */
   /** \brief Ask Market Center */
   int    _askTime;
   /** \brief Bid Market Center */
   char  *_bidMktCtr;
   /** \brief Ask Market Center */
   char  *_askMktCtr;
} QFQuoteAppendage;


/**************************
 * _mtBLOB
 *************************/
/**
 * \struct BlobList
 * \brief A list of results returned from a query such as Quodd_GetOptionChain() 
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_BlobList
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /**
    * \brief Last Blob packet received
    *
    * This ALWAYS equals BlobList._totPkt 
    */
   int    _nPkt;
   /**
    * \brief Total Blob packets received
    *
    * This ALWAYS equals BlobList._nPkt
    */ 
   int    _totPkt;
   /** \brief List size */ 
   int    _nLst;
   /** \brief List name */ 
   char   _name[32];
   /**
    * \brief The List
    *
    * Equivalent to char *_lst[BlobList._nLst]
    */ 
   char **_lst;
} BlobList;

/**
 * \struct BlobRow
 * \brief One row of a BlobTable
 */
typedef struct {
   /** \brief Number of cells (columns) in the row */
   int    _nCel;
   /**
    * \brief Row values
    *
    * Equivalent to char *_cels[BlobRow._nCel]
    */
   char **_cels;
} BlobRow;

/**
 * \struct BlobTable
 * \brief A table of results returned from a query
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_BlobTable
 */
typedef struct {
   /** \brief Message Header */
   qHdr     _hdr;
   /**
    * \brief Last Blob packet received
    *
    * This ALWAYS equals BlobList._totPkt
    */
   int      _nPkt;
   /**
    * \brief Total Blob packets received
    *
    * This ALWAYS equals BlobList._nPkt
    */
   int      _totPkt;
   /** \brief Number of rows in table */
   int      _nRow;
   /** \brief List name */
   char     _name[32];
   /** \brief Column header names */
   BlobRow  _colHdr;
   /**
    * \brief The Table
    *
    * Equivalent to BlobRow *_rows[BlobTable._nRow]
    */
   BlobRow *_rows;
} BlobTable;


/**************************
 * _mtEQUITY
 *************************/
/**
 * \struct EQBbo
 * \brief Equity BBO Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_EQBbo
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Limit Up / Down Flags */
   int    _luldFlags;
   /** \brief Quote Condition */
   char   _cond;
   /** \brief Quote Flags */
   int    _flags;
   /** \brief Bid Market Center */
   char  *_bidMktCtr;
   /** \brief Ask Market Center */
   char  *_askMktCtr;
} EQBbo;

/**
 * \struct EQQuote
 * \brief Equity Market Center Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_EQQuote
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Quote Condition */
   char   _cond;
   /** \brief Quote Flags */
   int    _flags;
   /** \brief Market Center */
   char  *_mktCtr;
} EQQuote;

/**
 * \struct EQBboMM
 * \brief Equity BBO Quote Market Maker Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_EQBboMM
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Quote Condition */
   char   _cond;
   /** \brief Quote Flags */
   int    _flags;
   /** \brief Market Center */
   char  *_mktCtr;
   /** \brief Bid Market Maker */
   char  *_bidMmid;
   /** \brief Ask Market Maker */
   char  *_askMmid;
} EQBboMM;

/**
 * \struct EQQuoteMM
 * \brief Equity Market Maker Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_EQQuoteMM
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Quote Condition */
   char   _cond;
   /** \brief Quote Flags */
   int    _flags;
   /** \brief Market Center */
   char  *_mktCtr;
   /** \brief Market Maker */
   char  *_mmid;
} EQQuoteMM;

/**
 * \struct EQTrade
 * \brief Equity Trade Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_EQTrade
 */
typedef struct {
   /** \brief Message Header */
   qHdr    _hdr;
   /** \brief Condition Table */
   int     _condTable;
   /** \brief Unique Trade ID */
   long    _trdID;
   /** \brief Trade Price */
   double  _trdPrc;
   /** \brief Trade Volume */
   long    _trdVol;
   /** \brief Trade Time - Millis since midnight */
   long    _trdTime;
   /** \brief Eligibility Flags */
   int     _eligFlags;
   /** \brief Settlement Type */
   int     _setlType;
   /** \brief Report Type */
   int     _rptType;
   /** \brief Report Detail */
   int     _rptDetail;
   /** \brief Report Flags */
   int     _rptFlags;
   /** \brief Participant Volume */
   long    _partVol;
   /** \brief Consolidated Volume */
   long    _consVol;
   /** \brief Net Change */
   double  _netChg;
   /** \brief Pct Change */
   double  _pctChg;
   /** \brief Daily High */
   double  _high;
   /** \brief Daily High Time - Millis since midnight */
   long    _highTime;
   /** \brief Daily Low */
   double  _low;
   /** \brief Daily Low Time - Millis since midnight */
   long    _lowTime;
   /** \brief Market Center */
   char   *_mktCtr;
   /** \brief Daily Accumulated Volume */
   long    _acVol;
   /** \brief Daily Turnover */
   long    _tnOvr;
   /** \brief Open Price */
   double  _openPrc;
   /** \brief Open Volume */
   long    _openVol;
   /** \brief Open Time - Millis since midnight */
   long    _openTime;
   /** \brief Trade Direction : UP, DOWN, UNCHANGED */
   char    _prcTck;
   /** \brief Volume Weighted Average Price */
   double  _vwap;
   /*
    * Extended Trade Fields
    */
   /** \brief 1 if extended fields such as _trdPrc_ext are available */
   char    _bExtFlds;
   /** \brief Trade Price - Extended Hours */
   double  _trdPrc_ext;
   /** \brief Trade Volume - Extended Hours */
   long    _trdVol_ext;
   /** \brief Trade Time - Millis since midnight - Extended Hours */
   long    _trdTime_ext;
   /** \brief Net Change - Extended Hours */
   double  _netChg_ext;
   /** \brief Pct Change - Extended Hours */
   double  _pctChg_ext;
   /** \brief Market Center - Extended Hours */
   char   *_mktCtr_ext;
   /** \brief Trade Direction : UP, DOWN, UNCHANGED - Extended Hours */
   char    _prcTck_ext;
} EQTrade;

/**
 * \struct EQLimitUpDn
 * \brief Equity Limit Up / Down Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_EQLimitUpDn
 */
typedef struct {
   /** \brief Message Header */
   qHdr    _hdr;
   /** \brief Lower Band */
   double  _lowerPrice;
   /** \brief Upper Band */
   double  _upperPrice;
   /** \brief Effective Time - Millis since midnight */
   long    _time;
   /** \brief Price Band Indicator */
   char    _indicator;
} EQLimitUpDn;


/**************************
 * _mtOPTION
 *************************/
/**
 * \struct OPBbo
 * \brief Options BBO Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_OPBbo
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Quote Condition */
   char   _qteCond;
   /** \brief Quote Flags */
   int    _qteFlags;
   /** \brief BBO Market Center */
   char  *_mktCtr;
   /** \brief Bid Market Center */
   char  *_bidMktCtr;
   /** \brief Ask Market Center */
   char  *_askMktCtr;
} OPBbo;

/**
 * \struct OPQuote
 * \brief Options Market Center Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_OPQuote
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Quote Condition */
   char   _qteCond;
   /** \brief Quote Flags */
   int    _qteFlags;
   /** \brief Market Center */
   char  *_mktCtr;
} OPQuote;

/**
 * \struct OPQuotAppendagee
 * \brief Options Market Center Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt ==
 * QuoddMsgType::qMsg_OPQuote
 */
typedef struct {
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Bid Time - Millis since Midnight */
   int    _bidTime;
   /** \brief Ask Time - Millis since Midnight */
   /** \brief Ask Market Center */
   int    _askTime;
   /** \brief Bid Market Center */
   char  *_bidMktCtr;
   /** \brief Ask Market Center */
   char  *_askMktCtr;
} OPQuoteAppendage;

/**
 * \struct OPTrade
 * \brief Options Trade Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_OPTrade
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Trade Price */
   double  _trdPrc;
   /** \brief Trade Volume */
   long    _trdVol;
   /** \brief Trade Conditions */
   char   _trdCond;
   /** \brief Trade Flags */
   int    _trdFlags;
   /** \brief Net Change */
   double  _netChg;
   /** \brief Pct Change */
   double  _pctChg;
   /** \brief Daily High */
   double  _high;
   /** \brief Daily High Time - Millis since midnight */
   long    _highTime;
   /** \brief Daily Low */
   double  _low;
   /** \brief Daily Low Time - Millis since midnight */
   long    _lowTime;
   /** \brief Market Center */
   char   *_mktCtr;
   /** \brief Daily Accumulated Volume */
   long    _acVol;
   /** \brief Daily Turnover */
   long    _tnOvr;
   /** \brief Open Price */
   double  _openPrc;
   /** \brief Open Volume */
   long    _openVol;
   /** \brief Open Time - Millis since midnight */
   long    _openTime;
   /** \brief Trade Direction : UP, DOWN, UNCHANGED */
   char    _prcTck;
   /** \brief Volume Weighted Average Price */
   double  _vwap;
   /*
    * Extended UF 1.0 Protocol
    */
   /** \brief Trade ID = OPRA Packet SeqNum */
   long    _trdID;
   /** \brief Unique Trade ID = ( _trdID * 100 ) + PositionInPkt  */
   long    _trdIDuniq;
   /** \brief Quote Appendage : Contract Quote */
   QFQuoteAppendage _option;
   /** \brief Quote Appendage : Underlyer Quote */
   QFQuoteAppendage _equity;
} OPTrade; 

/**************************
 * _mtINDEX
 *************************/
/**
 * \struct IDXValue
 * \brief Index Value Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_IDXValue
 */
typedef struct {
   /** \brief Message Header */
   qHdr        _hdr;
   /** \brief Index Value */
   double      _value;
   /** \brief Shared */
   long        _iShares;
   /** \brief Session ID */
   int         _session;
   /** \brief Calc Time - Millis since midnight */
   long        _calcTime;
   /** \brief Calc Method */
   int         _iCalc;
   /** \brief Calc Method Name */
   const char *_calcMethod;
   /** \brief Net Change */
   double       _netChg;
   /** \brief Pct Change */
   double       _pctChg;
   /** \brief Open Price */
   double       _open;
   /** \brief Daily High */
   double       _high;
   /** \brief Daily Low */
   double       _low;
} IDXValue;

/**
 * \struct IDXSummary
 * \brief Index Summary Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_IDXSummary
 */
typedef struct {
   /** \brief Message Header */
   qHdr        _hdr;
   /** \brief Open Price */
   double       _open;
   /** \brief Daily High */
   double       _high;
   /** \brief Daily Low */
   double       _low;
   /** \brief Closing Price */
   double      _close;
   /** \brief Net Change */
   double      _netChg;
   /** \brief Summary Type Indicator */
   char        _iType;
   /** \brief Summary Type Description */
   const char *_summType;
   /** \brief Index Volume */
   u_int64_t   _volume;
   /** \brief Volume Type : 'A' = Actual; 'E' = Expected; ' ' = Unspecified */
   char        _volumeType;
} IDXSummary;

/**************************
 * _mtFUTURE
 *************************/
/**
 * \struct FUTRQuote
 * \brief Futures Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_FUTRQuote
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Bid Quote Condition */
   u_char _bidCond;
   /** \brief Ask Quote Condition */
   u_char _askCond;
   /** \brief Quote Flags */
   int    _qteFlags;
   /** \brief Session ID */
   u_char _sessionId;
   /** \brief Volume Indicator */
   u_char _volInd;
   /** \brief Exceptional Quotation Indicator */
   u_char _excInd;
   /** \brief As-Of Sequence */
   int    _asofSeq;
   /** \brief Combo Code */
   long   _comboCode;
} FUTRQuote;

/**
 * \struct FUTRTrade
 * \brief Futures Trade Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_FUTRTrade
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Trade Price */
   double  _trdPrc;
   /** \brief Trade Volume */
   long    _trdVol;
   /** \brief Trade Conditions */
   char   _trdCond;
   /** \brief Trade Flags */
   int    _trdFlags;
   /** \brief Session ID */
   u_char _sessionId;
   /** \brief Net Change */
   double  _netChg;
   /** \brief Pct Change */
   double  _pctChg;
   /** \brief Daily High */
   double  _high;
   /** \brief Daily High Time - Millis since midnight */
   long    _highTime;
   /** \brief Daily Low */
   double  _low;
   /** \brief Daily Low Time - Millis since midnight */
   long    _lowTime;
   /** \brief Market Center */
   char   *_mktCtr;
   /** \brief Daily Accumulated Volume */
   long    _acVol;
   /** \brief Daily Turnover */
   long    _tnOvr;
   /** \brief Open Price */
   double  _openPrc;
   /** \brief Open Volume */
   long    _openVol;
   /** \brief Open Time - Millis since midnight */
   long    _openTime;
   /** \brief Trade Direction : UP, DOWN, UNCHANGED */
   char    _prcTck;
} FUTRTrade;

/**
 * \struct FUTRMisc
 * \brief Futures Hi/Lo/Last or Open Interest Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_FUTRMisc
 */
typedef struct {
   /** \brief Message Header */
   qHdr    _hdr;
   /** \brief True if Hi/Lo/Last; false if Open Interest */
   char    _bHiLo;
   /** \brief Type Code */
   char    _typeCode;
   /** \brief Session ID */
   char    _sessionId;
   /** \brief High Price, if _bHiLo is true */
   double  _highPrc;
   /** \brief Sale Condition on High, if _bHiLo is true */
   char    _highPrcInd;
   /** \brief Low Price, if _bHiLo is true */
   double  _lowPrc;
   /** \brief Sale Condition on Low, if _bHiLo is true */
   char    _lowPrcInd;
   /** \brief Last Price, if _bHiLo is true */
   double  _lastPrc;
   /** \brief Sale Condition on Last, if _bHiLo is true */
   char    _lastPrcInd;
   /** \brief Open Interest, if _bHiLo is false */
   long    _openInt;
   /** \brief _openInt code 'A'=Actual; 'E'=Estimated, if _bHiLo is false */
   char    _openIntInd;
} FUTRMisc;


/**************************
 * _mtBOND
 *************************/
/**
 * \struct BONDQuote
 * \brief Bond Quote Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_BONDQuote
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Bid Price */
   double _bid;
   /** \brief Bid Size */
   int    _bidSize;
   /** \brief Ask Price */
   double _ask;
   /** \brief Ask Size */
   int    _askSize;
   /** \brief Quote Condition */
   int    _qteCond;
   /** \brief Quote Flags */
   int    _qteFlags;
   /** \brief Session ID */
   u_char _sessionId;
   /** \brief Bid Yield */
   double _bidYield;
   /** \brief Ask Yield */
   double _askYield;
   /** \brief Market Center */
   char   *_mktCtr;
} BONDQuote;

/**
 * \struct BONDTrade
 * \brief Bond Trade Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_BONDTrade
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Trade Price */
   double  _trdPrc;
   /** \brief Trade Volume */
   long    _trdVol;
   /** \brief Trade Conditions */
   int     _trdCond;
   /** \brief Trade Flags */
   int    _trdFlags;
   /** \brief Net Change */
   double  _netChg;
   /** \brief Pct Change */
   double  _pctChg;
   /** \brief Daily High */
   double  _high;
   /** \brief Daily High Time - Millis since midnight */
   long    _highTime;
   /** \brief Daily Low */
   double  _low;
   /** \brief Daily Low Time - Millis since midnight */
   long    _lowTime;
   /** \brief Market Center */
   char   *_mktCtr;
   /** \brief Daily Accumulated Volume */
   long    _acVol;
   /** \brief Daily Turnover */
   long    _tnOvr;
   /** \brief Volume Weighted Average Price */
   double  _vwap;
} BONDTrade;


/**************************
 * _mtFUND
 *************************/
/**
 * \struct FUNDnav
 * \brief Mutual Fund Net Asset Value message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_FUNDnav
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Indicates presence of optional fields */
   int    _flags;
   /** \brief Reporting Period */ 
   char   _reportType;
   /** \brief Footnote code, from source */
   char  *_footnotes;
   /** \brief Net Asset Value (optional) */
   double _nav;
   /** \brief _fundSubFUNDNAV : Offer / Market Price (optional) */
   double _price;
   /** \brief _fundSubFUNDNAV : Wrap Price (optional) */
   double _wrapPrice;
   /** \brief _fundSubFUNDNAV : Total Net Assets (Optional) */
   double _netAssets;
   /** \brief _fundSubFUNDNAV : Current Yield */
   double _yield;
   /** \brief _fundSubFUNDNAV : Estimated Long Term Return */
   double _eltr;
   /** \brief _fundSubFUNDNAV : Accrued Interest */
   double _accruedInt;
   /** \brief _fundSubFUNDNAV : Daily Dividend Indicator : Y = Fat; N = No */
   char   _divIndicator;
   /** \brief _fundSubFUNDNAV : Daily Divident Factor */
   double _dailyDividend;
   /** \brief _fundSubFUNDNAV : Date to whith entry should be applied */
   long   _entryDate;
   /** \brief _fundSubFUNDNAV : Previous Close */
   double _close;
   /** \brief _fundSubFUNDNAV : Net Change */
   double _netChg;
   /** \brief _fundSubFUNDNAV : Percent Change */
   double _pctChg;
   /*
    * _fundSubMMNAV
    */
   /** \brief _fundSubMMNAV : Average Maturity */
   int    _avgMaturity;
   /** \brief _fundSubMMNAV : Average Life */
   int    _avgLife;
   /** \brief _fundSubMMNAV : 7-day Gross Yield */
   double _yield7DayGross;
   /** \brief _fundSubMMNAV : 7-day Subsidized Yield */
   double _yield7DaySubsidized;
   /** \brief _fundSubMMNAV : 7-day Annualized Yield */
   double _yield7DayAnnualized;
   /** \brief _fundSubMMNAV : 30-day Yield */
   double _yield30Day;
   /** \brief _fundSubMMNAV : 30-day Yield Date - MMDDYYYY */
   long   _yield30DayDate;
   /** \brief _fundSubMMNAV : Total Assets */
   double _totalNetAssets;
   /*
    * _fundSubCAPDISTRO
    */
   /** \brief _fundSubCAPDISTRO : Action */
   u_char _action;
   /** \brief _fundSubCAPDISTRO : Short-Term Gain */
   double _shortTermGain;
   /** \brief _fundSubCAPDISTRO : Long-Term Gain */
   double _longTermGain;
   /** \brief _fundSubCAPDISTRO : Unallocated Distribution */
   double _unallocatedDist;
   /** \brief _fundSubCAPDISTRO : Return on Capital */
   double _returnOnCapital;
   /** \brief _fundSubCAPDISTRO : Ex-Distribution Date */
   long   _exDate;
   /** \brief _fundSubCAPDISTRO : Record Date */
   long   _recordDate;
   /** \brief _fundSubCAPDISTRO : Payment Date */
   long   _paymentDate;
   /** \brief _fundSubCAPDISTRO : Re-investment Date */
   long   _reinvestDate;
   /*
    * _fundSubDIVID_INT
    */
   /** \brief _fundSubDIVID_INT : Distribution Type */
   u_char _distroType;
   /** \brief _fundSubDIVID_INT : Total Cash Distribution */
   double _totalCashDist;
   /** \brief _fundSubDIVID_INT : Non-Qualified Cash Distribution */
   double _nonQualCashDist;
   /** \brief _fundSubDIVID_INT : Qualified Cash Distribution */
   double _qualCashDist;
   /** \brief _fundSubDIVID_INT : Tax-Free Cash Distribution */
   double _taxFreeCashDist;
   /** \brief _fundSubDIVID_INT : Ordinary Foreign Tax Credit */
   double _ordForeignTaxCredit;
   /** \brief _fundSubDIVID_INT : Qualified Foreign Tax Credit */
   double _qualForeignTaxCredit;
   /** \brief _fundSubDIVID_INT : Stock Dividend */
   double _stockDividend;
   /** \brief All : Fund Type */
   u_char _fundType;
   /** \brief All : Fund Code */
   u_char _fundCode;
} FUNDnav;

/**************************
 * _mtSubIMG
 *************************/
/**
 * \struct Image
 * \brief Initial Image Message
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_Image.  This is the 1st message you receive on 
 * a stream after calling Quodd_Subscribe().
 */
typedef struct {
   /*
    * All Asset Types
    */
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Time of last update - Unix time in seconds */
   long   _tUpd;
   /** \brief UltraCache channel number */
   int    _nChan;
   /** \brief Number of updates received today */
   int    _nUpd;
   /** \brief Number of trades received today */
   int    _nTrade;
   /** \brief Today's UltraFeed locate code */
   int    _locCode;
   /** \brief UltraCache database index */
   int    _dbIdx;
   /** \brief Record description */
   char   _desc[32];
   /** \brief Primary Market Center */
   char   _priMktCtr[16];
   /*
    * _mtEQUITY / _mtOPTION - Meta Data
    */
   /** \brief Lot Size (Equity / Option Only - Meta Data) */
   int    _lotSize;
   /** \brief Most Recent Quote Flags (Equity / Option Only - Meta Data) */
   int    _qteFlags;
   /** \brief Tier (Equity / Option Only - Meta Data) */
   u_char _tier;
   /** \brief Disclose Status (Equity / Option Only - Meta Data) */
   u_char _discloseSts;
   /** \brief Asset Class (Equity / Option Only - Meta Data) */
   u_char _assetClass;
   /** \brief Authenticity (Equity / Option Only - Meta Data) */
   u_char _authenticity;
   /** \brief Is Short Sale Restricted (Equity / Option Only - Meta Data) */
   u_char _isShortSaleRestr;
   /** \brief Caveat Emptor (Equity / Option Only - Meta Data) */
   u_char _isCaveatEmptor;
   /** \brief Most Recent Trade Direction (Equity / Option Only - Meta Data) */
   u_char _prcTck;
   /** \brief Halted (Equity / Option Only - Meta Data) */
   u_char _halted;
   /** \brief Reg SHO (Equity / Option Only - Meta Data) */
   u_char _regSHO;
   /** \brief Unsolicited (Equity / Option Only - Meta Data) */
   u_char _unsolicited;
   /** \brief Is BB Quoted (Equity / Option Only - Meta Data) */
   u_char _BBquoted;
   /** \brief Is Message Disabled (Equity / Option Only - Meta Data) */
   u_char _msgDisabled;
   /** \brief Market Category (Equity / Option Only - Meta Data) */
   u_char _mktCat;
   /** \brief Issue Type (Equity / Option Only - Meta Data) */
   u_char _issueType;
   /** \brief Financial Status (Equity / Option Only - Meta Data) */
   u_char _financialSts;
   /** \brief TSE Class (Equity / Option Only - Meta Data) */
   u_char _TSEclass;
   /*
    * _mtEQUITY / _mtOPTION - Quote / Trade
    */
   /** \brief Quote Condition (Equity / Option Only - Quote & Trade Data) */
   u_char _qteCond;
   /** \brief Bid Price (Equity / Option Only - Quote & Trade Data) */
   double _bid;
   /** \brief Bid Size (Equity / Option Only - Quote & Trade Data) */
   int    _bidSize;
   /** \brief Bid Time (Equity / Option Only - Quote & Trade Data) */
   long   _bidTime;
   /** \brief Bid Market Center (Equity / Option Only - Quote & Trade Data) */
   char  *_bidMktCtr;
   /** \brief Ask Price (Equity / Option Only - Quote & Trade Data) */
   double _ask;
   /** \brief Ask Size (Equity / Option Only - Quote & Trade Data) */
   int    _askSize;
   /** \brief Ask Time (Equity / Option Only - Quote & Trade Data) */
   long   _askTime;
   /** \brief Ask Market Center (Equity / Option Only - Quote & Trade Data) */
   char  *_askMktCtr;
   /** \brief Trade Market Center (Equity / Option Only - Quote & Trade Data) */
   char  *_trdMktCtr;
   /** \brief Trade Condition (Equity / Option Only - Quote & Trade Data) */
   u_char _trdCond;
   /** \brief Last Trade Price (Equity / Option Only - Quote & Trade Data) */
   double _trdPrc;
   /** \brief Last Trade Volume (Equity / Option Only - Quote & Trade Data) */
   int    _trdVol;
   /** \brief Last Trade Time (Equity / Option Only - Quote & Trade Data) */
   QFTime _trdTime;
   /** \brief Total Daily Volume (Equity / Option Only - Quote & Trade Data) */
   long   _acVol;
   /** \brief Total Turnover (Equity / Option Only - Quote & Trade Data) */
   long   _tnOvr;
   /** \brief Open Price (Equity / Option Only - Quote & Trade Data) */
   double _open;
   /** \brief Open Volume (Equity / Option Only - Quote & Trade Data) */
   int    _openVol;
   /** \brief Open Time (Equity / Option Only - Quote & Trade Data) */
   long   _openTime;
   /** \brief High Price (Equity / Option Only - Quote & Trade Data) */
   double _high;
   /** \brief High Time (Equity / Option Only - Quote & Trade Data) */
   long   _highTime;
   /** \brief Low Price (Equity / Option Only - Quote & Trade Data) */
   double _low;
   /** \brief Low TIme (Equity / Option Only - Quote & Trade Data) */
   long   _lowTime;
   /** \brief Yesterday's Close (Equity / Option Only - Quote & Trade Data) */
   double _close;
   /** \brief Close Date (Equity / Option Only - Quote & Trade Data) */
   int    _closeDate;
   /** \brief Net Change (Equity / Option Only - Quote & Trade Data) */
   double _netChg;
   /** \brief Percent Change (Equity / Option Only - Quote & Trade Data) */
   double _pctChg;
   /*
    * _mtINDEX
    */
   /** \brief Index Value (Index Only) */
   double _value;
   /** \brief Index Value Calc Method (Index Only) */
   u_char _calcMethod;
   /** \brief 1 if held (Index Only) */
   u_char _isHeld;
   /** \brief As-Of Open Price (Index Only) */
   double _asofOpen;
   /** \brief As-Of High Price (Index Only) */
   double _asofHigh;
   /** \brief As-Of Low Price (Index Only) */
   double _asofLow;
   /** \brief As-Of Closing Price (Index Only) */
   double _asofClose;
   /** \brief As-Of Net Change (Index Only) */
   double _asofNetChg;
   /** \brief As-Of Date (Index Only) */
   long   _asofDate;
   /** \brief As-Of Action (Index Only) */
   u_char _asofAction;
   /** \brief Dividend (Index Only) */
   double _dividend;
   /** \brief Settlement Value (Index Only) */
   double _settleValue;
   /** \brief Settlement Time (Index Only) */
   long   _settleTime;
   /** \brief Settlement Summary (Index Only) */
   double _settleSumm;
   /** \brief ETP Intra Day Price (Index Only) */
   double _ETPIntraDay;
   /** \brief 1 if ETP Is Held (Index Only) */
   u_char _ETPisHeld;
   /** \brief ETP Estimated Cash Per Creation Unit (Index Only) */
   double _ETPEstCashPCU;
   /** \brief ETP Total Cash Per Creation Unit (Index Only) */
   double _ETPTotCashPCU;
   /** \brief ETP Net Asset Value (Index Only) */
   double _ETPNav;
   /** \brief ETP Shares Outstanding (Index Only) */
   long   _ETPSharesOut;
   /** \brief ETP Shares Outstanding (Index Only) */
   double _ETPDividend;
   /*
    * _mtFUND
    */
   /** \brief Fund Type (Mutual Fund Only) */
   u_char _fundType;
   /** \brief Fund Code (Mutual Fund Only) */
   u_char _fundCode;
   /** \brief Frequency (Mutual Fund Only) */
   u_char _frequency;
   /** \brief Report Type (Mutual Fund Only) */
   u_char _reportType;
   /** \brief Par Value (Mutual Fund Only) */
   double _parValue;
   /** \brief Footnotes (Mutual Fund Only) */
   char  *_footnotes;
   /** \brief Net Asset Value (Mutual Fund Only) */
   double _fundNav;
   /** \brief Fund Price (Mutual Fund Only) */
   double _fundPrc;
   /** \brief Fund Wrap Price (Mutual Fund Only) */
   double _fundWrapPrc;
   /** \brief Fund Net Assets (Mutual Fund Only) */
   long   _fundNetAssets;
   /** \brief Fund Yield (Mutual Fund Only) */
   double _yield;
   /** \brief Fund Estimated Long-Term Return (Mutual Fund Only) */
   double _eltr;
   /** \brief Fund Accrued Interest (Mutual Fund Only) */
   double _accruedInt;
   /** \brief Fund Daily Dividend Indicator (Mutual Fund Only) */
   u_char  _dailyDivInd;
   /** \brief Fund Daily Dividend (Mutual Fund Only) */
   double _dailyDiv;
   /** \brief Fund Entry Date (Mutual Fund Only) */
   long   _entryDate;
   /** \brief Fund Average Maturity (Mutual Fund Only) */
   int    _avgMaturity;
   /** \brief Fund Average Life (Mutual Fund Only) */
   int    _avgLife;
   /** \brief Fund 7-day Gross Yield (Mutual Fund Only) */
   double _yield7DayGross;
   /** \brief Fund 7-day Subsidized Yield (Mutual Fund Only) */
   double _yield7DaySubsidized;
   /** \brief Fund 7-day Annualized Yield (Mutual Fund Only) */
   double _yield7DayAnnualized;
   /** \brief Fund 30-day Yield (Mutual Fund Only) */
   double _yield30Day;
   /** \brief Fund 30-day Yield Date (Mutual Fund Only) */
   long   _yield30DayDate;
   /** \brief Fund Short-term Gain (Mutual Fund Only) */
   double _shortTermGain;
   /** \brief Fund Long-term Gain (Mutual Fund Only) */
   double _longTermGain;
   /** \brief Fund Unallocated Distribution (Mutual Fund Only) */
   double _unallocatedDist;
   /** \brief Fund Return on Capital (Mutual Fund Only) */
   double _returnOnCapital;
   /** \brief Fund Ex-Distribution Date (Mutual Fund Only) */
   long   _exDate;
   /** \brief Fund Record Date (Mutual Fund Only) */
   long   _recordDate;
   /** \brief Fund Payment Date (Mutual Fund Only) */
   long   _paymentDate;
   /** \brief Fund Reinvestment Date (Mutual Fund Only) */
   long   _reinvestDate;
   /** \brief Fund Closing Price (Mutual Fund Only) */
   double _fundClose;
   /** \brief Fund Net Change (Mutual Fund Only) */
   double _fundNetChg;
   /** \brief Fund Pct Change (Mutual Fund Only) */
   double _fundPctChg;

   /*
    * TODO : _mtFUTURE / _mtBOND
    */

   /*
    * Extended Trade Fields
    */
   /** \brief Trade Price - Extended Hours */
   double  _trdPrc_ext;
   /** \brief Trade Volume - Extended Hours */
   long    _trdVol_ext;
   /** \brief Trade Time - Millis since midnight - Extended Hours */
   long    _trdTime_ext;
   /** \brief Net Change - Extended Hours */
   double  _netChg_ext;
   /** \brief Pct Change - Extended Hours */
   double  _pctChg_ext;
   /** \brief Market Center - Extended Hours */
   char   *_mktCtr_ext;
   /** \brief Trade Direction : UP, DOWN, UNCHANGED - Extended Hours */
   char    _prcTck_ext;
} Image;


/**************************
 * Quality of Service
 *************************/
/**
 * \struct QoS
 * \brief Quality of Service
 *
 * You receive this library-generated message in a QuoddMsg when the 
 * multicast line drops / recovers packets.  For this message, the
 * QuoddMsg::_mt == QuoddMsgType::qMsg_QoS
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
   /** \brief Current multicast Sequence Number */
   u_int64_t _seqNum;
   /** \brief Num msgs dropped in this gap */
   int        _numDropped;
   /** \brief Total Msgs dropped since we joined multicast stream */
   u_int64_t _totalDropped;
   /** \brief Gap Time - Microseconds */
   long      _gapTimeUs;
} QoS;

/**************************
 * Heartbeat
 *************************/
/**
 * \struct Heartbeat
 * \brief Unsolicited Heartbeat
 *
 * You receive this in a QuoddMsg when the QuoddMsg::_mt == 
 * QuoddMsgType::qMsg_Hearbeat
 */
typedef struct {
   /** \brief Message Header */
   qHdr   _hdr;
} Heartbeat;

#endif /* __QOD_MESSAGES_H */
