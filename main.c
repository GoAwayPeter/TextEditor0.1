#include "Redraw.h"
#include "HandleInput.h"

int main_Loop();

int main(int argc, char** argv)
{
    char* locale;
    locale = setlocale(LC_ALL,"");

    initscr();
    cbreak();
    keypad(stdscr,TRUE);
    scrollok(stdscr,TRUE);
    noecho();
    if(has_colors() == FALSE)
    {
        endwin();
        printf("Terminal lacks color support.\n");
    }
    else
        start_color();


    LBUFFER *buffer1;
    create_lbuffer(buffer1);

    main_Loop(&buffer1);
    endwin();

    return 0;
}

int main_Loop(LBUFFER* currBuffer)
{   
    wchar_t ch = 0;
    int running = 1;

/*  Initialise Game of Life Buffer 
 * 
 *
 *  BUFFER buffer2;
 *  buffer1.currwin = stdscr;
 *  getmaxyx(buffer2.currwin,buffer2.numRows,buffer2.numCols);
 *  create_buffer(&buffer2,2,buffer2.numCols);
 *
 */
    while(running)
    {
        ch = getch();
        if(ch == 27)
            break;
    }
    return 0;
}
