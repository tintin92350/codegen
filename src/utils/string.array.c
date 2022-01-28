#include "string.array.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief 
 * 
 * Insert array size with initial_size passed as argument.
 * Then allocate the array with the size even when 0
 * 
 * @param initial_size Initial positive size of the array
 * @return string_array Initialized strin garray
 */
string_array_t string_array_init(unsigned int initial_size)
{
    string_array_t array;

    array.size = initial_size;
    array.values = (char **)malloc(sizeof(char) * array.size);

    memset(array.values, '\0', array.size);

    return array;
}

/**
 * @brief Initialize an empty array with a size of -1
 * 
 * @return string_array_t Empty array
 */
string_array_t string_array_empty()
{
    string_array_t array;

    array.size = -1;
    array.values = NULL;

    return array;
}

/**
 * @brief
 * 
 * Pass the new size of the array then return new sized array
 * 
 * @param new_size New size of the array
 * @return string_array 
 */
int string_array_resize(string_array_t *array, unsigned int new_size)
{
    char **buffer = (char **)realloc(array->values, sizeof(char *) * new_size);

    if (buffer == NULL)
    {
        free(buffer);
        free(array->values);
        return 0;
    }

    array->values = buffer;
    array->size = new_size;

    return 1;
}

/**
 * @brief
 * 
 * Return true if array is set to be empty
 * 
 * @param array Array to check
 * @return string_array 
 */
int string_array_is_empty(string_array_t *array)
{
    return array->size == -1;
}