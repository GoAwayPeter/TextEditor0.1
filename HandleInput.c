#include "HandleInput.h"

int handle_input(LBUFFER* currBuffer,int* key)
{
    switch(*key)
    {
        case KEY_MOUSE:
            break;
        case BACKSPACE:
            break;
            /* etc */
    }
    return 0;
}

/* Game of Life Input */

int golhandle_input(BUFFER* currBuffer,int* key)
{
    switch(*key)
    {
        case BACKSPACE:
            if(goldelete_char_buffer_at_cursor(currBuffer) == 1)
            {
                printf("error manipulating buffer (deletion)");
                break;
            }
            --MOUSE_X;
            break;
        case NEWLINE:
            if(golchange_buffer_size(currBuffer,INCREASE,1,0) == 1)
            {
                printf("error changing buffer size");
                break;
            }
            ++MOUSE_Y;
            MOUSE_X = 0;
            break;
        case DELETE:
            if(goldelete_char_buffer_at_cursor(currBuffer) == 1)
            {
                printf("error manipulating buffer (deletion)");
                break;
            }
            --MOUSE_X;
            break;
        default:
            if(goladd_char_buffer_at_cursor(currBuffer,key) == 1)
            {
                printf("error manipulating buffer (addition), cursor at y=%d,x=%d; %d chars",MOUSE_Y,MOUSE_X,NUMCHARS);
                break;
            }
            ++MOUSE_X;
            break;
    }
    move(MOUSE_X,MOUSE_Y);
    return 0;
}
