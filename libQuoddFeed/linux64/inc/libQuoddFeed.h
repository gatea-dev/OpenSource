/******************************************************************************
*
*  libQuoddFeed.h
*     'C' interface library to QuoddFeed
*
*  REVISION HISTORY:
*     12 DEC 2012 jcs  Created.
*     14 DEC 2012 jcs  Build  2: _sessCbk
*     17 JAN 2013 jcs  Build  3: Image
*      7 MAR 2013 jcs  Build  4: FUTRTrade / FUTRQuote
*     10 MAR 2013 jcs  Build  5: BlobTable / GetFuturesChain()
*     18 OCT 2013 jcs  Build  8: QuoddMsg._tRead / _tMsg
*     29 JAN 2014 jcs  Build  9: Quodd_QueryTSQ(); Status
*      2 SEP 2014 jcs  Build 10: qMsg_IDXValue
*     15 DEC 2014 jcs  Build 11: int Quodd_Subscribe(); EQLimitUpDn
*      3 FEB 2015 jcs  Build 13: Quodd_GetFuturesOptionChain(); Compression
*     12 MAR 2015 jcs  Build 14: Mutex.hpp; Funds
*     24 MAR 2015 jcs  Build 15: FUTRMisc; Quodd_DumpDebugStats()
*     20 APR 2015 jcs  Build 16: Quodd_BulkSubscribe()
*      1 MAY 2015 jcs  Build 17: Multicast; QoS; Quodd_GetChannel()
*      6 JUL 2015 jcs  Build 19: qIoctl_getFd
*     10 FEB 2016 jcs  Build 21: qMsg_BONDXxx
*     19 APR 2016 jcs  Build 22: mainpage doc
*     18 JAN 2018 jcs  Build 23: Heartbeat
*     17 DEC 2019 jcs  Build 24: VOID_PTR
*     14 JUL 2022 jcs  Build 25: bld.cpp
*
*  (c) 1994-2022, Gatea Ltd.
******************************************************************************/

/**
 * \mainpage libQuoddFeed API Reference Manual
 *
 * libQuoddFeed allows you to consume real-time from the UltraCache server.
 * Real-time data is delivered to you as messages that are converted into
 * C structures, for example:
 *
 * struct | Description
 * ---- | ---
 * EQBbo | Equity BBO Quote
 * EQQuote | Equity Market Center Quote
 * OPTrade | Options Trade
 */

#ifndef __LIB_QUODD_H
#define __LIB_QUODD_H
#include <bld.cpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* QuoddFeed Messages */

#include <Messages.h>
#include <MsgTypes.h>

/* COM / .NET is __stdcall */

/** 
 * \brief Handle different callback calling conventions on all platforms
 *
 * In other words, Microsoft sucks.
 */
#ifdef WIN32
#define QODAPI __stdcall
#pragma warning(disable:4018)
#pragma warning(disable:4101)
#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4355)
#pragma warning(disable:4996)
#else
#define QODAPI
#endif /* WIN32 */

/* Useful Macros */

/** \brief Find min of 2 values */
#define gmin( a,b )          ( ((a)<=(b)) ? (a) : (b) )
/** \brief Find max of 2 values */
#define gmax( a,b )          ( ((a)>=(b)) ? (a) : (b) )
/** \brief true if b is in the range [ a,c ] */
#define InRange( a,b,c )     ( ((a)<=(b)) && ((b)<=(c)) )
/** \brief Too lazy to type in 1024 sometimes */
#define K                    1024

#define VOID_PTR void *)(size_t

/* Data Structures */

/** \brief The context number of an initialized channel */
typedef int Quodd_Context;

/**
 * \enum QuoddState 
 * \brief The state of a QuoddFeed channel
 */
typedef enum {
   /** \brief Channel is UP */
   qod_up = 0,
   /** \brief Channel is DOWN */
   qod_down
} QuoddState;

/** 
 * \enum QuoddMsgType
 * \brief QuoddFeed Message Type
 * 
 * \see QuoddMsg
 */
typedef enum {
   qMsg_undef  = 0,
   /** \brief Status - Stream status message */
   qMsg_Status,
   /** \brief UCStatus - UltraCache Status message */
   qMsg_UCStatus,
   /** \brief BlobList - Query / Response List */
   qMsg_BlobList,
   /** \brief BlobTable - Query / Response Table */
   qMsg_BlobTable,
   /** \brief Image - Initial Image */
   qMsg_Image,
   /** \brief EQBbo - Equity BBO Quote */
   qMsg_EQBbo,
   /** \brief EQBboMM - Equity BBO Market Maker Quote */
   qMsg_EQBboMM,
   /** \brief EQQuote - Equity Market Center Quote */
   qMsg_EQQuote,
   /** \brief EQQuoteMM - Equity Quote Market Maker */
   qMsg_EQQuoteMM,
   /** \brief EQTrade - Equity Trade */
   qMsg_EQTrade,
   /** \brief EQLimitUpDn - Equity Limit Up / Down */
   qMsg_EQLimitUpDn,
   /** \brief OPBbo - Options BBO Quote */
   qMsg_OPBbo,
   /** \brief OPQuote - Options Market Center Quote */
   qMsg_OPQuote,
   /** \brief OPTrade - Options Trade */
   qMsg_OPTrade,
   /** \brief FUTRQuote - Futures Quote */
   qMsg_FUTRQuote,
   /** \brief FUTRTrade - Futures Trade */
   qMsg_FUTRTrade,
   /** \brief IDXValue - Index Value */
   qMsg_IDXValue,
   /** \brief IDXSummary - Index Summary */
   qMsg_IDXSummary,
   /** \brief IDXSetlValu - Index Settlement Value */
   qMsg_IDXSetlValu,
   /** \brief IDXSetlSumm - Index Settlement Summary */
   qMsg_IDXSetlSumm,
   /** \brief FUNDnav - Mutual Fund Net Asset Value */
   qMsg_FUNDnav,
   /** \brief FUTRMisc - Futures Hi/Lo/Last or Open Interest */
   qMsg_FUTRMisc,
   /** \brief QoS - Quality of Service (message drops) */
   qMsg_QoS,
   /** \brief BONDQuote - Bond Quote */
   qMsg_BONDQuote,
   /** \brief BONDTrade - Bond Trade */
   qMsg_BONDTrade,
   /** \brief Heartbeat */
   qMsg_Heartbeat
} QuoddMsgType;

/**
 * \enum QuoddIoctl
 * \brief Wire protocol control commands passed to Quodd_ioctl()
 */
typedef enum {
   /**
    * \brief Set max number of items that may be opened every second.
    *
    *  \param (void *)val - Pointer to integer; Default is unlimited.
    */
   qIoctl_setOpenRate          =  0,
   /** 
    * \brief Gets the open rate set via qIoctl_setOpenRate.
    *
    * \param (void *)val - Pointer to integer to receive open rate
    */
   qIoctl_getOpenRate          =  1,
   /**
    * \brief Sets the SO_RCVBUF.
    * 
    * \param (void *)val - Pointer to integer.  Default is system 
    * dependent, e.g. /proc/sys/net/ipv4/tcp_rmem on Linux.
    */
   qIoctl_setRcvBufSize        =  2,
   /** 
    * \brief Gets the SO_RCVBUF set via qIoctl_setRcvBufSize
    *
    * \param (void *)val - Pointer to integer to receive SO_RCVBUF size.
    */
   qIoctl_getRcvBufSize        =  3,
   /** 
    * \brief Ties this thread / channel to specific CPU core
    *
    * \param (void *)val - Pointer to integer containing CPU core to tie to.  Default 
    * is 0 - Don't tie to any CPU.
    */
   qIoctl_setThreadProcessor   =  4,
   /** 
    * \brief Gets the CPU core we tied to via qIoctl_setThreadProcessor
    * 
    * \param (void *)val - Pointer to integer to receive CPU we are tied to. 
    */
   qIoctl_getThreadProcessor   =  5,
   /** 
    * \brief Sets channel to compressed
    * 
    * \param (void *)val - Pointer to integer; 1 to enable; 0 to disable
    */
   qIoctl_setCompression       =  6,
   /** 
    * \brief Gets channel to compression config set via qIoctl_setCompression
    *
    * \param (void *)val - Pointer to integer to receive compression config.
    * 1 = enabled; 0 = disabled.
    */
   qIoctl_getCompression       =  7,
   /** 
    * \brief Enable (1) / Disable (0) dispatch of Image
    *
    * \param (void *)val - Integer; 1 = enable; 0 = disable.  Default is 1.
    */
   qIoctl_dispatchImage        =  8,
   /**
    * \brief Enable (1) / Disable (0) dispatch of Status
    *
    * \param (void *)val - Integer; 1 = enable; 0 = disable.  Default is 1.
    */
   qIoctl_dispatchStatus       =  9,
   /**
    * \brief Enable (1) / Disable (0) dispatch of QoS (Quality of Service).
    *
    * \param (void *)val - Integer; 1 = enable; 0 = disable.  Default is 1.
    */
   qIoctl_dispatchQoS          = 10,
   /** 
    * \brief Set multicast address and enable reception of multicast.
    *
    * \param (void *)val - String = <ChanName>:<address>:<port>:<interface>
    */
   qIoctl_setMulticast         = 11,
   /**
    * \brief Retrieves socket file descriptor
    *
    * \param (void *)val - Pointer to integer to receive fd
    */
   qIoctl_getFd                = 12
} QuoddIoctl;

/**
 * \brief A QuoddFeed message received from the UltraCache server
 * 
 * The libQuoddFeed library will unpack messages from the wire and 
 * stuff them into this data structure.  The guts of the structure 
 * is the union of all messages known to libQuoddFeed at build time.
 * The first element in EVERY message in the union is a qHdr.
 */
typedef struct {
   /** \brief The QuoddFeed channel this message came from */
   Quodd_Context _cxt;
   /** \brief Message Type */
   QuoddMsgType  _mt;
   /** \brief Ticker name */
   const char   *_tkr;
   /** \brief User-defined argument specified in Quodd_Subscribe() */
   void         *_arg;
   /** \brief Pointer to raw QuoddFeed message */
   const char   *_rawData;
   /** \brief Raw QuoddFeed message length */
   int           _rawLen;
   /** \brief Time the buffer was read from the channel socket */
   double        _tRead;
   /** \brief Time the message was parsed from the (multi-message) buffer */
   double        _tMsg;
   /** \brief Parsed Message Contents based on QuoddMsg::_mt */
   union {
      /** \brief QuoddMsg::_mt == qMsg_Status */
      Status      _Status;
      /** \brief QuoddMsg::_mt == qMsg_BlobList */
      BlobList    _BlobList;
      /** \brief QuoddMsg::_mt == qMsg_BlobTable */
      BlobTable   _BlobTable;
      /** \brief QuoddMsg::_mt == qMsg_Image */
      Image       _Image;
      /** \brief QuoddMsg::_mt == qMsg_EQBbo */
      EQBbo       _EQBbo;
      /** \brief QuoddMsg::_mt == qMsg_EQBboMM */
      EQBboMM     _EQBboMM;
      /** \brief QuoddMsg::_mt == qMsg_EQQuote */
      EQQuote     _EQQuote;
      /** \brief QuoddMsg::_mt == qMsg_EQQuoteMM */
      EQQuoteMM   _EQQuoteMM;
      /** \brief QuoddMsg::_mt == qMsg_EQTrade */
      EQTrade     _EQTrade;
      /** \brief QuoddMsg::_mt == qMsg_EQLimitUpDn */
      EQLimitUpDn _EQLimitUpDn;
      /** \brief QuoddMsg::_mt == qMsg_OPBbo */
      OPBbo       _OPBbo;
      /** \brief QuoddMsg::_mt == qMsg_OPQuote */
      OPQuote     _OPQuote;
      /** \brief QuoddMsg::_mt == qMsg_OPTrade */
      OPTrade     _OPTrade;
      /** \brief QuoddMsg::_mt == qMsg_FUTRQuote */
      FUTRQuote   _FUTRQuote;
      /** \brief QuoddMsg::_mt == qMsg_FUTRTrade */
      FUTRTrade   _FUTRTrade;
      /** \brief QuoddMsg::_mt == qMsg_IDXValue */
      IDXValue    _IDXValue;
      /** \brief QuoddMsg::_mt == qMsg_IDXSummary */
      IDXSummary  _IDXSummary;
      /** \brief QuoddMsg::_mt == qMsg_FUNDnav */
      FUNDnav     _FUNDnav;
      /** \brief QuoddMsg::_mt == qMsg_FUTRMisc */
      FUTRMisc    _FUTRMisc;
      /** \brief QuoddMsg::_mt == qMsg_QoS */
      QoS         _QoS;
      /** \brief QuoddMsg::_mt == qMsg_BONDQuote */
      BONDQuote   _BONDQuote;
      /** \brief QuoddMsg::_mt == qMsg_BONDTrade */
      BONDTrade   _BONDTrade;
      /** \brief QuoddMsg::_mt == qMsg_Heartbeat */
      Heartbeat   _Heartbeat;
   } _v;
} QuoddMsg;

/**
 * \brief Time/Sales/Quote request
 * 
 * Used by Quodd_QueryTSQ(). Results returned asynchronously as normal 
 * messages in the QuoddAttr::_msgCbk function you passed into 
 * Quodd_Initialize().  
 */
typedef struct {
   /** \brief Unique name for this query; Returned in QuoddMsg::_tkr */
   const char *_qry;
   /** \brief Ticker name */
   const char *_tkr;
   /** \brief HH:MM:SS.mmm */
   const char *_startTime;
   /** \brief HH:MM:SS.mmm */
   const char *_endTime;
   /** \brief TRADE, QUOTE, ALL, none */
   const char *_filter;    
} QuoddTsqReq;

/**
 * \brief Connection callback definition
 * 
 * This is called when channel connects / disconnects or when session 
 * is accepted / rejected.  
 * 
 * \see QuoddAttr::_connCbk
 * \see QuoddAttr::_sessCbk
 * \param cxt - QuoddFeed channel context 
 * \param msg - Textual description of event
 * \param state - QuoddState
 */
typedef void (QODAPI *QuoddConnFcn)( Quodd_Context cxt, 
                                     const char   *msg, 
                                     QuoddState    state );

/**
 * \brief Market data message callback definition
 * 
 * This is called when a message (market data) arrives on the channel.
 *
 * \see QuoddAttr::_msgCbk 
 * \param msg - Incoming parsed QuoddMsg
 */
typedef void (QODAPI *QuoddMsgFcn)( QuoddMsg *msg );

/**
 * \brief Attributes passed to Quodd_Initialize()
 */
typedef struct {
   /** \brief UltraCache server host specified as host1:port1,host2:port2,...  */
   const char  *_pSvrHosts;
   /** \brief UltraCache server username - Supplied your Quodd sales rep */
   const char  *_pUsername;
   /** \brief UltraCache server password - Supplied your Quodd sales rep */
   const char  *_pPassword;
   /** \brief Callback when channel connects or disconnects */
   QuoddConnFcn _connCbk;
   /** \brief Callback when session established or lost */
   QuoddConnFcn _sessCbk;
   /** \brief Callback to receive real-time market data updates */
   QuoddMsgFcn  _msgCbk;
} QuoddAttr;

/* Library load / unload */

/**
 * \brief Load the library
 *
 * If not explicitly called by your application, this is called on the 
 * first call to Quodd_Initialize().
 *
 * \return 1 if loaded; 0 if already loaded
 */
char   Quodd_LoadLibrary();

/**
 * \brief Unload the library. 
 *
 * On Windows, this calls WSACleanup() if and only if no channels
 * are opened (i.e., Every Quodd_Initialize() has been Quodd_Destroy()'ed.
 *
 * \return 1 if unloaded; 0 if one or more channels still open 
 */
char   Quodd_UnloadLibrary();

/* Initialization, Connection, Control */

/**
 * \brief Initialize the connection to the QuoddFeed channel.
 *
 * This initializes the QuoddFeed channel to the UltraCache.  You connect
 * by calling Quodd_Start().
 *
 * \param attr - QuoddAttr configuration
 * \return Initialized context for Quodd_Subscribe(), Quodd_Destroy(), etc.
 */
Quodd_Context Quodd_Initialize( QuoddAttr attr );

/**
 * \brief Connect to UltraCache server.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \return Description of connection
 */
const char *Quodd_Start( Quodd_Context cxt );

/**
 * \brief Destroy connection to UltraCache server.
 *
 * \param cxt - Context from Quodd_Initialize()
 */
void Quodd_Destroy( Quodd_Context cxt );

/**
 * \brief Configures the QuoddFeed channel
 *
 * See QuoddIoctl for list of command and values
 *
 * \param cxt - Channel Context from Quodd_Initialize()
 * \param cmd - Command from QuoddIoctl
 * \param val - Command value
 */
void Quodd_ioctl( Quodd_Context cxt, QuoddIoctl cmd, void *val );

/**
 * \brief Gets max items to open each second from UltraCache.
 *
 * Equivalent to calling Quodd_ioctl() with cmd = qIoctl_getOpenRate
 *
 * \param cxt - Channel Context from Quodd_Initialize()
 * \return Max msgs per second to open from UltraCache
 */
int Quodd_GetOpenRate( Quodd_Context cxt );

/**
 * \brief Sets max items to open each second from UltraCache.
 *
 * Equivalent to calling Quodd_ioctl() with cmd = qIoctl_setOpenRate
 *
 * \param cxt - Channel Context from Quodd_Initialize()
 * \param rate - Max msgs per second to open from UltraCache
 * \return Configured Max msgs per second to open from UltraCache
 */
int Quodd_SetOpenRate( Quodd_Context cxt, int rate );

/**
 * \brief Gets SO_RCVBUF size for QuoddFeed channel
 *
 * Equivalent to calling Quodd_ioctl() with cmd = qIoctl_getRcvBufSize
 *
 * \param cxt - Channel Context from Quodd_Initialize()
 * \return Max SO_RCVBUF size
 */
int Quodd_GetRcvBufSize( Quodd_Context cxt );

/**
 * \brief Sets SO_RCVBUF size for QuoddFeed channel
 *
 * Equivalent to calling Quodd_ioctl() with cmd = qIoctl_setRcvBufSize
 *
 * \param cxt - Channel Context from Quodd_Initialize()
 * \param bufSiz - Max SO_RCVBUF size
 * \return Configured Max SO_RCVBUF size
 */
int Quodd_SetRcvBufSize( Quodd_Context cxt, int bufSiz );

/**
 * \brief Return library build description
 *
 * \return Library build description 
 */
const char   *Quodd_Version();

/* Subscription */

/**
 * \brief Open a bunch of subscription stream for a ticker from a file
 * 
 * File format is 3 tokens per line - <Channel> <Ticker> <UserDefArg> -
 * where lines are terminated by \r\n and tokens separated by the 3rd
 * argument to Quodd_BulkSubscribe().
 *
 * The 3 tokens in the file are the 3 arguments to Quodd_Subscribe() as 
 * follows:
 * - <Channel> : Channel number from Quodd_Initialize()
 * - <Ticker> : Ticker name
 * - <UserDefArg> : A user-defined argument / string that is returned in 
 * the _arg field of QuoddMsg on every update for the <Ticker>.
 *
 * The channels must have already been initialized via Quodd_Initialize() 
 * before calling Quodd_BulkSubscribe().  Market data arrives asynchronously 
 * in the channel threads just as it does for Quodd_Subscribe()
 *
 * \param filename - Filename 
 * \param sep - Token separator (comma, pipe, space, etc.)
 * \return Number of streams opened across all channels
 */
int Quodd_BulkSubscribe( char *filename, const char *sep );

/**
 * \brief Open a subscription stream for a ticker
 * 
 * You receive asynchronous market data updates in the QuoddAttr::_msgCbk
 * function you passed into Quodd_Initialize(). 
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param tkr - Ticker name 
 * \param arg - User defined argument returned in QuoddMsg struct  
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_Subscribe( Quodd_Context cxt, const char *tkr, void *arg );

/**
 * \brief Query UltraCache for time/sales/quotes time-series
 *
 * The result set is returned to you as a series of asynchronous market 
 * data updates in the QuoddAttr::_msgCbk function you passed into 
 * Quodd_Initialize().  The QuoddMsg::_tkr = QuoddTsqReq::_qry.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param tsq - Query structure
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_QueryTSQ( Quodd_Context cxt, QuoddTsqReq tsq, void *arg );

/**
 * \brief Close subscription stream for a ticker
 *
 * This stops the flow of asynchronous market data updates in the 
 * QuoddAttr::_msgCbk function you passed into Quodd_Initialize()
 * for this ticker
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param tkr - Ticker name
 */
void Quodd_Unsubscribe( Quodd_Context cxt, const char *tkr );

/* Query / Response */

/**
 * \brief Query for list of tickers for a given exchange
 *
 * Results are returned as a BlobTable structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param exch - Exchange name - BBO, ARCX, etc.
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetExchList( Quodd_Context cxt, const char *exch, void *arg );

/**
 * \brief Query for list of all indices in UltraCache
 *
 * Results are returned as a BlobTable structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetIndices( Quodd_Context cxt, void *arg );


/**
 * \brief Query for list of all mutual funds in UltraCache
 *
 * Results are returned as a BlobTable structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetMutualFunds( Quodd_Context cxt, void *arg );

/**
 * \brief Query for list of all options underlyers
 *
 * Results are returned as a BlobTable structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetOptionUnderlyers( Quodd_Context cxt, void *arg );

/**
 * \brief Query for list of options in for a given underlyer
 *
 * Results are returned as a BlobList structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param tkr - Underlyer name (e.g., AAPL, IBM, etc.)
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetOptionChain( Quodd_Context cxt, const char *tkr, void *arg );

/**
 * \brief Query for list of futures in for a contract type.
 * 
 * Results are returned as a BlobList structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param tkr - Contract type (e.g., ES, GC, etc.)
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */ 
int Quodd_GetFuturesChain( Quodd_Context cxt, const char *tkr, void *arg );

/**
 * \brief Query for list of options for a specific futures contract.
 *
 * Results are returned as a BlobTable structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param tkr - Contract (e.g., /ESG15, etc.)
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetFuturesOptionChain( Quodd_Context cxt, const char *tkr, void *arg );

/**
 * \brief Query for list of all tickers from a specific UltraCache channel.
 *
 * Results are returned as a BlobTable structure.
 *
 * \param cxt - Context from Quodd_Initialize()
 * \param chan - Channel name (e.g., OPRA1)
 * \param arg - User defined argument returned in QuoddMsg struct
 * \return Non-zero Unique Stream ID if successful; 0 if not
 */
int Quodd_GetChannel( Quodd_Context cxt, const char *chan, void *arg );

/** \brief Load multicast tickers from 3-column file.
 *
 * This function loads a flat ASCII file containing rows with 3 
 * comma-separated columns in the following order:
 *    + Ticker Name (e.g., CSCO or /ESM15)
 *    + Channel Name (EQUITY or OPRA1 OR FUTURE)
 *    + d/b Index
 * for example,
 *   CSCO,EQUITY,1
 *   O:GPOR\16M15\47.50,OPRA1,15
 *   . . .
 *
 * In multicast, the d/b index is used to uniquely identify the stream
 * and message such as CSCO or /ESM15.  THIS FILE MUST BE PROVIDED TO
 * YOU FROM THE QUODD SUPPORT STAFF AS THE d/b INDEX FOR A GIVEN
 * TICKER SUCH AS AAPL IS DIFFERENT DEPENDING ON THE SERVER YOU LISTEN
 * TO.
 *
 * \param pFile - Filename containing tickers for this channel
 * \return Number of tickers loaded
 */
int Quodd_LoadMulticastTickers( const char *pFile );

/* Utilities */

/**
 * \brief Sets the library debug level; Initiates logging
 *
 * \param pLog - Log filename
 * \param dbgLvl - Debug verbosity level
 */
void   Quodd_Log( const char *pLog, int dbgLvl );

/**
 * \brief Return current time as Unix time (secs / uS since Jan 1, 1970).
 *
 * \return Current time as Unix time (secs / uS since Jan 1, 1970).
 */
double Quodd_TimeNs();


/**
 * \brief Return current time as Unix time (secs since Jan 1, 1970).
 *
 * \return Current time as Unix time (secs since Jan 1, 1970).
 */
time_t Quodd_TimeSec();

/**
 * \brief Returns message time formatted in YYYY-MM-DD HH:MM:SS.mmm
 *
 * \param qh - Message header; 0 means current time
 * \param outbuf - Output buffer to hold formatted message time
 * \return Message time formatted in YYYY-MM-DD HH:MM:SS.mmm
 */
char  *Quodd_pDateTimeMs( qHdr qh, char *outbuf );

/**
 * \brief Returns message time formatted in HH:MM:SS.mmm
 *
 * \param qh - Message header; 0 means current time
 * \param outbuf - Output buffer to hold formatted message time
 * \return Message time formatted in HH:MM:SS.mmm
 */
char  *Quodd_pTimeMs( qHdr qh, char *outbuf );

/**
 * \brief Calculate number of seconds to a point in the future
 *
 * \param hr - Hour
 * \param min - Minute
 * \param sec - Second
 * \return Number of seconds to a point in the future
 */
int    Quodd_Time2Mark( int hr, int min, int sec );

/**
 * \brief Sleep for a period
 *
 * \param tSlp - Sleep time in seconds / microseconds
 */
void   Quodd_Sleep( double tSlp );

/**
 * \brief Hex dump a message into an output buffer
 *
 * \param msg - Message to dump
 * \param len - Message length
 * \param outbuf - Output buffer to hold hex dump
 * \return Length of hex dump in outbuf
 */
int    Quodd_hexMsg( char *msg, int len, char *outbuf );

/**
 * \brief Dump library debug stats to stdout 
 *
 * \param pFile - File containing debug stats
 */
void   Quodd_DumpDebugStats( const char *pFile );

/**
 * \brief No-Op
 */
void   Quodd_breakpoint();



#ifdef __cplusplus
}
#include <hpp/qMutex.hpp>
#include <hpp/BlobList.hpp>
#include <hpp/BlobTable.hpp>
#include <hpp/BONDQuote.hpp>
#include <hpp/BONDTrade.hpp>
#include <hpp/EQBbo.hpp>
#include <hpp/EQBboMM.hpp>
#include <hpp/EQQuote.hpp>
#include <hpp/EQQuoteMM.hpp>
#include <hpp/EQLimitUpDn.hpp>
#include <hpp/EQTrade.hpp>
#include <hpp/Image.hpp>
#include <hpp/Heartbeat.hpp>
#include <hpp/FUNDnav.hpp>
#include <hpp/FUTRMisc.hpp>
#include <hpp/FUTRQuote.hpp>
#include <hpp/FUTRTrade.hpp>
#include <hpp/IDXSummary.hpp>
#include <hpp/IDXValue.hpp>
#include <hpp/OPBbo.hpp>
#include <hpp/OPQuote.hpp>
#include <hpp/OPTrade.hpp>
#include <hpp/Status.hpp>
#include <hpp/QoS.hpp>
#include <hpp/Channel.hpp>
#endif /* __cplusplus */

#endif /* __LIB_QUODD_H */
