#include "Buffer.h"
#include "UTF_8.h"

/* Normal Buffer Functions */


int create_gap_node(GAP_BUFFER_NODE* gapNode,int size)
{
    GAP_BUFFER_NODE newNode;
    gapNode = &newNode;
    gapNode->gapArray = malloc(size*sizeof(char_u));
    if(gapNode->gapArray == NULL)
    {
        printf("error allocating gapbuffer");
        return -1;
    }
    gapNode->startGap = &gapNode->gapArray[0];
    gapNode->endGap = &gapNode->gapArray[size - 1];
    gapNode->currSize = size;
    gapNode->prevNode = NULL;
    gapNode->nextNode = NULL;
    gapNode->gapSize = size;

    return 0;
}

int delete_gap_node(GAP_BUFFER_NODE* gapNode)
{
    free(gapNode->gapArray);
    return 0;
}

int create_lbuffer(LBUFFER* currBuffer)
{
    LBUFFER newBuffer;
    currBuffer = &newBuffer;
    if(create_gap_node(currBuffer->currNode,INITGAPSIZE) == -1)
        return -1;

    currBuffer->currwin = stdscr;
    currBuffer->numChars = 0;
    currBuffer->numLines = 1;
    currBuffer->currLineNum = 0;
    
    return 0;
}

int insert_line(LBUFFER* currBuffer,int aboveBelow)
{
    GAP_BUFFER_NODE* newGapNode;
    if(create_gap_node(newGapNode,16) == -1)
    {
        printf("error creating gap node");
        return 1;
    }

    if(aboveBelow == ABOVE)
    {
        if(PREVNODE == NULL)
        {
            PREVNODE = newGapNode;
            newGapNode->nextNode = currBuffer->currNode;
        }
        else
        {
            PREVNODE->nextNode = newGapNode;
            newGapNode->prevNode = PREVNODE;
            PREVNODE = newGapNode;
            newGapNode->nextNode = currBuffer->currNode;
        }
    }
    else if(aboveBelow == BELOW)
    {
        if(NEXTNODE == NULL)
        {
            NEXTNODE = newGapNode;
            newGapNode->prevNode = currBuffer->currNode;
        }
        else
        {
            NEXTNODE->prevNode = newGapNode;
            newGapNode->nextNode = NEXTNODE;
            NEXTNODE = newGapNode;
            newGapNode->prevNode = currBuffer->currNode;
        }
    }
    else
    {
        printf("Invalid insert line argument");
        return 1;
    }
    ++currBuffer->numLines;
    return 0;
}

int remove_line(LBUFFER* currBuffer,int aboveBelow)
{
    /* need to implement undo stack first */
     
    return 0;
}

int change_line(LBUFFER* currBuffer,int upDown,int moveAmount)
{   int i;

    for(i = 0;i<moveAmount;i++)
    {
        if(upDown == UP && currBuffer->currNode->prevNode != NULL)
        {
            currBuffer->currNode = currBuffer->currNode->prevNode;
            --currBuffer->currLineNum;
        }
        else if(upDown == DOWN && currBuffer->currNode->nextNode != NULL)
        {
            currBuffer->currNode = currBuffer->currNode->nextNode;
            ++currBuffer->currLineNum;
        }
    }
    return 0;
}

/*
 * NEED TO IMPLEMENT MULTI BYTE CHARS FIRST
 */

int move_cursor(LBUFFER* currBuffer,int leftRight,int moveAmount)
{
    if((leftRight == LEFT &&
        STARTGAP[0] == GAPARRAY[0]) ||
       (leftRight == RIGHT &&
        ENDGAP[0] == GAPARRAY[GAPARRAYSIZE]))
        return 0;

    if( STARTGAP[0] < 0xC0 && 
        STARTGAP[0] >= 0x80) 
    {
        int i;
        for(i = 0; i<4;i++)
        {
            if( STARTGAP[i] >= 0xC0 || 
                STARTGAP[i] < 0x80) 
            {
                STARTGAP = &STARTGAP[i];
            }
        }
    }
    

    if(leftRight == LEFT)
    {
        int numBytes = 0;
        char_u *char_before_gap = NULL;
        char_before_gap = find_prev_mb_char(STARTGAP);
        numBytes = sizeof_mb_char(char_before_gap);

        if(numBytes == 0)
        {
            printf("error determining size of mbchar");
            return -1;
        }

        int i;
        for(i = 0;i < numBytes;i++)
        {
            ENDGAP[i - numBytes] = STARTGAP[i - numBytes];
        }
        ENDGAP = &ENDGAP[-numBytes];
        STARTGAP = &STARTGAP[-numBytes];
    }
    else if(leftRight == RIGHT)
    {
        int numBytes = 0;
        numBytes = sizeof_mb_char(ENDGAP);

        int i;
        for(i = 0;i < numBytes;i++)
        {
            STARTGAP[i] = ENDGAP[i];
        }
        
        ENDGAP = &ENDGAP[numBytes];
        STARTGAP = &STARTGAP[numBytes];
    }

    return 0;
}

int increase_gap(LBUFFER* currBuffer)
{
    GAP_BUFFER_NODE *newNode; 
    if(create_gap_node(newNode,GAPARRAYSIZE*2) == -1)
        return -1;

    int i;
    int endGapPos;
    for(i = 0; i<GAPARRAYSIZE; i++)
    {
        if(&GAPARRAY[i] == STARTGAP)
        {
            endGapPos = GAPARRAYSIZE - i;
            i++;
            break;
        }
        newNode->gapArray[i] = GAPARRAY[i];
    }

    int j;
    for(j = 0; j<endGapPos; j++)
    {
        newNode->gapArray[GAPARRAYSIZE*2 - i] = GAPARRAY[i + j];
    }

    newNode->startGap = &(newNode->gapArray[i]);
    newNode->endGap = &(newNode->gapArray[GAPARRAYSIZE*2 - endGapPos]);

    newNode->prevNode = PREVNODE;
    if(PREVNODE != NULL)
        PREVNODE->nextNode = newNode;

    newNode->nextNode = NEXTNODE;
    if(NEXTNODE != NULL)
        NEXTNODE->prevNode = newNode;

    newNode->gapSize = GAPARRAYSIZE;
    newNode->currSize = GAPARRAYSIZE*2;

    delete_gap_node(currBuffer->currNode);
    currBuffer->currNode = newNode;

    return 0;
}

int insert_char(LBUFFER* currBuffer,uint32_t* key)
{
    if(STARTGAP == ENDGAP)
        increase_gap(currBuffer);
    
    switch(wchar_to_mbchar(key,STARTGAP))
    {
        case 1:
            STARTGAP = &STARTGAP[1];
            return 0;
        case 2:
            STARTGAP = &STARTGAP[2];
            return 0;
        case 3:
            STARTGAP = &STARTGAP[3];
            return 0;
        case 4:
            STARTGAP = &STARTGAP[4];
            return 0;
        case -1:
            printf("error inserting char");
            return -1;
    }

    return 0;
}

int delete_char_before_cur(LBUFFER* currBuffer)
{
    switch(sizeof_mb_char(find_prev_mb_char(STARTGAP)))
    {
        case 1:
            STARTGAP = &STARTGAP[-1];
            break;
        case 2:
            STARTGAP = &STARTGAP[-2];
            break;
        case 3:
            STARTGAP = &STARTGAP[-3];
            break;
        case 4:
            STARTGAP = &STARTGAP[-4];
            break;
        case -1:
            printf("error could not find char to delete");
            return -1;
    }
    return 0;
}

int delete_char_after_cur(LBUFFER* currBuffer)
{
    switch(sizeof_mb_char(ENDGAP))
    {
        case 1:
            ENDGAP = &ENDGAP[1];
            break;
        case 2:
            ENDGAP = &ENDGAP[2];
            break;
        case 3:
            ENDGAP = &ENDGAP[3];
            break;
        case 4:
            ENDGAP = &ENDGAP[4];
            break;
        case -1:
            printf("error could not find char to delete");
            return -1;
    }
    return 0;
}

int delete_char_from_to(LBUFFER* currBuffer,int beforeAfterCur,int num)
{
    int i;
    if(beforeAfterCur == BEFORE)
    {
        for(i = 0; i < num; i++)
        {
            if(STARTGAP == GAPARRAY)
            {
                printf("warning, deleting past array bounds");
                return 0;
            }
            if(delete_char_before_cur(currBuffer) == -1)
            {
                printf("error deleting char 1");
                return -1;
            }
        }
    }
    else if(beforeAfterCur == AFTER)
    {
        for(i = 0; i < num; i++)
        {
            if(ENDGAP == &GAPARRAY[GAPARRAYSIZE])
            {
                printf("warning, deleting past array bounds");
                return 0;
            }
            if(delete_char_after_cur(currBuffer) == -1)
            {
                printf("error deleting char 2");
                return -1;
            }
        }
    }
    
    return 0;
}

/*  
 *  
 * Game of Life Buffer Functions
 *
 */


BUFFER* golcreate_buffer(BUFFER* currBuffer,int rows,int cols)
{   int i;
    
    currBuffer->numRows = rows;
    currBuffer->numCols = cols;

    currBuffer->bufData = malloc(rows*sizeof(int*));

    for(i = 0;i<rows;i++)
        currBuffer->bufData[i] = malloc(cols*sizeof(int));
    
    MOUSE_Y = 0;
    MOUSE_X = 0;

    return currBuffer;
}

int golchange_buffer_size(BUFFER* currBuffer,int incrDecr,int rowChange,int colChange)
{   int i; /* Iterators */
    int j;

    int** temp;
    int newRows; /* New number of rows and cols */
    int newCols;

    if(incrDecr == INCREASE)
    {
        newRows = currBuffer->numRows + rowChange;
        temp = malloc(newRows*sizeof(int*));
        if(temp == NULL)
            return 1;

        newCols = currBuffer->numCols + colChange;
        for(i = 0; i < newRows; i++)
        {
            temp[i] = malloc(newCols*sizeof(int));
            if(temp[i] == NULL)
                return 1;
        }
    }
    else if(incrDecr == DECREASE)
    {
        newRows = currBuffer->numRows - rowChange;
        temp = malloc(newRows*sizeof(int*));
        if(temp == NULL)
            return 1;

        for(i = 0; i < newRows; i++)
        {
            temp[i] = malloc(newCols*sizeof(int));
            if(temp[i] == NULL)
                return 1;
        }
    }

    /* Copy data to new buffer */

    for(i = 0;i < currBuffer->numRows;i++ )
    {
        for(j = 0;j < currBuffer->numCols;j++ )
        {
            if(    i >= 0
                && i <= newRows
                && j >= 0
                && j <= newCols)
            {
                temp[i][j] = currBuffer->bufData[i][j];
            }
            else
                printf("Warning, buffer truncation occurred\n");
        }
    }
    
    if(goldelete_buffer_data(currBuffer) == 1) /* Only deletes bufData */
        printf("warning, buffer data not freed");

    currBuffer->bufData = temp;
    currBuffer->numRows = newRows;
    currBuffer->numCols = newCols;

    return 0;
}

int goldelete_buffer_data(BUFFER* currBuffer)
{   int i;


    for(i = 0;i<currBuffer->numRows;i++)
    {
        if(currBuffer->bufData[i] == NULL)
            return 1;
        free(currBuffer->bufData[i]);
    }

    if(currBuffer->bufData == NULL)
        return 1;
    free(currBuffer->bufData);
    return 0;
}

int goladd_char_buffer(BUFFER* currBuffer,int* ch,int row,int col)
{
    if(    row < 0 
        || col < 0 
        || row > currBuffer->numRows
        || col > currBuffer->numCols)
    {
        return 1;
    }
    else
    {
        currBuffer->bufData[row][col] = *ch;
        ++NUMCHARS;
    }
    return 0;

}

int goladd_char_buffer_at_cursor(BUFFER* currBuffer, int* ch)
{
    if(    MOUSE_Y < 0
        || MOUSE_X < 0
        || MOUSE_X > currBuffer->numCols
        || MOUSE_Y > currBuffer->numRows)
    {
        return 1;
    }
    else
    {
        currBuffer->bufData[MOUSE_Y][MOUSE_X] = *ch;
        ++NUMCHARS;
    }
    return 0;
}

int goldelete_char_buffer(BUFFER* currBuffer,int row,int col)
{
    if(    row < 0 
        || col < 0 
        || row > currBuffer->numRows
        || col > currBuffer->numCols)
    {
        return 1;
    }
    else
    {
        currBuffer->bufData[row][col] = 0;
        --NUMCHARS;
    }
    return 0;

}

int goldelete_char_buffer_at_cursor(BUFFER* currBuffer)
{
    if(    MOUSE_Y < 0
        || MOUSE_X < 0
        || MOUSE_X > currBuffer->numCols - 1
        || MOUSE_Y > currBuffer->numRows - 1)
    {
        return 1;
    }
    else
    {
        currBuffer->bufData[MOUSE_Y][MOUSE_X - 1] = 0;
        --NUMCHARS;
    }
    return 0;
}
