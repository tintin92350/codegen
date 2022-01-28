/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   2022-01-28 18:33:17
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-28 19:23:37
 */

#ifndef UTILS_STRING_ARRAY_HEADER
#define UTILS_STRING_ARRAY_HEADER

struct string_array
{
    int size;
    char **values;
};

typedef struct string_array string_array_t;

/**
 * @brief Initialize a string array with an initial size as parameter
 * 
 * Pass the initial variable as parameter, then catch the result as
 * function's result.
 * 
 * @param initial_size Initial positive size of the array
 * @return string_array Initialized string array
 */
string_array_t string_array_init(unsigned int);

/**
 * @brief Initialize an empty array with a size of -1
 * 
 * @return string_array_t Empty array
 */
string_array_t string_array_empty();

/**
 * @brief
 * 
 * Pass the new size of the array then return new sized array
 * 
 * @param array Array to resize
 * @param new_size New size of the array
 * @return string_array 
 */
int string_array_resize(string_array_t *, unsigned int);

/**
 * @brief
 * 
 * Return true if array is set to be empty
 * 
 * @param array Array to check
 * @return string_array 
 */
int string_array_is_empty(string_array_t *);


#endif // UTILS_STRING_ARRAY_HEADER