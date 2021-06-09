/// @file Error_C.h
#ifndef __ERROR_C_H__
#define __ERROR_C_H__

#include "fdsfe_Api_C.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

  /*! @brief Set of possible error codes. */
  typedef enum {
    FE_NO_ERROR,    //!< The operation completed successfully and no error has occurred
    FE_E_UNKNOWN,   //!< Unexpected failure due to unknown reasons
    FE_E_NO_SERV,   //!< The requested service was unavailable
    FE_E_NOT_FOUND, //!< A required resource was not found
    FE_E_RENAME,    //!< A required resource was renamed
    FE_E_EXISTS,    //!< The output of this operation already exists
    FE_E_LIMIT,     //!< A resource limit was reached
    FE_E_PROTOCOL,  //!< An error occurred in the communication protocol
    FE_E_INVAL,     //!< The request was invalid
    FE_E_RESOURCE,  //!< A required resource was exhausted
    FE_E_NO_CONN,   //!< There was no connection to the server
    FE_E_VERSION,   //!< The operation was invalid for the current version
    FE_E_SHUTDOWN,  //!< Operation failed because the system is shutting down
    FE_E_ACCESS,    //!< The user lacked the necessary permissions
    FE_E_TIMEOUT,   //!< The operation timed out
    FE_E_UNEXPECTED_SUBSCRIBE,  //!< An unexpected subscribe response was received
    FE_E_UNEXPECTED_UNSUBSCRIBE //!< An unexpected unsubscribe response was received
  } Error_ErrorCode;

  typedef struct ErrorHandle Error;

  /*! @brief No args constructor
  *
  * Constructs a new Error with error code FE_NO_ERROR and an empty description string.
  */
  FDSFE_C_API Error* Error_new();
  
  /*! @brief Constructor
  *
  * Constructs a new Error with the given error code and description.
  *
  * @param code The Error_ErrorCode of the error
  * @param description The text description of the error
  *
  * @return Pointer to a new Error with the provided code and description
  */
  FDSFE_C_API Error* Error_new_args(Error_ErrorCode code, const char* const description);

  /*! @brief Copy constructor
  *
  * Constructs a new Error with the contents copied from other
  *
  * @param other The Error to copy from
  *
  * @return Pointer to a new Error with the same Error_ErrorCode and description as \p other
  */
  FDSFE_C_API Error* Error_new_copy(const Error* const other);

  /*! @brief Frees an Error allocated with Error_new(), Error_new_args() or Error_new_copy()
  *
  * @param e The Error to free
  */
  FDSFE_C_API void Error_delete(Error* const e);

  /*! @brief Copies contents of one Error into another
  *
  * @param e Error into which contents should be copied
  * @param other Error from which contents should be copied
  *
  * @return True if the operation is successful, false otherwise
  */
  FDSFE_C_API bool Error_copy(Error* const e, const Error* const other);

  /*! @brief Gets the Error_ErrorCode of the provided Error
  *
  * @param e Error to get the error code from
  * @param error_code_out The error's error code
  *
  * @return True if the operation was successful, false otherwise
  */
  FDSFE_C_API bool Error_get_error_code(const Error* const e, Error_ErrorCode* error_code_out);

  /*! @brief Gets the description of the provided Error
  *
  * @param e Error to get the description from
  * @param description_out The error's description
  *
  * @return True if the operation was successful, false otherwise
  */
  FDSFE_C_API bool Error_get_description(const Error* const e, const char** description_out);

  /*! @brief Equality test for Errors
  *
  * Errors are considered equal if they have the same Error_ErrorCode and description.
  * Note that the result of the comparison is stored into the \p result_out parameter
  * and not returned as the  return value, which is used to indicate whether an issue
  * occurred while performing the comparison
  *
  * @param e The first error to be used in the comparison
  * @param other The second error to be used in the comparison
  * @param result_out True if \p e and \p other have the same Error_ErrorCode and description
  *
  * @return True if the operation was successful, false otherwise
  */
  FDSFE_C_API bool Error_equals(const Error* const e, const Error* const other, bool* result_out);

  /*! @brief Error indicating that no error has occurred. */
  FDSFE_C_API bool Error_NoError(const Error* const e, bool* result_out);

#ifdef __cplusplus
}
#endif

#endif //__ERROR_C_H__
