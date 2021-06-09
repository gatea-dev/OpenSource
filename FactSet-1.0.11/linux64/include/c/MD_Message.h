/**
 * @file MD_Message.h
 * @brief Message type used to deliver market data.
 */

#ifndef __MD_MESSAGE_C_H__
#define __MD_MESSAGE_C_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "MD_DateTime.h"
#include "MD_Message_Api.h"
#include "MD_FieldMap.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Opaque struct used to represent market data.
 */
typedef struct MD_MessageHandle MD_Message;

/*! @brief Struct used to store binary data.
 */
typedef struct Buffer {
    const void* ptr;  ///< Pointer to start of binary data.
    uint16_t size;    ///< Length of the data.
} Buffer;

/*! @brief Struct used to store strings.
 */
typedef struct String {
    const char* ptr;  ///< Pointer to start of string.
    uint16_t size;  ///< Length of the string, not including the null character
                    ///< if the string is also null terminated
} String;

/*! @brief Allocates a new MD_Message using the mInt format.
 *
 * An MD_Message allocated with this function should be freed using
 * MD_Message_delete().
 *
 * @return Pointer to a new empty MD_Message in mInt format.
 */
MD_MESSAGE_C_API MD_Message* MD_Message_new_mInt();

/*! @brief Allocates a new MD_Message that is a copy of another message.
 *
 * An MD_Message allocated with this function should be freed using
 * MD_Message_delete().
 *
 * @return Pointer to a new empty MD_Message in mInt format.
 */
MD_MESSAGE_C_API MD_Message* MD_Message_clone(MD_Message* m);

/*! @brief Frees an MD_Message allocated with MD_Message_new_mInt(),
 * MD_Message_createFromBytes(), or MD_Message_clone().
 *
 * @param m The MD_Message to free.
 */
MD_MESSAGE_C_API void MD_Message_delete(MD_Message* m);

/*! @brief Allocates a new MD_Message from the given binary data.
 *
 * @param data Pointer to the start of binary data.
 * @param length Length of data.
 *
 * @return Pointer to a new MD_Message representing the given data.
 */
MD_MESSAGE_C_API MD_Message* MD_Message_create_from_bytes(
    const unsigned char* const data, unsigned int length);

/*! @brief Swaps the contents of two MD_Messages.
 *
 * @param m1 Message to swap with \p m2.
 * @param m2 Message to swap with \p m1.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_swap(MD_Message* m1, MD_Message* m2);

/*! @brief Returns a pointer to an MD_Message's binary data buffer.
 *
 * The resulting data is MD_Message_get_size() bytes long.
 *
 * @param m MD_Message from which to retrieve the binary.
 * @param data Pointer to const unsigned char* where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_binary(const MD_Message* const m,
                                            const unsigned char** data);

/*! @brief Updates an existing MD_Message with the values from the given binary
 * data.
 *
 * @param m MD_Message to update.
 * @param data Pointer to the start of binary data.
 * @param length Length of data.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_set_binary(const MD_Message* const m,
                                            const unsigned char* data,
                                            unsigned int length);

/*! @brief Reads a char field from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the char field to read.
 * @param result_out Pointer to char where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_char(const MD_Message* const m,
                                          FieldID fid,
                                          char* result_out);

/*! @brief Reads an int field from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the int field to read.
 * @param result_out Pointer to int where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_int(const MD_Message* const m,
                                         FieldID fid,
                                         int* result_out);

/*! @brief Reads an int64_t field from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the int64_t field to read.
 * @param result_out Pointer to int64_t where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_int64(const MD_Message* const m,
                                           FieldID fid,
                                           int64_t* result_out);

/*! @brief Reads a float field from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the float field to read.
 * @param result_out Pointer to float where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_float(const MD_Message* const m,
                                           FieldID fid,
                                           float* result_out);

/*! @brief Reads a double field from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the double field to read.
 * @param result_out Pointer to double where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_double(const MD_Message* const m,
                                            FieldID fid,
                                            double* result_out);

/*! @brief Reads a string field from an MD_Message to a String.
 *
 * @param m MD_Message to read.
 * @param fid Id of the string field to read.
 * @param result_out Pointer to String where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_string(const MD_Message* const m,
                                            FieldID fid,
                                            String* result_out);

/*! @brief Reads a string field from an MD_Message to a null-terminated cstring.
 *
 * @param m MD_Message to read.
 * @param fid Id of the string field to read.
 * @param result_out Pointer to const char* where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_cstring(const MD_Message* const m,
                                             FieldID fid,
                                             const char** result_out);

/*! @brief Reads binary data from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the binary field to read.
 * @param result_out Pointer to Buffer where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_buffer(const MD_Message* const m,
                                            FieldID fid,
                                            Buffer* result_out);

/*! @brief Reads datetime data from an MD_Message.
 *
 * @param m MD_Message to read.
 * @param fid Id of the datetime field to read.
 * @param result_out Pointer to MD_DateTime where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_datetime(const MD_Message* const m,
                                              FieldID fid,
                                              MD_DateTime* result_out);

/*! @brief Reads a timestamp from an MD_Message in nanoseconds since epoch
 * format.
 *
 * @param m MD_Message to read.
 * @param fid Id of the epoch_nsec field to read.
 * @param result_out Pointer to uint64_t where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_epoch_ns(const MD_Message* const m,
                                              FieldID fid,
                                              uint64_t* result_out);

/*! @brief Adds a char field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_char() to update an existing char field.
 *
 * @param m MD_Message to which to add a new char field.
 * @param fid Id of the char field to add.
 * @param data Char to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_char(const MD_Message* const m,
                                          FieldID fid,
                                          char data);
                        
/*! @brief Adds an int field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_int() to update an existing int field.
 *
 * @param m MD_Message to which to add a new int field.
 * @param fid Id of the int field to add.
 * @param data Int to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_int(const MD_Message* const m,
                                         FieldID fid,
                                         int data);

/*! @brief Adds an int64 field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_int64() to update an existing int64 field.
 *
 * @param m MD_Message to which to add a new int64 field.
 * @param fid Id of the int64 field to add.
 * @param data Int64 to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_int64(const MD_Message* const m,
                                           FieldID fid,
                                           int64_t data);

/*! @brief Adds a float field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_float() to update an existing float field.
 *
 * @param m MD_Message to which to add a new float field.
 * @param fid Id of the float field to add.
 * @param data Float to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_float(const MD_Message* const m,
                                           FieldID fid,
                                           float data);

/*! @brief Adds a double field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_double() to update an existing double field.
 *
 * @param m MD_Message to which to add a new double field.
 * @param fid Id of the double field to add.
 * @param data Double to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_double(const MD_Message* const m,
                                            FieldID fid,
                                            double data);

/*! @brief Adds a string field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_string() or MD_Message_update_cstring()
 * to update an existing string field.
 *
 * @param m MD_Message to which to add a new string field.
 * @param fid Id of the string field to add.
 * @param data String to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_string(const MD_Message* const m,
                                            FieldID fid,
                                            String data);

/*! @brief Adds a string field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_string() or MD_Message_update_cstring()
 * to update an existing string field.
 *
 * @param m MD_Message to which to add a new string field.
 * @param fid Id of the string field to add.
 * @param data C string to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_cstring(const MD_Message* const m,
                                             FieldID fid,
                                             const char* const data);

/*! @brief Adds a Buffer field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_buffer() to update an existing Buffer field.
 *
 * @param m MD_Message to which to add a new Buffer field.
 * @param fid Id of the Buffer field to add.
 * @param data Buffer to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_buffer(const MD_Message* const m,
                                            FieldID fid,
                                            Buffer data);

/*! @brief Adds an datetime field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_datetime() to update an existing datetime field.
 *
 * @param m MD_Message to which to add a new datetime field.
 * @param fid Id of the datetime field to add.
 * @param data Datetime to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_datetime(const MD_Message* const m,
                                              FieldID fid,
                                              MD_DateTime* data);

/*! @brief Adds an epoch_nsec field to an MD_Message.
 *
 * If \p fid already exists in \p m, this function will fail. 
 * Use MD_Message_update_epoch_ns() to update an existing epoch_nsec field.
 *
 * @param m MD_Message to which to add a new epoch_nsec field.
 * @param fid Id of the epoch_nsec field to add.
 * @param data epoch_nsec to add to \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_add_epoch_ns(const MD_Message* const m,
                                              FieldID fid,
                                              uint64_t data);

/*! @brief Updates a char field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_char(const MD_Message* const m,
                                             FieldID fid,
                                             char data);

/*! @brief Updates an int field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_int(const MD_Message* const m,
                                            FieldID fid,
                                            int data);

/*! @brief Updates an int64 field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_int64(const MD_Message* const m,
                                              FieldID fid,
                                              int64_t data);

/*! @brief Updates a float field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_float(const MD_Message* const m,
                                              FieldID fid,
                                              float data);

/*! @brief Updates a double field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_double(const MD_Message* const m,
                                               FieldID fid,
                                               double data);

/*! @brief Updates a string field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_string(const MD_Message* const m,
                                               FieldID fid,
                                               String data);

/*! @brief Updates a string field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_cstring(const MD_Message* const m,
                                                FieldID fid,
                                                const char* const data);

/*! @brief Updates a Buffer field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_buffer(const MD_Message* const m,
                                               FieldID fid,
                                               Buffer data);

/*! @brief Updates a datetime field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_datetime(const MD_Message* const m,
                                                 FieldID fid,
                                                 MD_DateTime* data);

/*! @brief Updates an epoch_ns field in an MD_Message.
 *
 * If \p fid does not exist in \p m, it will be added with the given value.
 *
 * @param m MD_Message which will be updated.
 * @param fid Id of the field to update.
 * @param data New value for \p fid in \p m.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_update_epoch_ns(const MD_Message* const m,
                                                 FieldID fid,
                                                 uint64_t data);

/*! @brief Erases a field from an MD_Message.
 *
 * @param m MD_Message from which a field will be erased.
 * @param fid Id of the field to erase.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_erase(const MD_Message* const m, FieldID fid);

/*! @brief Erases multiple fields from an MD_Message.
 *
 * @param m MD_Message from which fields will be erased.
 * @param fids Pointer to the start of the array of field ids to erase.
 * @param fids_length Number of field ids in \p fids.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_erase_array(const MD_Message* const m,
                                             FieldID* fids,
                                             size_t fids_length);

/*! @brief Erases all fields from an MD_Message.
 *
 * @param m MD_Message from which all fields will be erased.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API void MD_Message_clear(const MD_Message* const m);

/*! @brief Checks if an MD_Message is empty.
 *
 * @param m MD_Message to check.
 *
 * @return True if \p m has 0 fields, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_empty(const MD_Message* const m);

/*! @brief Checks if an MD_Message contains the given field.
 *
 * @param m MD_Message to check for \p fid.
 * @param fid Field id to check in \p m.
 *
 * @return True if \p m contains a value for \p fid, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_exists(const MD_Message* const m, FieldID fid);

/*! @brief Returns the size in bytes of the MD_Message's binary data.
 *
 * @param m MD_Message for which to get the size.
 *
 * @return Size in bytes of \p m.
 */
MD_MESSAGE_C_API uint32_t MD_Message_get_size(const MD_Message* const m);

/*! @brief Returns the number of fields in an MD_Message.
 *
 * @param m MD_Message for which to get the field count.
 *
 * @return Number of fields in \p m.
 */
MD_MESSAGE_C_API size_t MD_Message_field_count(const MD_Message* const m);

/*! @brief Retrieves value of whole, num, and denom fields as a double.
 *
 * The resulting double will be equal to whole + num/denom.
 *
 * @param m MD_Message from which to read \p whole_id, \p num_id, and \p denom_id.
 * @param whole_id Field id for whole number.
 * @param num_id Field id for numerator.
 * @param denom_id Field id for denominator.
 * @param result_out Pointer to double where the result will be stored.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_get_wnd(const MD_Message* const m,
                                         FieldID whole_id,
                                         FieldID num_id,
                                         FieldID denom_id,
                                         double* result_out);

/*! @brief Prints an MD_Message to stdout.
 *
 * @param m MD_Message to print.
 *
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_Message_print(const MD_Message* const m);

/*! @brief Populates a preallocated array with the fields in an MD_Message.
 *
 * Callers of this function should preallocate \p fids_out to the length of
 * MD_Message_field_count().
 *
 * @param m MD_Message to read for fields.
 * @param fids_out Preallocated array of FieldID to populate with the fields in \p m.
 * @param fids_out_length Length of \p fids_out.
 *
 * @return Number of fields in \p m.
 */
MD_MESSAGE_C_API bool MD_Message_get_field_ids(const MD_Message* const m,
                                               FieldID* fids_out,
                                               size_t fids_out_length);

#ifdef __cplusplus
}
#endif

#endif
