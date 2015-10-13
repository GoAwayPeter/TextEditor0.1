#ifndef _HANDLEINPUT_H_
    #define _HANDLEINPUT_H_

#define BACKSPACE 8
#define NEWLINE 10
#define TAB 9
#define SPACE 32
#define ESCAPE 27
#define DELETE 127
#define CARRIAGERETURN 13
#define CANCEL 18
#define COLON 58

#include "Buffer.h"

int golhandle_input(BUFFER*,int*);
int handle_input(LBUFFER*,int*);

#endif
