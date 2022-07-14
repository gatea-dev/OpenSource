/******************************************************************************
*
*  Channel.hpp
*     QuoddFeed Channel connection to UltraCache
*
*  REVISION HISTORY:
*      3 SEP 2014 jcs  Created.
*     15 DEC 2014 jcs  Build 11: RcvBuf() / OpenRate(); EQLimitUpDn_
*      3 MAR 2015 jcs  Build 13: Quodd_GetFuturesOptionChain(); Compression
*     12 MAR 2015 jcs  Build 14: qMsg_FUNDnav; GetMutualFunds()
*     11 APR 2015 jcs  Build 15: qMsg_FUTRMisc; LoadLibrary()
*     13 APR 2015 jcs  Build 16: ID(); BulkSubscribe()
*      1 MAY 2015 jcs  Build 17: QoS; GetChannel()
*     29 JUN 2015 jcs  Build 19: GetSocket()
*     10 FEB 2016 jcs  Build 21: BONDQuote / BONDTrade
*     18 JAN 2018 jcs  Build 23: Heartbeat
*     17 DEC 2019 jcs  Build 24: VOID_PTR
*
*  (c) 1994-2019 Gatea Ltd.
******************************************************************************/
#ifndef __QUODD_CHANNEL_H
#define __QUODD_CHANNEL_H
#include <string.h>

#define _MAX_CHAN 100 // Max 100 connections per instance

namespace QUODD
{

class Channel;  // Forward declaration for bInit_ / chans_ 

static bool     bInit_ = false;
static Channel *chans_[_MAX_CHAN];


////////////////////////////////////////////////
//
//      c l a s s   C h a n n e l
//
////////////////////////////////////////////////

/**
 * \class Channel
 * \brief This class manages your connection to UltraCache.
 *
 * You typically derive a class from Channel and override the 
 * virtual methods such as OnConnect() and OnUpdate( EQTrade & )
 * to process incoming events and messages.
 */
class Channel
{
	////////////////////////////////////
	// Constructor / Destructor
	////////////////////////////////////
public:
	/**
	 * \brief Constructor.  Call Start() to connect to UltraCache.
	 *
	 * The constructor initializes internal variables, including reusable 
	 * message objects passed to the OnUpdate() family of calls.  You
	 * call Start() to connect to the UltraCache server.
	 */
	Channel() :
		_hosts(),
		_user(),
		_pword(),
		_dtTm(),
		_tm(),
		_mcAddr(),
		_mcRcvBuf( 0 ),
		_mcQoS( true ),
		cxt_( (Quodd_Context)0 ),
		_bCompress( false ),
		msg_(),
		BlobList_(),
		BlobTable_(),
		BONDQuote_(),
		BONDTrade_(),
		EQBbo_(),
		EQBboMM_(),
		EQQuote_(),
		EQQuoteMM_(),
		EQTrade_(),
		EQLimitUpDn_(),
		Heartbeat_(),
		Image_(),
		OPBbo_(),
		OPQuote_(),
		OPTrade_(),
		FUNDnav_(),
		FUTRQuote_(),
		FUTRTrade_(),
		FUTRMisc_(),
		IDXValue_(),
		IDXSummary_(),
		Status_(),
		QoS_()
	{
		int i;

		// Initialize channel lookup (ONCE)

		for ( i=0; !bInit_ && i<_MAX_CHAN; chans_[i++]=(Channel *)0 );
		bInit_ = true;

		// Initialize us

		::memset( &attr_, 0, sizeof( attr_ ) );
	}

	/**
	 * \brief Descructor.  Calls Stop() to disconnect from UltraCache.
	 * 
	 * The descructor calls Stop() to disconnect from the UltraCache server
	 * and cleans up internal resources. 
	 */
	virtual ~Channel()
	{
		Stop();
	}


	////////////////////////////////////
	// Channel Operations
	////////////////////////////////////
public:
	/**
	 * \brief Enables multicast reception for this channel.
	 *
	 * This must be called before callin Start()
	 *
	 * \param mcAddr - Multicast address formatted as
	 *  <ChannelName>:<ipAddr>:<port>:<interface>
	 * \param rxBufSiz - Receive Buffer Size (SO_RCVBUF)
	 * \param bQoS - true to dispatch QoS messages; false to disable
	 */
	void SetMulticast( const char *mcAddr, int rxBufSiz, bool bQoS=true )
	{
		_mcAddr   = mcAddr;
		_mcRcvBuf = rxBufSiz;
		_mcQoS    = bQoS;
	}

	/**
	 * \brief Initialize the connection to UltraCache.
	 *
	 * Your application is notified via OnConnect() and OnSession() when 
	 * you have successfully connnected and established a session.
	 *
	 * \param host - Comma-separated list of UlraCache <host>:<port> to connect
	 * \param user - UltraCache username supplied by your Quodd sales rep
	 * \param pword - UltraCache password supplied by your Quodd sales rep
	 * \return Textual description of the connection state
	 */
	const char *Start( const char *hosts, const char *user, const char *pword )
	{
		const char *mc;
		int         iv;

		// Pre-condition(s)

		if ( IsValid() )
			return "Already connected";
		if ( !hosts )
			return "No hostname specified";
		if ( !user )
			return "No username specified";
		if ( !pword )
			return "No password specified";

		// Initialize our channel

		_hosts = hosts; 
		_user  = user; 
		_pword = pword; 
		::memset( &attr_, 0, sizeof( attr_ ) );
		attr_._pSvrHosts = _hosts.c_str();
		attr_._pUsername = _user.c_str();
		attr_._pPassword = _pword.c_str();
		attr_._connCbk   = _connCbk;
		attr_._sessCbk   = _sessCbk;
		attr_._msgCbk    = _msgCbk;
		cxt_ = ::Quodd_Initialize( attr_ );
		chans_[cxt_] = this;
		iv = _bCompress ? 1 : 0;
		::Quodd_ioctl( cxt_, qIoctl_setCompression, &iv );
		if ( strlen( (mc=_mcAddr.data()) ) ) {
			::Quodd_ioctl( cxt_, qIoctl_setMulticast, (VOID_PTR)mc );
			if ( _mcRcvBuf )
				SetRcvBufSize( _mcRcvBuf );
			if ( !_mcQoS )
				::Quodd_ioctl( cxt_, qIoctl_dispatchQoS, (VOID_PTR)0 );
		}
		return ::Quodd_Start( cxt_ );
	}

	/**
	 * \brief Destroy connection to the UltraCache
	 * 
	 * Calls ::Quodd_Destroy() to disconnect from the UltraCache server.
	 */
	void Stop()
	{
		if ( cxt_ ) {
			chans_[cxt_] = (Channel *)0;
			::Quodd_Destroy( cxt_ );
		}
		cxt_ = (Quodd_Context)0;
	}

	/**
	 * \brief Enable / Disable reception of qMsg_Image
	 *
	 * \param bEnable - true to receive qMsg_Image
	 */
	void SetImageDispatch( bool bEnable )
	{
		int iv  = bEnable ? 1 : 0;

		::Quodd_ioctl( cxt_, qIoctl_dispatchImage, (VOID_PTR)iv );
	}

	/**
	 * \brief Enable / Disable reception of qMsg_Status
	 *
	 * \param bEnable - true to receive qMsg_Status
	 */
	void SetStatusDispatch( bool bEnable )
	{
		int iv  = bEnable ? 1 : 0;

		::Quodd_ioctl( cxt_, qIoctl_dispatchStatus, (VOID_PTR)iv );
	}

	/**
	 * \brief Set max items to open each second from UltraCache
	 *
	 * \param rate - Max items to open / second
	 * \return  GetOpenRate()
	 */
	int SetOpenRate( int rate )
	{
		return ::Quodd_SetOpenRate( cxt_, rate );
	}

	/**
	 * \brief Get max items to open each second from UltraCache
	 *
	 * \return Max items / sec configured value
	 */
	int GetOpenRate()
	{
		return ::Quodd_GetOpenRate( cxt_ );
	}

	/**
	 * \brief Set SO_RCVBUF for this QuoddFeed channel
	 *
	 * \param bufSiz - SO_RCVBUF size
	 * \return  GetRcvBufSize()
	 */
	int SetRcvBufSize( int bufSiz )
	{
		return ::Quodd_SetRcvBufSize( cxt_, bufSiz );
	}

	/**
	 * \brief Get SO_RCVBUF for this QuoddFeed channel
	 *
	 * \return SO_RCVBUF size
	 */
	int GetRcvBufSize()
	{
		return ::Quodd_GetRcvBufSize( cxt_ );
	}

	/**
	 * \brief Tie this channel thread to a specific CPU
	 *
	 * \param cpu - CPU core to attach this channel thread to
	 * \return  GetThreadProcessor()
	 */
	int SetThreadProcessor( int cpu )
	{
		::Quodd_ioctl( cxt_, qIoctl_setThreadProcessor, &cpu );
		return GetThreadProcessor();
	}

	/**
	 * \brief Get the CPU this channel is tied to
	 *
	 * \return  CPU this channel is tied to
	 */
	int GetThreadProcessor()
	{
		int cpu;

		cpu = 0;
		::Quodd_ioctl( cxt_, qIoctl_getThreadProcessor, &cpu );
		return cpu;
	}

	/**
	 * \brief Enable / disable compression on this channel.
	 *
	 * You must set compression before calling Start().
	 *
	 * \param bCompress - true to compress
	 */
	void SetCompression( bool bCompress )
	{
		_bCompress = bCompress;
	}

	/**
	 * \brief Return true if channel is compressed
	 *
	 * \return true if channel is compressed
	 */
	bool IsCompressed()
	{
		int iVal;

		iVal = -1;
		if ( IsValid() )
			::Quodd_ioctl( cxt_, qIoctl_getCompression, &iVal );
		return( iVal == 1 );
	}

	/**
	 * \brief Return unique channel ID
	 *
	 * \return  Unique channel ID
	 */
	int ID()
	{
		return cxt_;
	}

	/**
	 * \brief Return true is we are Start()'ed
	 *
	 * \return  true if we have Start()'ed but not Stop()'ed.
	 */
	bool IsValid()
	{
		return( cxt_ != (Quodd_Context)0 );
	}

	/**
	 * \brief Return socket file descriptor for this Channel
	 *
	 * \return Socket file descriptor for this Channel
	 */
	int GetSocket()
	{
		int fd;

		fd = 0;
		if ( IsValid() )
			::Quodd_ioctl( cxt_, qIoctl_getFd, &fd );
		return fd;
   }


	////////////////////////////////////
	// Date / Time
	////////////////////////////////////
	/**
	 * \brief Returns current time as YYYY-MM-DD HH:MM:SS.mmm
	 *
	 * \return current time as YYYY-MM-DD HH:MM:SS.mmm
	 */
	const char *pDateTimeMs()
	{
		qHdr qh;
		char buf[K];

		::memset( &qh, 0, sizeof( qh ) );
		_dtTm = ::Quodd_pDateTimeMs( qh, buf );
		return _dtTm.c_str();
	}

	/**
	 * \brief Returns current time as HH:MM:SS.mmm
	 *
	 * \return current time as HH:MM:SS.mmm
	 */
	const char *pTimeMs()
	{
		qHdr qh;
		char buf[K];

		::memset( &qh, 0, sizeof( qh ) );
		_tm = ::Quodd_pTimeMs( qh, buf );
		return _tm.c_str();
	}


	////////////////////////////////////
	// Subscribe / Unsubscribe
	////////////////////////////////////
public:
	/**
	 * \brief Open a subscription stream for tkr
	 *
	 * Calls ::Quodd_Subscribe() to open a subscription stream for this 
	 * ticker.  Market data updates are returned in the OnUpdate() family
	 * of API calls.
	 *
	 * \see ::Quodd_Subscribe()
	 *
	 * \param tkr - Ticker name (e.g. CSCO)
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int Subscribe( const char *tkr, void *arg )
	{
		return ::Quodd_Subscribe( cxt_, tkr, arg );
	}

	/**
	 * \brief Closes subscription stream for tkr
	 *
	 * Calls ::Quodd_Unsubscribe() to close a subscription stream for this 
	 * ticker.  Market data updates are stopped
	 *
	 * \see ::Quodd_Unsubscribe()
	 *
	 * \param tkr - Ticker name (e.g. CSCO) passed on Subscribe()
	 */
	void Unsubscribe( const char *tkr )
	{ 
		::Quodd_Unsubscribe( cxt_, tkr );
	} 


	////////////////////////////////////
	// Query
	////////////////////////////////////
public:
	/**
	 * \brief Query for list of NBBO options for a given underlyer
	 *
	 * Calls ::Quodd_GetExchList() to query for list of all options 
	 * for a given underlyer.  Asynchronous response is returned in 
	 * OnUpdate( BlobTable & ).
	 *
	 * \param exch - Exchange name (e.g., BBO, ARCX, etc.)
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetExchList( const char *exch, void *arg )
	{
		return ::Quodd_GetExchList( cxt_, exch, arg );
	}

	/**
	 * \brief Query for list of of all indices
	 *
	 * Calls ::Quodd_GetIndices() to query for list of all indices
	 * in UltraCache.  Asynchronous response is returned in 
	 * OnUpdate( BlobTable & ).
	 *
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetIndices( void *arg )
	{
		return ::Quodd_GetIndices( cxt_, arg );
	}

	/**
	 * \brief Query for list of of all mutual funds
	 *
	 * Calls ::Quodd_GetIndices() to query for list of all mutual 
	 * funds in UltraCache.  Asynchronous response is returned in 
	 * OnUpdate( BlobTable & ).
	 *
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetMutualFunds( void *arg )
	{
		return ::Quodd_GetMutualFunds( cxt_, arg );
	}

	/**
	 * \brief Query for list of all options underlyers
	 *
	 * Calls ::Quodd_GetOptionUnderlyers() to query for table of all 
	 * options underlyers.  Asynchronous response is returned in 
	 * OnUpdate( BlobTable & ).
	 *
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetOptionUnderlyers( void *arg )
	{
		return ::Quodd_GetOptionUnderlyers( cxt_, arg );
	}

	/**
	 * \brief Query for list of NBBO options for a given underlyer
	 *
	 * Calls ::Quodd_GetOptionChain() to query for list of all options 
	 * for a given underlyer.  Asynchronous response is returned in 
	 * OnUpdate( BlobList & ).
	 *
	 * \param und - Ticker name (e.g. CSCO)
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetOptionChain( const char *und, void *arg )
	{
		return ::Quodd_GetOptionChain( cxt_, und, arg );
	}

	/**
	 * \brief Query for list of futures for a given contract.
	 *
	 * Calls ::Quodd_GetFuturesChain() to query for list of all futures 
	 * for a given contract such as ES or GC.  Asynchronous response is 
	 * returned in OnUpdate( BlobTable & ).
	 *
	 * \param contract - Contract type - ES, GC, etc.
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetFuturesChain( const char *contract, void *arg )
	{
		return ::Quodd_GetFuturesChain( cxt_, contract, arg );
	}

	/**
	 * \brief Query for list of options for a given futures contract
	 *
	 * Calls ::Quodd_GetFuturesOptionChain() to query for list of all 
	 * options for a given futures contract such as /ESH15.  Asynchronous 
	 * response is returned in OnUpdate( BlobTable & ).
	 *
	 * \param contract - Futures Contract - /ESH15, etc.
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetFuturesOptionChain( const char *contract, void *arg )
	{
		return ::Quodd_GetFuturesOptionChain( cxt_, contract, arg );
	}

	/**
	 * \brief Query for list of all tickers on a channel
	 *
	 * Calls ::Quodd_GetChannel() to query for list of all tickers on  
	 * speciid channel such as OPRA1.  Asynchronous response is returned 
	 * in OnUpdate( BlobTable & ).
	 *
	 * \param chan - Channel name (e.g., OPRA1)
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \return Non-zero Unique Stream ID if successful; 0 if not
	 */
	int GetChannel( const char *chan, void *arg )
	{
		return ::Quodd_GetChannel( cxt_, chan, arg );
	}

	/**
	 * \brief Query UltraCache for time/sales/quotes time-series
	 *
	 * The result set is returned to you as a series of asynchronous market
	 * data updates in the OnUpdate() family.
	 *
	 * \param tkr - Ticker name to query
	 * \param startTime - Ticker name to query
	 * \param endTime - Ticker name to query
	 * \param arg - User-supplied argument returned in OnUpdate()
	 * \param filter - Optional filter : TRADE, QUOTE, ALL (default)
	 * \param qryName - Optional query name; Default is TSQ
	 * \return Non-zero Unique Query ID if successful; 0 if not
	 */
	int QueryTSQ( const char *tkr,
	              const char *startTime,
	              const char *endTime,
	              void       *arg,
	              const char *filter  = "ALL",
	              const char *qryName = "TSQ" )
	{
		QuoddTsqReq q;

		q._qry       = qryName;
		q._tkr       = tkr;
		q._startTime = startTime;
		q._endTime   = endTime;
		q._filter    = filter;
		return ::Quodd_QueryTSQ( cxt_, q, arg );
	}


	////////////////////////////////////
	// Asynchronous Callbacks
	////////////////////////////////////
protected:
	/**
	 * \brief Called asynchronously when we connect or disconnect from 
	 * the UltraCache.
	 *
	 * Override this method in your application to take action when 
	 * your connect or disconnect from the UltraCache.
	 *
	 * \param msg - Textual description of connection state
	 * \param bOK - true if connected; false if disconnected
	 */
	virtual void OnConnect( const char *msg, bool bOK )
	{ ; }

	/**
	 * \brief Called asynchronously when we establish or lose the 
	 * session to the UltraCache.
	 *
	 * This method is called after OnConnect() when your session request 
	 * is accepted or rejected by the UltraCache server.  Override this 
	 * method in your application to take appropriate action.  Note that
	 * a session is rejected if your username / password credentials
	 * passed in via Start() are not approved by the server.
	 *
	 * \param msg - Textual description of connection state
	 * \param bOK - true if accepted; false if rejected
	 */
	virtual void OnSession( const char *msg, bool bOK )
	{ ; }

	/**
	 * \brief Called asynchronously when a DEAD status message is received.
	 *
	 * \param msg - Status message
	 */
	virtual void OnUpdate( Status &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Blob List message is received.
	 *
	 * \param msg - BlobList message
	 */
	virtual void OnUpdate( BlobList &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Blob Table message is received.
	 *
	 * \param msg - BlobTable message
	 */
	virtual void OnUpdate( BlobTable &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Initial Image message is received.
	 *
	 * \param msg - Image message
	 */
	virtual void OnImage( Image &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Bond Quote message is received.
	 *
	 * \param msg - BONDQuote message
	 */
	virtual void OnUpdate( BONDQuote &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Bond Trade message is received.
	 *
	 * \param msg - BONDTrade message
	 */
	virtual void OnUpdate( BONDTrade &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Equity BBO message is received.
	 *
	 * \param msg - EQBbo message
	 */
	virtual void OnUpdate( EQBbo &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Equity BBO Market Maker message 
	 * is received.
	 *
	 * \param msg - EQBboMM message
	 */
	virtual void OnUpdate( EQBboMM &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Equity Quote message is received.
	 *
	 * \param msg - EQQuote message
	 */
	virtual void OnUpdate( EQQuote &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Equity Quote Market Maker 
	 * message is received.
	 *
	 * \param msg - EQQuoteMM message
	 */
	virtual void OnUpdate( EQQuoteMM &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Equity Trade message is received.
	 *
	 * \param msg - EQTrade message
	 */
	virtual void OnUpdate( EQTrade &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Equity Limit Up / Down message 
	 * is received.
	 *
	 * \param msg - EQLimitUpDn message
	 */
	virtual void OnUpdate( EQLimitUpDn &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Options BBO message is received.
	 *
	 * \param msg - OPBbo message
	 */
	virtual void OnUpdate( OPBbo &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Options Quote message is received.
	 *
	 * \param msg - OPQuote message
	 */
	virtual void OnUpdate( OPQuote &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Options Trade message is received.
	 *
	 * \param msg - OPTrade message
	 */
	virtual void OnUpdate( OPTrade &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Futures Quote message is received.
	 *
	 * \param msg - FUTRQuote message
	 */
	virtual void OnUpdate( FUTRQuote &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Futures Trade message is received.
	 *
	 * \param msg - FUTRTrade message
	 */
	virtual void OnUpdate( FUTRTrade &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Futures Misc message is received.
	 *
	 * \param msg - FUTRMisc message
	 */
	virtual void OnUpdate( FUTRMisc &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Index Value message is received.
	 *
	 * \param msg - IDXValue message
	 */
	virtual void OnUpdate( IDXValue &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Index Summary message is received.
	 *
	 * \param msg - IDXSummary message
	 */
	virtual void OnUpdate( IDXSummary &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Mutual Fund message is received.
	 *
	 * \param msg - FUNDnav message
	 */
	virtual void OnUpdate( FUNDnav &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Quality of Service message is
	 * received.
	 *
	 * \param msg - QoS message
	 */
	virtual void OnUpdate( QoS &msg )
	{ ; }

	/**
	 * \brief Called asynchronously when a Heartbeat message is
	 * received.
	 *
	 * \param msg - Heartbeat message
	 */
	virtual void OnUpdate( Heartbeat &msg )
	{ ; }


	/**
	 * \brief Called asynchronously when an unknown message is received
	 *
	 * An unknown message might be a new UltraFeed message type or an 
	 * existing message type that has not yet been coded into the 
	 * library.
	 * \param msg - Unknown Message
	 */
	virtual void OnUnknown( Message &msg )
	{ ; }

	////////////////////////////////////
	// Asynchronous Callback (private)
	////////////////////////////////////
private:
	void _OnMessage( QuoddMsg *qm )
	{
		msg_.Set( this, qm );
		switch( msg_.MT() ) {
			case qMsg_Status:
				Status_.Set( this, qm );
				OnUpdate( Status_ );
				break;
			case qMsg_BlobList:
				BlobList_.Set( this, qm );
				OnUpdate( BlobList_ );
				break;
			case qMsg_BlobTable:
				BlobTable_.Set( this, qm );
				OnUpdate( BlobTable_ );
				break;
			case qMsg_Image:
				Image_.Set( this, qm );
				OnImage( Image_ );
				break;
			case qMsg_BONDQuote:
				BONDQuote_.Set( this, qm );
				OnUpdate( BONDQuote_ );
				break;
			case qMsg_BONDTrade:
				BONDTrade_.Set( this, qm );
				OnUpdate( BONDTrade_ );
				break;
			case qMsg_EQBbo:
				EQBbo_.Set( this, qm );
				OnUpdate( EQBbo_ );
				break;
			case qMsg_EQBboMM:
				EQBboMM_.Set( this, qm );
				OnUpdate( EQBboMM_ );
				break;
			case qMsg_EQQuote:
				EQQuote_.Set( this, qm );
				OnUpdate( EQQuote_ );
				break;
			case qMsg_EQQuoteMM:
				EQQuoteMM_.Set( this, qm );
				OnUpdate( EQQuoteMM_ );
				break;
			case qMsg_EQLimitUpDn:
				EQLimitUpDn_.Set( this, qm );
				OnUpdate( EQLimitUpDn_ );
				break;
			case qMsg_EQTrade:
				EQTrade_.Set( this, qm );
				OnUpdate( EQTrade_ );
				break;
			case qMsg_OPBbo:
				OPBbo_.Set( this, qm );
				OnUpdate( OPBbo_ );
				break;
			case qMsg_OPQuote:
				OPQuote_.Set( this, qm );
				OnUpdate( OPQuote_ );
				break;
			case qMsg_OPTrade:
				OPTrade_.Set( this, qm );
				OnUpdate( OPTrade_ );
				break;
			case qMsg_FUTRQuote:
				FUTRQuote_.Set( this, qm );
				OnUpdate( FUTRQuote_ );
				break;
			case qMsg_FUTRTrade:
				FUTRTrade_.Set( this, qm );
				OnUpdate( FUTRTrade_ );
				break;
			case qMsg_IDXValue:
				IDXValue_.Set( this, qm );
				OnUpdate( IDXValue_ );
				break;
			case qMsg_IDXSummary:
				IDXSummary_.Set( this, qm );
				OnUpdate( IDXSummary_ );
				break;
			case qMsg_FUNDnav:
				FUNDnav_.Set( this, qm );
				OnUpdate( FUNDnav_ );
				break;
			case qMsg_FUTRMisc:
				FUTRMisc_.Set( this, qm );
				OnUpdate( FUTRMisc_ );
				break;
			case qMsg_Heartbeat:
				Heartbeat_.Set( this, qm );
				OnUpdate( Heartbeat_ );
				break;
			case qMsg_QoS:
				QoS_.Set( this, qm );
				OnUpdate( QoS_ );
				break;
			default:
				OnUnknown( msg_ );
				break;
		}
	}


	////////////////////////
	// Private Members
	////////////////////////
private:
	std::string   _hosts;
	std::string   _user;
	std::string   _pword;
	std::string   _dtTm;
	std::string   _tm;
	std::string   _mcAddr;
	int           _mcRcvBuf;
	bool          _mcQoS;
	QuoddAttr     attr_;
	Quodd_Context cxt_;
	bool          _bCompress;
	Message       msg_;
	BlobList      BlobList_;
	BlobTable     BlobTable_;
	BONDQuote     BONDQuote_;
	BONDTrade     BONDTrade_;
	EQBbo         EQBbo_;
	EQBboMM       EQBboMM_;
	EQQuote       EQQuote_;
	EQQuoteMM     EQQuoteMM_;
	EQTrade       EQTrade_;
	EQLimitUpDn   EQLimitUpDn_;
	Heartbeat     Heartbeat_;
	Image         Image_;
	OPBbo         OPBbo_;
	OPQuote       OPQuote_;
	OPTrade       OPTrade_;
	FUNDnav       FUNDnav_;
	FUTRQuote     FUTRQuote_;
	FUTRTrade     FUTRTrade_;
	FUTRMisc      FUTRMisc_;
	IDXValue      IDXValue_;
	IDXSummary    IDXSummary_;
	Status        Status_;
	QoS           QoS_;

	////////////////////////////////////
	// Class-wide public methods
	////////////////////////////////////
public:
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
	 * The channels must have already been initialized before calling
	 * BulkSubscribe().  Market data arrives asynchronously
	 * in the channel threads just as it does for Subscribe().
	 *
	 * \param filename - Filename
	 * \param sep - Token separator (comma, pipe, space, etc.)
	 * \return Number of streams opened across all channels
	 */
	int BulkSubscribe( char *filename, const char *sep )
	{
		return ::Quodd_BulkSubscribe( filename, sep );
	}

	/**
	 * \brief Load the library
	 *
	 * If not explicitly called by your application, this is called 
	 * on the first call to Start().
	 *
	 * \return true if loaded; false if already loaded
	 */
	static bool LoadLibrary()
	{
		return ::Quodd_LoadLibrary() ? true : false;
	}

	/**
	 * \brief Unload the library
	 *
	 * On Windows, this calls WSACleanup() if and only if no 
	 * channels are opened (i.e., Every Start() has been Stop()'ed.
	 *
	 * \return true if unloaded; false if one or more channels 
	 * still open.
	 */
	static bool UnloadLibrary()
	{
		return ::Quodd_UnloadLibrary() ? true : false;
	}

	/** \brief Load multicast tickers from 3-column file.
	 *
	 * Flat ASCII file containing rows with 3 comma-separated columns in the
	 * following order:
	 * + Ticker Name (e.g., CSCO or /ESM15)
	 * + Channel Name (EQUITY or OPRA1 OR FUTURE)
	 * + d/b Index
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
	int LoadMulticastTickers( const char *pFile )
	{
		return ::Quodd_LoadMulticastTickers( pFile );
	}

	/**
	 * \brief Dump library debug stats to stdout
	 *
	 * You enable the library to keep run-time stats in a memory-
	 * mapped file by setting the QUODD_DEBUG_STATS environment 
	 * variable to the filename of the debug stats file.  If not 
	 * set, the run-time stats file is not created.
	 *
	 * \param pFile - File containing debug stats
	 */
	static void DumpDebugStats( const char *pFile )
	{
		::Quodd_DumpDebugStats( pFile );
	}

	/**
	 * \brief Returns the library build version
	 *
	 * \see ::Quodd_Version()
	 * \return Build version of the library
	 */
	static const char *Version()
	{
		return ::Quodd_Version();
	}

	/**
	 * \brief Sleep for period of time
	 *
	 * \param dSlp - Sleep interval in seconds
	 */
	static void Sleep( double dSlp )
	{
		::Quodd_Sleep( dSlp );
	}

	/**
	 * \brief Sets the library debug level; Initiates logging
	 *
	 * \see ::Quodd_Log()
	 * \param pLog - Log filename
	 * \param dbgLvl - Debug level
	 */
	static void Log( const char *pLog, int dbgLvl )
	{
		::Quodd_Log( pLog, dbgLvl );
	}


	////////////////////////////////////
	// Class-wide (private) callbacks
	////////////////////////////////////
private:
	static void QODAPI _connCbk( Quodd_Context cxt, 
	                             const char   *msg, 
	                             QuoddState    state )
	{
		Channel *us;

		if ( (us=chans_[cxt]) )
			us->OnConnect( msg, ( state == qod_up ) );
	}

	static void QODAPI _sessCbk( Quodd_Context cxt, 
	                             const char   *msg,
	                             QuoddState    state )
	{
		Channel *us;

		if ( (us=chans_[cxt]) )
			us->OnSession( msg, ( state == qod_up ) );
	} 

	static void QODAPI _msgCbk( QuoddMsg *qm )
	{
		Channel *us;

		if ( (us=chans_[qm->_cxt]) )
			us->_OnMessage( qm );
	}
};  // class Channel

} // namespace QUODD

#endif // __QUODD_CHANNEL_H 
