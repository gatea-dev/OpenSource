/// @file FEConsumer_C.h
#ifndef __FECONSUMER_C_H__
#define __FECONSUMER_C_H__

#include "fdsfe_Api_C.h"
#include "Error_C.h"
#include "Event_C.h"
#include "Log_Message_C.h"
#include "c/MD_Message.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

  /*! @brief Callback signature for receiving market data messages
  *
  * @param topic The topic of the message
  * @param msg The message being received
  */
  typedef void (*MessageCallback_C) (const char* topic, MD_Message* msg, void* closure);

  /*! @brief Callback signature for receiving events
  *
  * @param event The event being received
  * @param closure The closure that was given when the callback was set or
  * NULL if no closure was given
  *
  * @see FEConsumer_event_cb()
  */
  typedef void (*EventCallback_C) (const Event* event, void* closure);

  /*! @brief Callback signature for receiving logging messages
  *
  * For user with FEConsumer_set_logging_callback() in order to integrate with
  * existing logging frameworks
  *
  * @param message Log message being sent from the toolkit
  */
  typedef void (*LoggingCallback_C) (const LogMessage* message);

  typedef struct FEConsumerHandle FEConsumer;
#ifdef __linux__
  typedef int FEConsumer_SocketType;
#elif _WIN32
  typedef unsigned long long FEConsumer_SocketType; //The SOCKET type
#endif
  extern const int NO_TIMEOUT;
  extern const int HWM_UNLIMITED;

  /*! @brief Set of possible snapshot modes when subscribing */
  typedef enum {
    NO_SNAPSHOT = 0,     // No snapshots will be requested
    REQUEST_SNAPSHOT = 1 // Snapshots will be requested
  } FEConsumer_SnapshotMode;

  /*! @brief Set of possible data modes when subscribing */
  typedef enum {
    LIVE = 0,    // Live realtime market data will be requested
    DELAYED = 1, // Delayed market data will be requested
    CANNED = 2   // Prerecorded canned data will be requested
  } FEConsumer_DataMode;

  /*! @brief Set of possible data sources when registering callbacks */
  typedef enum {
    MARKET = 0,   // Market data from subscribe
    RECOVERY = 1, // Recovery data from recover
    DATASOURCE_COUNT = 2     // Dummy value indicating number of DataSources
  } FEConsumer_DataSource;

  /*! @brief Constructor.
  *
  * Market data messages will be received, processed, and delivered by a pool
  * of subscriber and worker threads.
  *
  * Each subscriber thread will manage its own connection to the broadcast
  * server. Whenever a subscription to a new topic is made, a subscriber is
  * selected to handle that subscription on a round robin basis. Messages for
  * a topic will always be received by the same subscriber thread to preserve
  * proper ordering. Whenever a subscribe receives a message, that message is
  * forwarded to a worker thread. The worker chosen to process the message
  * will be consistent for a single topic.
  *
  * Worker threads process the messages and then forward them to a notify
  * queue registered with register_callback(). These messages can then be
  * dispatched by dispatch() or dispatch_fd().
  *
  * If the application is unable to process the messages fast enough, they
  * will begin to queue up until the system runs out of memory. If this is
  * undesirable, the high watermark can be set to drop messages once a
  * certain amount have been queued.
  *
  * @param sub_count The number of subscriber threads to create.
  * @param worker_count The number of worker threads to create.
  * @param high_watermark The high watermark of the subscribers and workers.
  *
  * @return Pointer to the new FEConsumer
  */
  FDSFE_C_API FEConsumer* FEConsumer_new_hwm(int sub_count, int worker_count, int high_watermark);

  /*! @copybrief FEConsumer_new_hwm(int sub_count, int worker_count, int high_watermark)
  *
  * High watermark will be set to #HWM_UNLIMITED
  *
  * @see FEConsumer_new_hwm()
  */
  FDSFE_C_API FEConsumer* FEConsumer_new(int sub_count, int worker_count);

  /*! @brief Frees a FEConsumer allocated with FEConsumer_new_hwm() or FEConsumer_new()
  *
  * @param c FEConsumer to free
  */
  FDSFE_C_API void FEConsumer_delete(FEConsumer* const c);

  /*! @brief Opens a file with the given filename to log messages from the toolkit
  *
  * @param filename Name of the time to store the logs
  * @param append If true, appends to previous log file instead of creating a fresh one
  * @param error_out Error pointer where the any errors generated will be stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_log_open(const char* const filename, bool apend, Error* error_out);

  /*! @brief Closes the log file previously opened with log_open() */
  FDSFE_C_API void FEConsumer_log_close();

  /*! @brief Turns on logging additional messages */
  FDSFE_C_API void FEConsumer_enable_verbose_logging();

  /*! @brief Turns off logging additional messages */
  FDSFE_C_API void FEConsumer_disable_verbose_logging();

  /*! @brief Connected state of the FEConsumer
  *
  * The FEConsumer is only considered connected if it is connected to the
  * data server on all channels. It is possible for this function to return
  * false while data is still being received if only one channel is
  * disconnected.
  *
  * @param c FEConsumer to check the connected state of
  * @param result_out Bool pointer where the connected state is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_connect() @see FEConsumer_connect_async()
  */
  FDSFE_C_API bool FEConsumer_connected(const FEConsumer* const c, bool* result_out);

  /*! @brief Authenticated state of the FEConsumer
  *
  * @param c FEConsumer to check the authenticated state of
  * @param result_out Bool pointer where the authenticated state is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_connect() @see FEConsumer_connect_async()
  */
  FDSFE_C_API bool FEConsumer_authenticated(const FEConsumer* const c, bool* result_out);

  /*! @brief Login state of the FEConsumer
  *
  * @param c FEConsumer to check the logged in state of
  * @param result_out Bool pointer where the logged in state is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_log_in() @see FEConsumer_log_in_async()
  */
  FDSFE_C_API bool FEConsumer_logged_in(const FEConsumer* const c, bool* result_out);

  /*! @brief Checks the FEConsumer for a subscription to a topic
  *
  * @param c FEConsumer to check for the subscription
  * @param topic The topic to check for the subscribed status
  * @param result_out Bool pointer where the result is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_subscribe() @see FEConsumer_subscribe_async()
  */
  FDSFE_C_API bool FEConsumer_is_subscribed(const FEConsumer* const c, const char* const topic, bool* result_out);

  /*! @brief Gets the seconds to spend on a single command before returning a timeout error
  *
  * @param c FEConsumer to get the timeout from
  * @param result_out uint pointer where the number of seconds will be stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_timeout_sec()
  */
  FDSFE_C_API bool FEConsumer_get_timeout_sec(const FEConsumer* const c, unsigned int* result_out);

  /*! @brief Gets the seconds to wait to receive all snapshot messages before returning a timeout error
  *
  * @param c FEConsumer to get the snapshot timeout from
  * @param result_out uint pointer where the number of seconds will be stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_snapshot_timeout_sec()
  */
  FDSFE_C_API bool FEConsumer_get_snapshot_timeout_sec(const FEConsumer* const c, unsigned int* result_out);

  /*! @brief Gets the seconds to wait to receive a response after sending a heartbeat before
  * considering the connection as timed out
  *
  * @param c FEConsumer to get the heartbeat timeout from
  * @param result_out uint pointer where the number of seconds is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_heartbeat_timeout_sec()
  */
  FDSFE_C_API bool FEConsumer_get_heartbeat_timeout_sec(const FEConsumer* const c, unsigned int* result_out);

  /*! @brief Gets the seconds to wait between sending heartbeat queries to the server
  *
  * @param c FEConsumer to get the heartbeat interval from
  * @param result_out uint pointer where the number of seconds is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_heartbeat_interval_sec()
  */
  FDSFE_C_API bool FEConsumer_get_heartbeat_interval_sec(const FEConsumer* const c, unsigned int* result_out);

  /*! @brief Gets the maximum number of messages to hold for each topic while waiting for snapshots
  *
  * @param c FEConsumer to get max snapshot queue size from
  * @param result_out uint pointer where the max snapshot queue size is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_max_snapshot_queue_size()
  */
  FDSFE_C_API bool FEConsumer_get_max_snapshot_queue_size(const FEConsumer* const c, unsigned int* result_out);

  /*! @brief Gets the notify queue message limit for a FEConsumer
  *
  * @param c FEConsumer to get the notify queue message limit from
  * @param result_out size_t pointer where the notify queue message limit is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_notify_queue_message_limit()
  */
  FDSFE_C_API bool FEConsumer_get_notify_queue_message_limit(const FEConsumer* const c, size_t* result_out);

  /*! @brief Gets the notify queue time limit
  *
  * @param c FEConsumer to get the notify queue time limit from
  * @param result_out int pointer where the time limit in milliseconds is stored
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_notify_queue_time_limit_ms()
  */
  FDSFE_C_API bool FEConsumer_get_notify_queue_time_limit_ms(const FEConsumer* const c, int* result_out);

  /*! @brief Sets the seconds to spend on a single command before returning a timeout error
  *
  * Use #NO_TIMEOUT to indicate infinite timeout
  *
  * @param c FEConsumer to set the timeout for
  * @param seconds The number of seconds to wait
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_set_timeout_sec(FEConsumer* const c, unsigned int seconds);

  /*! @brief Sets the seconds to wait to receive all snapshot messages before returning a timeout error
  *
  * @param c FEConsumer to set the snapshot timeout for
  * @param seconds The number of seconds to wait
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_set_snapshot_timeout_sec(FEConsumer* const c, unsigned int seconds);

  /*! @brief Sets the seconds to wait to receive a response after sending a heartbeat before
  * considering the connection as timed out
  *
  * @param c FEConsumer to set the heartbeat timeout for
  * @param seconds The number of seconds to wait
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_set_heartbeat_timeout_sec(FEConsumer* const c, unsigned int seconds);

  /*! @brief Sets the seconds to wait between sending heartbeats
  *
  * @param c FEConsumer to set the heartbeat interval for
  * @param seconds The number of seconds to wait
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_set_heartbeat_interval_sec(FEConsumer* const c, unsigned int seconds);

  /*! @brief Sets the maximum number of messages to hold for each topic while waiting for snapshots
  *
  * When the maximum queue size is reached, older messages will be dropped in favor of new ones
  *
  * @param c FEConsumer to set the max snapshot queue size for
  * @param size The new max number of messages for the snapshot queue
  *
  * @see FEConsumer_subscribe() @see FEConsumer_subscribe_async()
  */
  FDSFE_C_API bool FEConsumer_set_max_snapshot_queue_size(FEConsumer* const c, unsigned int size);

  /*! @brief Sets the notify queue message limit for a FEConsumer
  *
  * A notify queue that has not been notified will be notified when it
  * contains at least as many messages as the limit
  *
  * @param c FEConsumer to set the notify queue message limit for
  * @param limit The message limit count
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_set_notify_queue_time_limit_ms()
  * @see FEConsumer_register_callback()
  */
  FDSFE_C_API bool FEConsumer_set_notify_queue_message_limit(FEConsumer* const c, size_t limit);

  /*! @brief Sets the notify queue time limit in milliseconds
  *
  * A notify queue that has not been notified will be notified when it
  * contains at least 1 message and the time limit has passed
  *
  * @param c FEConsumer to set the notify queue time limit for
  * @param limit The new time limit in milliseconds
  */
  FDSFE_C_API bool FEConsumer_set_notify_queue_time_limit_ms(FEConsumer* const c, int limit);

  /*! @brief Enables tracking message latency throughout the consumer
  *
  * If enabled, the consumer will measure the latency for each message
  * at various internal checkpoints. Two latencies will be tracked: one
  * based on the MSG_GENESIS of the message (the time FactSet received
  * the message from the exchange, referred to as the "real latency")
  * and another based on the time the consumer received the message from
  * the broadcast server (referred to as the "toolkit latency"). The highest
  * latency observed during the bucket interval will be taken as the
  * latency for that interval.The latencies at each checkpoint will be
  * printed to the log at the end of the interval, in the following format:
  * CheckpointName [Real Latency (Toolkit Latency)]
  *
  * \b Checkpoints
  * | Name | Description |
  * | --- | ------- |
  * | Subscriber | The message was read off the socket from the broadcast server. |
  * | Worker | The message was received by a worker thread. |
  * | Callback Queue | The message was inserted into the matching callback queue. |
  * | Dispatch | The callback for the message was called during dispatching. |
  *
  * In addition to message latency, the consumer will also
  * track the time it takes to call all callbacks during a call to dispatch()
  * or dispatch_fd(). The longest of these durations within a bucket interval
  * will be printed as the callback time.
  *
  * All printed durations will be given in milliseconds.
  *
  * @param c FEConsumer to enable latency tracking for
  * @param bucket_interval_sec The bucket interval in seconds
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_disable_latency_tracking()
  */
  FDSFE_C_API bool FEConsumer_enable_latency_tracking(FEConsumer* const c, unsigned int bucket_interval_sec);

  /*! @brief Disables tracking message latency throughout the consumer
  *
  * @param c FEConsumer to disable latency tracking for
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_enable_latency_tracking()
  */
  FDSFE_C_API bool FEConsumer_disable_latency_tracking(FEConsumer* const c);

  /*! @brief Registers a callback for messages with topics starting with the
  * given prefix.
  *
  * For each registered callback, an internal notify queue will be created.
  * The file descriptor returned in the parameter fd_out will become readable
  * when a notify is triggered on this queue. A notify will be triggered
  * after a certain number of messages have been enqueued or after a certain
  * number of milliseconds have passed. After a notify is triggered, the file
  * descriptor should not be read from directly. Rather, dispatch_fd() should
  * be called to call the given FEConsumer::MessageCallback for each queued
  * message
  *
  * @see FEConsumer_dispatch()
  * @see FEConsumer_set_notify_queue_message_limit()
  * @see FEConsumer_set_notify_queue_time_limit_ms()
  *
  * @param c FEConsumer to register a callback for
  * @param topic_prefix The topic prefix associated with this callback
  * @param fd_out On success, fd_out will be set to the file descriptor of this callback
  * @param error_out Error pointer that will store the Error if an error
  * occurs, Error_NoError otherwise
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_register_callback(FEConsumer* const c, const char* const topic_prefix,
                                       MessageCallback_C cb, void* closure, FEConsumer_SocketType* fd_out,
                                       Error* error_out);

  /*! @brief Unregisters a callback previously registerd with FEConsumer_register_callback()
  *
  * @param c FEConsumer to unregister callback from
  * @param topic_prefix The topic prefix of the callback to unregister
  * @param error_out Error pointer that will store the Error if an error
  * occurs, Error_NoError otherwise
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_unregister_callback(FEConsumer* const c, const char* const topic_prefix, Error* error_out);

  /*! @copybrief FEConsumer_unregister_callback
  *
  * @param c FEConsumer to unregister callback from
  * @param fd The file descriptor of the callback to unregister
  * @param error_out Error pointer that will store the Error if an error
  * occurs, Error_NoError otherwise
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_unregister_callback_fd(FEConsumer* const c, FEConsumer_SocketType fd, Error* error_out);

  /*! @brief Returns the file descriptor for a given topic prefix
  *
  * The file descriptor returned will be the same value that was returned
  * when the callback was initially registered
  * @see FEConsumer_register_callback()
  *
  * @param c FEConsumer to get the notify socket from
  * @param topic_prefix The topic prefix for which to retrieve the file descriptor
  * @param fd_out On success, fd_out will be set to the file descriptor for the callback
  * @param error_out Error pointer that will store the Error if an error
  * occurs, Error_NoError otherwise
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_get_notify_socket(FEConsumer* const c, const char* const topic_prefix,
                                       FEConsumer_SocketType* fd_out, Error* error_out);

  /*! @brief Convenience function for dispatching on all registered callbacks.
  *
  * The file descriptors for each of the FEConsumer's registered callbacks will be polled and
  * dispatch_fd() will be called for each callback that has messages in its
  * queue. If no messages are immediately available when this function is
  * called, it will wait for up to timeout_ms milliseconds for messages to
  * become available before returning. If a timeout occurs, this is not
  * considered an error and Error::NoError will be returned
  *
  * @param c FEConsumer to dispatch on
  * @param timeout_ms Number of milliseconds to wait before timing out
  * @param error_out Error pointer that will store the Error if an error
  * occurs, Error_NoError otherwise
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_dispatch(FEConsumer* const c, long timeout_ms, Error* error_out);

  /*! @breif Dispatches messages for the callback with the given file descriptor
  *
  * For each message in the FEConsumer's notify queue for the callback with the given file
  * descriptor, the associated callback that was registered by
  * register_callback() will be called. This function should only be called
  * on a readable file descriptor
  *
  * @param c FEConsumer to dispatch on
  * @param fd File descriptor to dispatch
  * @param error_out Error pointer that will store the Error if an error
  * occurs, Error_NoError otherwise
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_dispatch_fd(FEConsumer* const c, FEConsumer_SocketType fd,
                                       Error* error_out);

  /*! @brief Sends logging messages to function instead of file/console
  *
  * @param callback LoggingCallback_C to call on log message
  */
  FDSFE_C_API void FEConsumer_set_logging_callback(LoggingCallback_C callback);

  /*! @brief Sets the EventCallback_C used for receiving events
  *
  * If the closure is set, then it will be returned with the callback,
  * otherwise NULL will be returned
  *
  * @param c FEConsumer to set event callback of
  * @param callback The EventCallback_C to call when an event is received
  * @parma closure Pointer to be provided to the callback when it is called
  */
  FDSFE_C_API bool FEConsumer_event_cb(FEConsumer* c, EventCallback_C callback, void* closure);

  /*! @name Synchronous methods
  * @anchor synchronous_methods
  * A synchronous method will block until it completes.
  * All synchronous methods will emit an Error through an error_out parameter if something went wrong or
  * Error_NoError otherwise. If the operation does not complete within
  * FEConsumer_timeout_sec() seconds, a timeout Error will be returned. Synchronous
  * methods cannot be called from FEConsumer callbacks.
  */
  //!@{

  /*! @brief Authenticates with the given credentials
  *
  * A counter file will be created to store the key and counter used for
  * authentication at the given path. This file will be named \<key_id>.data.
  * If a counter file already exists, the key and counter it contains will be
  * used instead of the key and counter parameters. Setting the force_input
  * flag to true will overwrite the counter file values with the given key
  * and counter
  *
  * @param c FEConsumer to authenticate
  * @param host The address of the authentication server in the format host:port
  * @param user The name of the user
  * @param serial The serial associated with user
  * @param key_id The key id of the key being used for authentication
  * @param key The key associated with key_id for user
  * @param counter The current counter for key
  * @param path The location to store the counter file
  * @param force_input If true, key and counter parameters will be used
  * instead of the values in an existing counter file
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_authenticate(FEConsumer* const c, const char* const host, const char* const user,
                                       const char* const serial, const char* const key_id, const char* const key,
                                       const char* const counter, const char* const path, bool force_input,
                                       Error* error_out);

  /*! @copybrief FEConsumer_authenticate()
  *
  * Similar to FEConsumer_authenticate(), except that \p user is passed
  * in the form user-serial
  *
  * @see FEConsumer_authenticate()
  *
  * @param c FEConsumer to authenticate
  * @param host The address of the authentication server in the format host:port
  * @param user The name and serial of the user in the format user-serial
  * @param key_id The key id of the key being used for authentication
  * @param key The key associated with key_id for user
  * @param counter The current counter for key
  * @param path The location to store the counter file
  * @param force_input If true, key and counter parameters will be used
  * instead of the values in an existing counter file
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_authenticate_no_serial(FEConsumer* const c, const char* const host,
                                       const char* const user, const char* const key_id, const char* const key,
                                       const char* const counter, const char* const path, bool force_input,
                                       Error* error_out);

  /*! @brief Connects to the broadcast server
  *
  * The provided FEConsumer must be authenticated before connecting
  * @see FEConsumer_authenticate() @see FEConsumer_authenticate_async()
  *
  * @param c FEConsumer to connect
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_connect(FEConsumer* const c, Error* error_out);

  /*! @brief Disconnects from the broadcast server
  *
  * Closes the connection to the broadcast server by logging out if
  * FEConsumer_get_logged_in() and disconnecting. To reconnect after
  * disconnecting it is necessary to begin again at authentication
  *
  * The provided FEConsumer must be authenticated before disconnecting
  * @see FEConsumer_authenticate() @see FEConsumer_authenticate_async()
  * @see FEConsumer_connect() @see FEConsumer_connect_async()
  *
  * @param c FEConsumer to disconnect
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_disconnect(FEConsumer* const c, Error* error_out);

  /*! @copybrief FEConsumer_log_in_request_file()
  *
  * The provided FEConsumer must be connected before logging in
  * @see FEConsumer_connect() @see FEConsumer_connect_async()
  *
  * This method is the same as calling FEConsumer_log_in_request_file(c, true, error_out)
  * @see FEConsumer_log_in_request_file()
  *
  * @param c FEConsumer to log in
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_log_in(FEConsumer* const c, Error* error_out); //request_files = true

  /*! @brief Logs in to the broadcast server
  *
  * The FEConsumer must be connected before logging in
  * @see FEConsumer_connect() @see FEConsumer_connect_async()
  *
  * @param c FEConsumer to log in
  * @param request_files If true, requests an updated version of
  * field map from the server when logging in
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_log_in_request_file(FEConsumer* const c, bool request_files, Error* error_out);
  
  /*! @brief Subscribes to a topic
  *
  * If snapshot messages were requested, all live data will be queued until
  * all snapshots are received for the requested topic. If
  * FEConsumer_get_max_snapshot_queue_size() live data messages are queued before all
  * snapshots are received, the oldest of the queued messages will be dropped
  * in favor of newer messages. Once all snapshots are received, queued
  * messages which are newer than the snapshots will be delivered via the
  * registered callbacks as usual. Messages older than the snapshots will be
  * dropped.
  *
  * The format for the topic is productCode|issueType|isoCode|symbol.
  * It is possible to subscribe to a prefix of the format, for example a
  * subscription to a productCode|issueType|isoCode combination will
  * subscribe to all symbols that match the combination. It is not possible
  * to subscribe to two topics which overlap.
  *
  * @param c FEConsumer to subscribe with
  * @param topic The topic being subscribed to
  * @param snapshot_mode Determines whether or not snapshots will be requested
  * @param data_mode Determines the type of data to request
  *
  * @return False if there is an issue with parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_unsubscribe()
  * @see FEConsumer_unsubscribe_async()
  * @see FEConsumer_register_callback()
  */
  FDSFE_C_API bool FEConsumer_subscribe(FEConsumer* const c, const char* const topic,
                                       FEConsumer_SnapshotMode snapshot_mode, FEConsumer_DataMode data_mode,
                                       Error* error_out);

  /*! @brief Unsubscribes from a topic
  *
  * @param c FEConsumer to unsubscribe with
  * @param topic The topic being unsubscribed
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  *
  * @see FEConsumer_subscribe() @see FEConsumer_subscribe_async()
  */
  FDSFE_C_API bool FEConsumer_unsubscribe(FEConsumer* const c, const char* const topic, Error* error_out);

  /*! @brief Requests a configuration file with a given name. countents_out is allocated by the toolkit,
  * but will need to be cleaned up by the user.
  *
  * @param c FEConsumer to request with
  * @param filename The name of the file being requested
  * @param contents_out The contents of the requested file or empty string if there was an error
  * @param contents_length_out The length of contents
  *
  * @return False if there is an issue with the parameters and the operation cannot successfully
  * complete, true otherwise
  */
  FDSFE_C_API bool FEConsumer_request_file(FEConsumer* const c, const char* const filename, char** contents_out,
                                       size_t* contents_length_out, Error* error_out);
  FDSFE_C_API bool FEConsumer_recover(FEConsumer* const c, const char* const topic, time_t begin_time, time_t end_time,
                                       FEConsumer_DataMode data_mode, Error* error_out);

  //!@} // End doxygen Synchronous methods group

  /*! @name Asynchronous methods
  * @anchor asynchronous_methods
  * An asynchronous method will return immediately and will send an Event via
  * the FEConsumer_event_cb() when it completes. If the operation does not complete
  * within FEConsumer_timeout_sec() seconds, an Event containing a timeout Error will be
  * generated. Asynchronous methods can be called from FEConsumer callbacks.
  */
  //!@{

  /*! @copydoc FEConsumer_authenticate() */
  FDSFE_C_API bool FEConsumer_authenticate_async(FEConsumer* const c, const char* const host, const char* const user,
                                       const char* const serial, const char* const key_id, const char* const key,
                                       const char* const counter, const char* const path, bool force_input);

  /*! @copydoc FEConsumer_authenticate_no_serial() */
  FDSFE_C_API bool FEConsumer_authenticate_no_serial_async(FEConsumer* const c, const char* const host,
                                       const char* const user, const char* const key_id, const char* const key,
                                       const char* const counter, const char* const path, bool force_input);

  /*! @copydoc FEConsumer_connect() */
  FDSFE_C_API bool FEConsumer_connect_async(FEConsumer* const c);

  /*! @copydoc FEConsumer_disconnect() */
  FDSFE_C_API bool FEConsumer_disconnect_async(FEConsumer* const c);

  /*! @copydoc FEConsumer_log_in() */
  FDSFE_C_API bool FEConsumer_log_in_async(FEConsumer* const c); //request_file = true

  /*! @copydoc FEConsumer_log_in_request_file() */
  FDSFE_C_API bool FEConsumer_log_in_request_file_async(FEConsumer* const c, bool request_file);

  /*! @copydoc FEConsumer_subscribe() */
  FDSFE_C_API bool FEConsumer_subscribe_async(FEConsumer* const c, const char* const topic,
                                       FEConsumer_SnapshotMode snapshot_mode, FEConsumer_DataMode data_mode);

  /*! @copydoc FEConsumer_unsubscribe() */
  FDSFE_C_API bool FEConsumer_unsubscribe_async(FEConsumer* const c, const char* const topic);

  /*! @copydoc FEConsumer_request_file() */
  FDSFE_C_API bool FEConsumer_request_file_async(FEConsumer* const c, const char* const filename);

  FDSFE_C_API bool FEConsumer_recover_async(FEConsumer* const c, const char* const topic, time_t begin_time,
                                       time_t end_time, FEConsumer_DataMode data_mode);

  //!@} // End doxygen Asynchronous methods group

#ifdef __cplusplus
}
#endif

#endif //__FECONSUMER_C_H__
