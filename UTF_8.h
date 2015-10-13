#ifndef _UTF8_H_
    #define _UTF8_H_

#include "main.h"
/* input char, output array
 * inserts mb_char in array
 * pointed at by char_u arg
 */
int wchar_to_mbchar(uint32_t*,char_u*); 

/* array, char position in array
 * deletes mb_char*/
int delete_mb_char(char_u*,int); 

/* array, char to find, line length
 * returns length along array match occurs
 * returns -1 otherwise
 */
int find_mb_char(char_u*,char_u*,int); 

/* returns length of mb_char
 * pointed at in char_u
 */
int sizeof_mb_char(char_u*);

/* returns pointer to first previous mb_char
 * returns same pointer if current byte is
 * a leading byte/single byte char
 */
char_u *find_prev_mb_char(char_u*);

/* returns pointer to next mb_char
 * similar to above
 */
char_u *find_next_mb_char(char_u*);

#endif
