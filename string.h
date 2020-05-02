#ifndef STRING_HEADER_FILE
#define STRING_HEADER_FILE

// Standard Library
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** 
 * Tests if two string are equals
 * @param str1
 * @param str2
 * @return boolean
 */
short int strequals(const char * str1, const char * str2);

/** 
 * Allocate then copy a string to another one
 * @param dst
 * @param src
 * @return boolean
 */
char * astrcpy(const char * src);

/** 
 * Allocate then copy a string to another one
 * @param dst
 * @param src
 * @return boolean
 */
int rastrcpy(char * dst, const char * src);

/** 
 * Allocate a string
 * @param size
 * @return boolean
 */
char * strinit(int size);

/** 
 * Erase all sames character
 * @param string
 * @param character
 */
char * streral(const char *, char);

/** 
 * Erase all sames character
 * @param string
 * @param character
 * @param character
 */
int strrepc(char *, char, char);


#endif // CODEGEN_PROJECT_HEADER_FILE