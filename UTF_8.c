#include "UTF_8.h"

/* input char, output array
 * input as decimal unicode
 * code point
 */
int wchar_to_mbchar(uint32_t *wChar,char_u* mbChar)
{
    /* WARNING - NO NULL DELIM */

    if(*wChar < 0x80) /* 1 byte chars */
    {
        *mbChar = *wChar;
        return 1;
    }
    else if(*wChar < 0x800) /* 2 byte chars */
    {
        mbChar[0] = 0xC0 + (*wChar >> 6);
        mbChar[1] = 0x80 + (*wChar - ((*wChar >> 6) << 6));
        return 2;
    }
    else if(*wChar < 0x10000) /* 3 byte chars */
    {
        mbChar[0] = 0xE0 + (*wChar >> 12);
        mbChar[1] = 0x80 + ((*wChar - ((*wChar >> 12) << 12)) >> 6);
        mbChar[2] = 0x80 + (*wChar - ((*wChar >> 6) << 6));
        return 3;
    }
    else if(*wChar < 0x200000) /* 4 byte chars */
    {
        mbChar[0] = 0xF0 + (*wChar >> 18);
        mbChar[1] = 0x80 + ((*wChar - ((*wChar >> 18) << 18)) >> 12);
        mbChar[2] = 0x80 + ((*wChar - ((*wChar >> 12) << 12)) >> 6);
        mbChar[3] = 0x80 + (*wChar - ((*wChar >> 6) << 6));
        return 4;
    }
    return -1;
}

/* array, char at */ 
int delete_mb_char(char_u* array,int arrayPos) 
{   int i;

    if(array[arrayPos] < 0xC0)
    {
        array[arrayPos] = '\0';
    }
    else if(array[arrayPos] < 0xE0 )
    {
        for(i = 0;i < 2;i++)
            array[arrayPos + i] = '\0';
    }
    else if(array[arrayPos] < 0xF0 )
    {
        for(i = 0;i < 3;i++)
            array[arrayPos + i] = '\0';
    }
    else
    {
        for(i = 0;i < 4;i++)
            array[arrayPos + i] = '\0';
    }

    return 0;
}

/* array, char to find 
 * returns -1 if no match
 * found
 */
int find_mb_char(char_u* array,char_u* findChar,int lineLength)
{   int i;
    if( findChar[0] < 0xC0 &&
        findChar[0] >= 0x80)
    {
        printf("warning; passed find_mb_char trailing character");
        return -1;
    }

    if(findChar[0] < 0x80)
    {
        for(i = 0;i < lineLength;i++)
        {
            if(array[i] == findChar[0])
                return i;
        }
    }
    else if(findChar[0] < 0xE0)
    {
        for(i = 0;i < lineLength;i++)
        {
            if( array[i] == findChar[0] &&
                array[i + 1] == findChar[1])
                return i;
        }
    }
    else if(findChar[0] < 0xF0)
    {
        for(i = 0;i < lineLength;i++)
        {
            if( array[i] == findChar[0]     &&
                array[i + 1] == findChar[1] &&
                array[i + 2] == findChar[2])
                return i;
        }
    }
    else
    {
        for(i = 0;i < lineLength;i++)
        {
            if( array[i] == findChar[0]     &&
                array[i + 1] == findChar[1] &&
                array[i + 2] == findChar[2] &&
                array[i + 3] == findChar[3])
                return i;
        }
    }
    /* return value -1 means no match found */
    return -1;
}

/* returns size of mb_char
 */
int sizeof_mb_char(char_u* sizeChar)
{
    if( *sizeChar < 0xC0 &&
        *sizeChar >= 0x80)
        return -1;

    if(*sizeChar < 0x80)
        return 1;
    else if(*sizeChar < 0xE0)
        return 2;
    else if(*sizeChar < 0xF0)
        return 3;
    else
        return 4;

    return 0;
}

/* returns pointer to first previous mb_char
 * returns same pointer if current byte is
 * a leading byte/single byte char
 */
char_u* find_prev_mb_char(char_u* currChar)
{
    if(*currChar < 0xC0 &&
        *currChar >= 0x80)
        return find_prev_mb_char(currChar - 1);
    return currChar;
}
