/**
 * vim: set ts=4 :
 * =============================================================================
 * SM JSON (C) 2012 Zach "theY4Kman" Kanzler
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

enum JsonType {
    JSON_OBJECT=0,
    JSON_ARRAY,
    JSON_STRING,
    JSON_INTEGER,
    JSON_REAL,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
};

// Flags for dumping JSON objects and values
#define JSON_INDENT(%1)     ((%1) & 0x1F)   /* Pretty-print the result, using newlines between array and object items, and indenting with %1 spaces (0-31). */
#define JSON_COMPACT        0x20            /* Don't print spaces after separators */
#define JSON_ENSURE_ASCII   0x40            /* Escapes all Unicode characters outside ASCII range */
#define JSON_SORT_KEYS      0x80            /* Objects are sorted by key */
#define JSON_PRESERVE_ORDER 0x100           /* Objects are printed in the order they were inserted into the JSON object */
#define JSON_ENCODE_ANY     0x200           /* Allows any value to be encoded, not just arrays and objects (violates strict compatiblity with RFC 4627) */
#define JSON_ESCAPE_SLASH   0x400

/**
 * Parses JSON-encoded data and returns a handle to a JSON value.
 * 
 * @param   json    JSON-encoded data in a string
 * @param   error   If parsing is unsuccessful, will contain an error message
 *                      explaining why.
 * @return  A handle to a JSON value for reading the object data, or
 *              INVALID_HANDLE if parsing was unsuccessful.
 */
native Handle:JSON_Parse(const String:json[]="",
                         String:error[]=NULL_STRING, maxlength=0);


/**
 * Retrieve the string representation of 
 */
native JSON_Dumps(Handle:json_value, String:buffer[], maxlength, flags=JSON_ENCODE_ANY);

/**
 * Returns the type of the JSON value.
 * 
 * @param   json_value      A handle to a JSON value
 * @return  The type of the JSON value
 * @error   Invalid Handle
 */
native JsonType:JSON_Type(Handle:json_value);

/**
 * Retrieves the string value of a JSON value
 * 
 * @param   json_value      A handle to a JSON value
 * @param   buffer          Buffer to store the string value
 * @param   maxlength       Maximum size of the string buffer
 * @return  The number of characters written to the buffer, or -1 if the
 *              `json_value` is not a string.
 * @error   Invalid Handle
 */
native JSON_ReadString(Handle:json_value, String:buffer[],
                       maxlength);

/**
 * Returns the integer value of a JSON value
 * 
 * @param   json_value      A handle to a JSON value
 * @return  The integer value of the JSON value, or -1 if the `json_value` is
 *              not an integer.
 * @error   Invalid Handle
 */
native JSON_ReadCell(Handle:json_value);

/**
 * Returns the floating point value of a JSON value
 * 
 * @param   json_value      A handle to a JSON value
 * @return  The float value of the JSON value, or -1.0 if `json_value` is not
 *              a float (real number).
 * @error   Invalid Handle
 */
native Float:JSON_ReadFloat(Handle:json_value);

/**
 * Returns the size of a JSON array
 * 
 * @param   json_value      A handle to a JSON value
 * @return  The size of the JSON array, or -1 if the JSON value is not an array
 * @error   Invalid Handle
 */
native JSON_ArraySize(Handle:json_value);

/**
 * Returns the JSON value at the specified index in a JSON array.
 * 
 * @param   json_value      A handle to a JSON value
 * @param   index           The position of the value to retrieve from the JSON
 *                              array in `json_value`.
 * @return  A handle to a JSON value on success; INVALID_HANDLE if `json_value`
 *              is not a JSON array, or the index is out of range
 * @error   Invalid Handle
 */
native Handle:JSON_ArrayGet(Handle:json_value, index);

/**
 * Returns the number of elements in the JSON object.
 * 
 * @param   json_value      A handle to a JSON value
 * @return  The number of elements in the JSON object, or -1 if `json_value` is
 *              not a JSON object.
 * @error   Invalid Handle
 */
native JSON_ObjectSize(Handle:json_value);

/**
 * Returns the JSON value with the specified key in a JSON object.
 * 
 * @param   json_value      A handle to a JSON value
 * @param   key             The key of the object to retrieve from the JSON
 *                              object in `json_value`.
 * @return  A handle to a JSON value on success; INVALID_HANDLE if `json_value`
 *              is not a JSON object, or `key` does not exist in `json_value`
 * @error   Invalid Handle
 */
native JSON_ObjectGet(Handle:json_value, const String:key[]);

/**
 * Returns a handle to an iterator used to iterate over the keys and values in
 * a JSON object.
 * 
 * @param   json_value      A handle to a JSON value
 * @return  A handle to an iterator; INVALID_HANDLE if `json_value` is not a
 *              JSON object or is empty.
 * @note    Must be freed with CloseHandle() if not read till the end.
 *              Otherwise, CloseHandle() need not be called.
 * @error   Invalid Handle
 */
native Handle:JSON_ObjectIter(Handle:json_value);

/**
 * Reads the key and value from an object iterator.
 * 
 * @param   iter            A handle to a JSON object iterator
 * @param   key             Buffer to store the key in
 * @param   maxlength       Maximum size of the buffer
 * @return  A handle to the JSON value pointed at by the object iterator. This
 *              must be freed with CloseHandle().
 * @error   Invalid Handle
 */
native Handle:JSON_ObjectIterRead(Handle:iter,
                                  String:key[]=NULL_STRING,
                                  maxlength=0);

/**
 * Advances the object iterator to the next key-value pair.
 * 
 * @param   iter            A handle to a JSON object iterator
 * @return  The handle to the JSON object iterator advanced to the next
 *              key-value pair; INVALID_HANDLE if `iter` is invalid or there
 *              are no more items to iterate over in the object.
 * @error   Invalid Handle
 */
native Handle:JSON_ObjectIterNext(Handle:iter);


/**
 * Do not edit below this line!
 */
public Extension:__ext_json = 
{
	name = "JSON",
	file = "json.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};
