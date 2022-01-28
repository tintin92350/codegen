/**
 * @author: Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @date:   1970-01-01 01:00:00
 * @lastModifiedBy:   Quentin RODIC <quentin.rodic.pro@outlook.fr>
 * @lastModifiedTime: 2022-01-28 19:54:01
 */

#ifndef UTILS_STRING_HEADER
#define UTILS_STRING_HEADER

/**
 * @brief 
 * 
 * Copy src string into dst
 * 
 * @param src 
 * @param dst 
 */
void copy_string(const char *src, char **dst);

/**
 * @brief 
 * 
 * Split src into two another string dst1 and dst2
 * Use del to tell which delimiter use.
 * 
 * @param src 
 * @param dst1 
 * @param dst2 
 * @param del 
 */
void split_string_in_two(char *src, char **dst1, char **dst2, const char *del);

#endif // UTILS_STRING_HEADER