#include "Redraw.h"

void redraw(LBUFFER* currBuffer)
{
    werase(currBuffer->currwin);
    /* do stuff plz */
}


void golredraw(BUFFER* currBuffer)
{   int i;
    int j;

    werase(currBuffer->currwin);

    for(i = 0;i<currBuffer->numRows;i++)
    {
        if(i > 0)
            printw("\n");

        for(j = 0;j<currBuffer->numCols;j++)
        {
            printw("%c",currBuffer->bufData[i][j]);
        }
    }
    refresh();
}
