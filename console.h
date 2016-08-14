#ifndef CONSOLE_H
#define CONSOLE_H

#include "windows.h"
#include <iostream>

class Console
{
public:
    Console();
    void gotoxy(short ix, short iy); // set cursor in appropiate position
    void clean_screen(int in);  // clean determined number of lines

};

#endif // CONSOLE_H
