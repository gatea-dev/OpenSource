/// @file Log_Message_C.h
#ifndef __LOG_MESSAGE_C_H__
#define __LOG_MESSAGE_C_H__

#include "fdsfe_Api_C.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef enum {
    LOG_ALL = 0,
    LOG_DEBUG = 1,
    LOG_INFO = 2,
    LOG_WARN = 3,
    LOG_ERROR = 4,
    LOG_PANIC = 5,
    LOG_NONE = 6
  } LogMessage_LogLevel;

  typedef struct LogMessageHandle LogMessage;

  FDSFE_C_API LogMessage* LogMessage_new();

  /*! @brief Copy constructor
  *
  * Constructs a new LogMessage, copying the contents from \p other
  *
  * @param other LogMessage to copy contents from
  *
  * @return Pointer to a new LogMessage with contents copied from \p other
  */
  FDSFE_C_API LogMessage* LogMessage_new_copy(const LogMessage* const other);

  /*! @brief Frees a LogMessage allocated with LogMessage_new_copy()
  *
  * @param m LogMessage to free
  */
  FDSFE_C_API void LogMessage_delete(LogMessage* const m);

  /*! @brief Gets the text from a LogMessage
  *
  * @param m LogMessage to read from
  * @param result_out C-string pointer where the text will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_text(const LogMessage* const m, const char** result_out);

  /*! @brief Gets the time from a LogMessage as seconds past unix epoch
  *
  * @param m LogMessage to read from
  * @param result_out Numeric pointer where the time will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_time(const LogMessage* const m, int64_t* result_out);

  /*! @brief Gets the number of microseconds past the last second that the
  *  provided message was logged
  *
  * @param m LogMessage to read from
  * @param result_out uint pointer where the number of microseconds will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_microseconds(const LogMessage* const m, unsigned int* result_out);

  /*! @brief Gets the LogMessage_LogLevel from a LogMessage
  *
  * @param m LogMessage to read from
  * @param result_out LogMessage_LogLevel pointer where the log level will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_level(const LogMessage* const m, LogMessage_LogLevel* result_out);

  /*! @brief Gets the filename from a LogMessage
  *
  * @param m LogMessage to read from
  * @param result_out C-string pointer where the filename will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_filename(const LogMessage* const m, const char** result_out);

  /*! @brief Gets the line number from a LogMessage
  *
  * @param m LogMessage to read from
  * @param m int pointer where the line number will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_line_number(const LogMessage* const m, int* result_out);

  /*! @brief Gets the thread id from a LogMessage
  *
  * @param m LogMessage to read from
  * @param result_out uint64_t pointer where the thread id will be stored
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool LogMessage_get_thread_id(const LogMessage* const m, uint64_t* result_out);

#ifdef __cplusplus
}
#endif

#endif //__LOGMESSAGE_C_H__
