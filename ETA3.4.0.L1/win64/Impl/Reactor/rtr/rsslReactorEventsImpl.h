/*
 * This source code is provided under the Apache 2.0 license and is provided
 * AS IS with no warranty or guarantee of fit for purpose.  See the project's 
 * LICENSE.md for details. 
 * Copyright (C) 2019 Refinitiv. All rights reserved.
*/

#ifndef _RTR_RSSL_EVENTS_INT_H
#define _RTR_RSSL_EVENTS_INT_H

#include "rtr/rsslReactorEvents.h"
#include "rtr/rsslQueue.h"
#include "rtr/rsslReactorTokenMgntImpl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	RSSL_RCIMPL_ET_TUNNEL_STREAM_LISTENER = 2,		/* TunnelStream Listener event */
	RSSL_RCIMPL_ET_CHANNEL = 1,		/* Channel event */
	RSSL_RCIMPL_ET_INIT = 0,		/* Unknown event. */
	RSSL_RCIMPL_ET_REACTOR = -1,	/* Reactor-related event */
	RSSL_RCIMPL_ET_FLUSH = -2,		/* Flushing needs to start or has finished */
	RSSL_RCIMPL_ET_TIMER = -3,		/* A timer event. */
	RSSL_RCIMPL_ET_TOKEN_MGNT = -4,	/* Token management event on Login stream */
	RSSL_RCIMPL_ET_CREDENTIAL_RENEWAL = -5, /* OAuth credential renewal event */
	RSSL_RCIMPL_ET_PING = -6, /* Ping event for channel statistics */
	RSSL_RCIMPL_ET_TOKEN_SESSION_MGNT = -7	/* For handling token session */
} RsslReactorEventImplType;

typedef struct
{
	RsslQueueLink eventQueueLink;
	RsslReactorEventImplType eventType;
} RsslReactorEventImplBase;

typedef enum
{
	RSSL_RCIMPL_CET_NEW_CHANNEL = -1,
	RSSL_RCIMPL_CET_CLOSE_CHANNEL = -2,
	RSSL_RCIMPL_CET_CLOSE_CHANNEL_ACK = -3,
	RSSL_RCIMPL_CET_DISPATCH_WL = -4,
	RSSL_RCIMPL_CET_DISPATCH_TUNNEL_STREAM = -5
} RsslReactorChannelEventImplType;

typedef enum
{
	RSSL_RCIMPL_FET_INIT = 0,
	RSSL_RCIMPL_FET_START_FLUSH = -1,
	RSSL_RCIMPL_FET_FLUSH_DONE = -2
} RsslReactorFlushEventType;

typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorFlushEventType flushEventType;
	RsslReactorChannel *pReactorChannel;
} RsslReactorFlushEvent;

RTR_C_INLINE void rsslInitFlushEvent(RsslReactorFlushEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorFlushEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_FLUSH;
}

/* Time that indicates that the reactor's timer is not currently in use.  */
static const RsslInt64 RCIMPL_TIMER_UNSET = 0x7fffffffffffffffLL;

typedef struct
{
	RsslReactorEventImplBase	base;
	RsslInt64					expireTime;
	RsslReactorChannel			*pReactorChannel;
} RsslReactorTimerEvent;

RTR_C_INLINE void rsslInitTimerEvent(RsslReactorTimerEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorTimerEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_TIMER;
}


typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorChannelEvent channelEvent;
	RsslBool isConnectFailure; /* Indicated by worker for channel-down events. Indicates whether the failure occurred while
								* attempting to connect/initialize the channel. */
} RsslReactorChannelEventImpl;

RTR_C_INLINE void rsslClearReactorChannelEventImpl(RsslReactorChannelEventImpl *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorChannelEventImpl));
	pEvent->base.eventType = RSSL_RCIMPL_ET_CHANNEL;
}

typedef enum
{
	RSSL_RCIMPL_STET_INIT = 0,
	RSSL_RCIMPL_STET_SHUTDOWN = -1,
	RSSL_RCIMPL_STET_DESTROY = -2
} RsslReactorStateEventType;

typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorStateEventType reactorEventType;
	RsslErrorInfo *pErrorInfo;
} RsslReactorStateEvent;

RTR_C_INLINE void rsslClearReactorEvent(RsslReactorStateEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorStateEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_REACTOR;
}

typedef enum
{
	RSSL_RCIMPL_TKET_INIT = 0,
	RSSL_RCIMPL_TKET_REISSUE = -1,
	RSSL_RCIMPL_TKET_REISSUE_NO_REFRESH = -2,
	RSSL_RCIMPL_TKET_RESP_FAILURE = -3,
	RSSL_RCIMPL_TKET_SUBMIT_LOGIN_MSG = -4,
	RSSL_RCIMPL_TKET_CHANNEL_WARNING = -5,
	RSSL_RCIMPL_TKET_RENEW_TOKEN = -6
} RsslReactorTokenMgntEventType;

typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorTokenMgntEventType reactorTokenMgntEventType;
	RsslReactorChannel *pReactorChannel;
	RsslReactorOAuthCredentialRenewal *pOAuthCredentialRenewal;
	RsslReactorAuthTokenEvent reactorAuthTokenEvent;
	RsslReactorAuthTokenEventCallback *pAuthTokenEventCallback;
	RsslReactorTokenSessionImpl *pTokenSessionImpl;
	RsslReactorErrorInfoImpl *pReactorErrorInfoImpl;
} RsslReactorTokenMgntEvent;

RTR_C_INLINE void rsslClearReactorTokenMgntEvent(RsslReactorTokenMgntEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorTokenMgntEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_TOKEN_MGNT; 
}

typedef enum
{
	RSSL_RCIMPL_CRET_MEM_ALLOC_FAILED = -1,
	RSSL_RCIMPL_CRET_INIT = 0,
	RSSL_RCIMPL_CRET_AUTH_REQ_WITH_PASSWORD = 0x01,
	RSSL_RCIMPL_CRET_AUTH_REQ_WITH_PASSWORD_CHANGE = 0x02,
	RSSL_RCIMPL_CRET_RENEWAL_CALLBACK = 0x04,
	RSSL_RCIMPL_CRET_MEMORY_DEALLOCATION = 0x08
} RsslReactorOAuthCredentialRenewalEventType;

typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorOAuthCredentialRenewal *pOAuthCredentialRenewal;
	RsslReactorOAuthCredentialRenewalEventType	reactorCredentialRenewalEventType;
	RsslReactorOAuthCredentialEvent reactorOAuthCredentialEvent;
	RsslReactorOAuthCredentialEventCallback *pOAuthCredentialEventCallback;
	RsslReactorTokenSessionImpl *pTokenSessionImpl;
	RsslReactorErrorInfoImpl *pReactorErrorInfoImpl;
} RsslReactorCredentialRenewalEvent;

RTR_C_INLINE void rsslClearReactorCredentialRenewalEvent(RsslReactorCredentialRenewalEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorCredentialRenewalEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_CREDENTIAL_RENEWAL;
}

/* This is used to notify ping sent from the worker thread to the dispatching thread */
typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorChannel *pReactorChannel;
} RsslReactorChannelPingEvent;

RTR_C_INLINE void rsslClearReactorChannelPingEvent(RsslReactorChannelPingEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorChannelPingEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_PING;
}

typedef enum
{
	RSSL_RCIMPL_TSET_INIT = 0,
	RSSL_RCIMPL_TSET_ADD_TOKEN_SESSION_TO_LIST = 0x01,
	RSSL_RCIMPL_TSET_REGISTER_CHANNEL_TO_SESSION = 0x02,
	RSSL_RCIMPL_TSET_UNREGISTER_CHANNEL_FROM_SESSION = 0x04,
	RSSL_RCIMPL_TSET_RETURN_CHANNEL_TO_CHANNEL_POOL = 0x08
} RsslReactorTokenSessionEventType;

typedef struct
{
	RsslReactorEventImplBase base;
	RsslReactorTokenSessionEventType reactorTokenSessionEventType;
	RsslReactorChannel *pReactorChannel;
	RsslReactorTokenSessionImpl *pTokenSessionImpl;
} RsslReactorTokenSessionEvent;

RTR_C_INLINE void rsslClearReactorTokenSessionEvent(RsslReactorTokenSessionEvent *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorTokenSessionEvent));
	pEvent->base.eventType = RSSL_RCIMPL_ET_TOKEN_SESSION_MGNT;
}

typedef union 
{
	RsslReactorEventImplBase			base;
	RsslReactorChannelEventImpl			channelEventImpl;
	RsslReactorCredentialRenewalEvent   credentialRenewalEvent;
	RsslReactorFlushEvent				flushEvent;
	RsslReactorChannelPingEvent			pingEvent;
	RsslReactorTokenMgntEvent			tokenMgntEvent;
	RsslReactorTokenSessionEvent		tokenSessionEvent;
	RsslReactorStateEvent				reactorEvent;
	RsslReactorTimerEvent				timerEvent;
} RsslReactorEventImpl;

RTR_C_INLINE void rsslClearReactorEventImpl(RsslReactorEventImpl *pEvent)
{
	memset(pEvent, 0, sizeof(RsslReactorEventImpl));
}

#ifdef __cplusplus
};
#endif

#endif

