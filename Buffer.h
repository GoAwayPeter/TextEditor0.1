#ifndef _BUFFER_H_
    #define _BUFFER_H_

#include "main.h"

#define INCREASE    0
#define DECREASE    1
#define FORWARD     2
#define BACKWARD    3
#define ABOVE       4
#define BELOW       5
#define UP          6
#define DOWN        7
#define LEFT        8
#define RIGHT       9
#define BEFORE      10
#define AFTER       11

#define MOUSE_Y         currBuffer->cursorPos[1] 
#define MOUSE_X         currBuffer->cursorPos[0] 
#define NUMCHARS        currBuffer->numChars
#define STARTGAP        currBuffer->currNode->startGap
#define ENDGAP          currBuffer->currNode->endGap
#define GAPARRAY        currBuffer->currNode->gapArray
#define GAPARRAYSIZE    currBuffer->currNode->currSize
#define GAPSIZE         currBuffer->currNode->gapSize
#define PREVNODE        currBuffer->currNode->prevNode
#define NEXTNODE        currBuffer->currNode->nextNode

#define INITGAPSIZE 20

/* implement undo queue */

typedef struct gapBufferNode GAP_BUFFER_NODE;

struct gapBufferNode {
    int currSize;
    int gapSize;
    char_u* startGap;
    char_u* endGap;
    char_u* gapArray;
    GAP_BUFFER_NODE* prevNode;
    GAP_BUFFER_NODE* nextNode;
};

typedef struct { 
    int numLines;
    int numChars;
    int currLineNum;
    WINDOW* currwin;
    GAP_BUFFER_NODE* currNode;
}LBUFFER;

int create_gap_node(GAP_BUFFER_NODE*,int); /* pointer to struct, size */
int delete_gap_node(GAP_BUFFER_NODE*);  /* pointer to struct */
int create_lbuffer(LBUFFER*);           /* buf,allocates buffer data */
int insert_line(LBUFFER*,int);          /* buf,ABOVE/BELOW */
int remove_line(LBUFFER*,int);          /* buf,ABOVE/BELOW */
int change_line(LBUFFER*,int,int);      /* buf,UP/DOWN,3rd arg numLines move */
int move_cursor(LBUFFER*,int,int);      /* buf,LEFT/RIGHT, numLines move */
int increase_gap(LBUFFER*);             /* buf,doubles size of gap buffer */
int insert_char(LBUFFER*,uint32_t*);    /* buf,int* char to insert */
int delete_char_before_cur(LBUFFER*);   /* buf,deletes char before cursor */
int delete_char_after_cur(LBUFFER*);    /* buf, deletes char after cursor */
int delete_char_from_to(LBUFFER*,int,int); /* buf,deletes multiple chars */

/* Game Of Life Buffer */

typedef struct { 
    int numRows;
    int numCols;
    int numChars;
    WINDOW* currwin;
    int** bufData;
    int cursorPos[2];
}BUFFER;

BUFFER* golcreate_buffer(BUFFER*,int,int);
int golchange_buffer_size(BUFFER*,int,int,int);
int goladd_char_buffer_at_cursor(BUFFER*,int*);
int goladd_char_buffer(BUFFER*,int*,int,int);
int goldelete_char_buffer_at_cursor(BUFFER*);
int goldelete_char_buffer(BUFFER*,int,int);
int goldelete_buffer_data(BUFFER*);

#endif
