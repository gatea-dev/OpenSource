/**
 * @file MD_FieldMap.h
 * @brief Mapping between field names, types, and ids for MD_Message.
 */

#ifndef __MD_FIELDMAP_C_H__
#define __MD_FIELDMAP_C_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "c/MD_Message_Api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Integer type used to identify field types within an MD_Message.
 */
typedef int TypeID;

/*! @brief Integer type used to identify specific fields within an MD_Message.
 */
typedef uint16_t FieldID;

/*! @brief Loads a field definition mapping from the given filepath.
 *
 * This function will fail if a field map is already loaded. To reload a
 * field map use MD_FieldMap_reload(), MD_FieldMap_reload_xml(), or
 * MD_FieldMap_reload_text().
 *
 * @param file Path of file to load.
 * @return 0 if operation was successful, an error code otherwise.
 */
MD_MESSAGE_C_API int MD_FieldMap_load_file(const char* const file);

/*! @brief Destroys the existing field map, if one exists.
 */
MD_MESSAGE_C_API void MD_FieldMap_destroy();

/*! @brief Reloads the field definition mapping.
 *
 * When called with a filename, this function is equivalent MD_FieldMap_reload_xml().
 * If file is an empty string, the field map will be reloaded using the default search path.
 * 
 * @param file Path of file to load or an empty string.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_reload(const char* const file);

/*! @brief Reloads the field definition mapping from an xml file.
 * 
 * @param file Path of xml file to load.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_reload_xml(const char* const file);

/*! @brief Reloads the field definition mapping from a text file.
 * 
 * @param file Path of text file to load.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_reload_text(const char* const file);

/*! @brief Returns the number of fields in the field map.
 * 
 * @return Number of fields in the field map.
 */
MD_MESSAGE_C_API size_t MD_FieldMap_size();

/*! @brief Retrieves the type of the field with the given id.
 * 
 * @param id Id of field for which to retrieve the type.
 * @param type_out Pointer to TypeID where the result will be stored.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_id_to_type(FieldID id, TypeID* type_out); 

/*! @brief Retrieves the name of the field with the given id.
 * 
 * @param id Id of field for which to retrieve the name.
 * @param name_out Pointer to const char* where the result will be stored.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_id_to_name(FieldID id, const char** name_out);

/*! @brief Retrieves the id of the field with the given name.
 * 
 * @param name Name of field for which to retrieve the id.
 * @param id_out Pointer to FieldID where the result will be stored.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_name_to_id(const char* const name, FieldID* id_out);

/*! @brief Retrieves the type of the field with the given name.
 * 
 * @param name Name of field for which to retrieve the type.
 * @param type_out Pointer to TypeID where the result will be stored.
 * @return True if the operation was successful, false otherwise.
 */
MD_MESSAGE_C_API bool MD_FieldMap_name_to_type(const char* const name, TypeID* type_out);

#ifdef __cplusplus
}
#endif

#endif
