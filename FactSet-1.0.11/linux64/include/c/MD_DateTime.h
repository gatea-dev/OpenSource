/**
 * @file MD_DateTime.h
 * @brief Definitions for the DateTime type used by MD_Message.
 */

#ifndef __MD_DATETIME_C_H__
#define __MD_DATETIME_C_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "c/MD_Message_Api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Opaque struct used by MD_Message to represent dates and times.
*/
typedef struct MD_DateTimeHandle MD_DateTime;

/*! @brief Allocates a new MD_DateTime representing a date and time.
*
* An MD_DateTime allocated with this function should be freed using MD_DateTime_delete().
*
* @param date The date in the format YYYYMMDD, e.g. 19950316 for March 16th, 1995.
* @param time The time in the format HHMMSSmmm, e.g. 163254123 for 16:32:54.123.
* @param exch_gmt_offset GMT offset
* @return Pointer to a new MD_DateTime representing the given date and time.
*/
MD_MESSAGE_C_API MD_DateTime* MD_DateTime_new(int date,
                                              int time,
                                              int16_t exch_gmt_offset);

/*! @brief Allocates a new empty MD_DateTime.
*
* An MD_DateTime allocated with this function should be freed using MD_DateTime_delete().
*
* @return Pointer to a new MD_DateTime representing no date or time.
*/
MD_MESSAGE_C_API MD_DateTime* MD_DateTime_new_empty();

/*! @brief Frees an MD_DateTime allocated with MD_DateTime_new() or MD_DateTime_new_empty().
*
* @param d The MD_DateTime to free.
*/
MD_MESSAGE_C_API void MD_DateTime_delete(MD_DateTime* d);

/*! @brief Serialize an MD_DateTime to a buffer.
*
* The MD_DateTime_serialized_size() function should be used to determine the size of an MD_DateTime when serialized.
* This function performs no bounds checking, so dst \b must be at least MD_DateTime_serialized_size() bytes long.
*
* @param dst Char buffer to which the MD_DateTime will be serialized.
* @param src The MD_DateTime to serialize.
* @return True if the operation was successful, false otherwise.
*/
MD_MESSAGE_C_API bool MD_DateTime_serialize(char* dst,
                                            const MD_DateTime* const src);

/*! @brief Deserialize an MD_DateTime from a buffer.
*
* This function performs no bounds checking, so src \b must be at least MD_DateTime_serialized_size() bytes long.
*
* @param dst MD_DateTime to which the buffer will be deserialized.
* @param src The char buffer to deserialize.
* @return True if the operation was successful, false otherwise.
*/
MD_MESSAGE_C_API bool MD_DateTime_deserialize(MD_DateTime* dst,
                                              const char* const src);

/*! @brief Returns the size of an MD_DateTime when serialized.
*
* @see MD_DateTime_serialize()
* @see MD_DateTime_deserialize()
*
* @return The size of an MD_DateTime when serialized.
*/
MD_MESSAGE_C_API size_t MD_DateTime_serialized_size();

/*! @brief Retrieves the date from an MD_DateTime.
*
* The date will be in the format YYYYMMDD, e.g. 19950316 for March 16th, 1995.
*
* @param d The MD_DateTime from which to retreive the date.
* @param result_out Pointer to int where the result will the stored.
* @return True if the operation was successful, false otherwise.
*/
MD_MESSAGE_C_API bool MD_DateTime_date(MD_DateTime* d, int* result_out);

/*! @brief Retrieves the time from an MD_DateTime.
*
* The time in the format HHMMSSmmm, e.g. 163254123 for 16:32:54.123.
*
* @param d The MD_DateTime from which to retreive the time.
* @param result_out Pointer to int where the result will the stored.
* @return True if the operation was successful, false otherwise.
*/
MD_MESSAGE_C_API bool MD_DateTime_time(MD_DateTime* d, int* result_out);

/*! @brief Retrieves the GMT offset from an MD_DateTime.
*
* @param d The MD_DateTime from which to retreive the GMT offset.
* @param result_out Pointer to int16_t where the result will the stored.
* @return True if the operation was successful, false otherwise.
*/
MD_MESSAGE_C_API bool MD_DateTime_exch_gmt_offset(MD_DateTime* d,
                                                  int16_t* result_out);

#ifdef __cplusplus
}
#endif

#endif
