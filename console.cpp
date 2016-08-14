#include "console.h"

using namespace std;

Console::Console()
{

}

void Console::gotoxy(short ix, short iy)
{
    static HANDLE h = NULL;
      if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD c = { ix, iy };
      SetConsoleCursorPosition(h,c);

//    printf("%c[%d;%df", 0x1B, iy, ix);

}

void Console::clean_screen(int in)
{
    for(int i=0; i<in; i++)
        cout << "                                                        " << endl;
}
