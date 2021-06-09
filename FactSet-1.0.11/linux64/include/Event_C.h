/// @file Event_C.h
#ifndef __EVENT_C_H__
#define __EVENT_C_H__

#include "fdsfe_Api_C.h"
#include "Error_C.h"

#ifdef __cplusplus
extern "C" {
#endif

  /*! @brief Set of possible event types.
  *
  * Events are most commonly spawned to indicate the completion of the associated @ref asynchronous_methods "asynchronous method".
  * Events and their associated methods are listed below.
  */
  typedef enum {
    STOP,                //!< FEConsumer::stop_async()
    AUTHENTICATE,        //!< FEConsumer::authenticate_async()
    CONNECTION_CHANGED,  //!< This Event can be spawned independently
                         //!< of any asynchronous function call.
                         //!< Whenever the FEConsumer was connected
                         //!< and becomes disconnected or was
                         //!< disconnected and becomes connected, this
                         //!< Event will be spawned. Note that this is
                         //!< true even if the synchronous interface
                         //!< is used.
    CONNECTION_RECOVERY, //!< Similar to CONNECTION_CHANGED, this
                         //!< event will be spawned after an attempt
                         //!< to recover a lost connection to the
                         //!< server to indicate the success of that
                         //!< attempt
     CONNECT,            //!< FEConsumer::connect_async()
     DISCONNECT,         //!< FEConsumer::disconnect_async()
     LOGIN,              //!< FEConsumer::log_in_async()
     SUBSCRIBE,          //!< FEConsumer::subscribe_async()
     UNSUBSCRIBE,        //!< FEConsumer::unsubscribe_async()
     REQUEST_FILE,       //!< FEConsumer::request_file_async()
     SNAPSHOT_COMPLETE,  //!< FEConsumer::subscribe() or FEConsumer::subscribe_async() with snapshots
     RECOVER,            //!< FEConsumer::recover_async()
     RECOVERY_COMPLETE,  //!< FEConsumer::recover() or FEConsumer::recover_async()
     COUNT               //!< Dummy value indicating number of EventTypes
  } Event_EventType;

  typedef struct EventHandle Event;

  FDSFE_C_API Event* Event_new();

  /*! @brief Copy constructor
  *
  * Creates a new Event with the contents copied from \p other
  *
  * @param other Event to copy contents from
  *
  * @return Pointer to a new Event with contents copied from \p other
  */
  FDSFE_C_API Event* Event_new_copy(const Event* const other);

  /*! @brief Frees an Event allocated with Event_new_copy()
  *
  * @param e Event to be freed
  */
  FDSFE_C_API void Event_delete(Event* const e);

  /*! @brief Event copy method
  *
  * Copies the contents from \p other into \e
  *
  * @param e Event to copy contents into
  * @param other Event to copy contents from
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Event_copy(Event* const e, const Event* const other);

  /*! @brief Gets the Error associated with Event e
  *
  * @param e Event to get Error from
  * @param error_out Error pointer where the error will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Event_get_error(const Event* const e, Error** error_out);

  /*! @brief Gets the Event_EventType of an Event
  *
  * @param e Event to get the type from
  * @param event_type_out Event_EventType pointer where the type will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Event_get_type(const Event* const e, Event_EventType* event_type_out);

  /*! @brief Gets the unique id of an Event
  *
  * @param e Event to get the id from
  * @param id_out C-string pointer where the unique id will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Event_get_id(const Event* const e, const char** id_out);

  /*! @brief Checks if a value with the given key exists in the Event
  *
  * Note that the return value is used to indicate whether this operation is successful,
  * and the result is passed via the out parameter
  *
  * @param e Event to check for value
  * @param key Key to check the existence of
  * @param value_out Bool pointer where the result of the search is stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Event_has_value(const Event* const e, const char* const key, bool* value_out);

  /*! @brief Gets the value for the given key in an Event
  *
  * Some events will contain additional information depending on the
  * type of the event. See below for tables containing keys by event
  * type and their meanings.
  *
  * #AUTHENTICATE
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | Host | std::string | Value of host argument from request. |
  * | User | std::string | Value of user argument from request. |
  * | Serial | std::string | Value of serial argument from request. |
  * | KeyId | std::string | Value of key_id argument from request. |
  * | Key | std::string | Value of key argument from request. |
  * | Counter | std::string | Value of counter argument from request. |
  * | Path | std::string | Value of path argument from request. |
  * | ForceInput | bool | Value of force_input argument from request. |
  *
  * #SUBSCRIBE
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | Topic | std::string | Value of topic argument from request. |
  * | SnapshotMode | FEConsumer::SnapshotMode | Value of snapshot_mode argument from request. |
  * | DataMode | FEConsumer::DataMode | Value of data_mode argument from request. |
  *
  * #UNSUBSCRIBE
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | Topic | std::string | Value of topic argument from request. |
  *
  * #CONNECTION_CHANGED
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | Connected | bool | True if the FEConsumer is currently connected, false otherwise. |
  *
  * #CONNECTION_RECOVERY
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | LastHeartbeat | std::chrono::system_clock::rep | Timestamp of the last heartbeat received in number of ticks since epoch. |
  *
  * #LOG_IN
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | RequestFiles | bool | Value of request_files argument from request. |
  *
  * #REQUEST_FILE
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * |Filename | string | Value of filename argument from request. |
  *
  * #SNAPSHOT_COMPLETE
  * | Key | Type | Description |
  * | --- | ---- | ------- |
  * | Topic | std::string | Value of topic argument from request. |
  * | SnapshotCount | uint32_t | Number of snapshot messages received. |
  *
  * @param e Event to get the value from
  * @param key Key to query
  * @param value_out C-string pointer where the retrieved value will be stored
  *
  * @returns True if the operation is successful, false otherwise. Note this will be true even if the query is successfully performed but the key is not present in the Event
  */
  FDSFE_C_API bool Event_get_value(const Event* const e, const char* const key, const char** value_out);

  /*! @brief Gets all key-value pairs in the Event
  *
  * @param e Event to get values from
  * @param values_out C-string pointer where the values will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Event_get_all_values(const Event* const e, const char** values_out);

#ifdef __cplusplus
}
#endif

#endif //__EVENT_C_H__
