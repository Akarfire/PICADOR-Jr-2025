// PICADOR-Jr-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PicadorJrCore.h"

int main()
{
    PicadorJrCore* core = new PicadorJrCore(/* ... */);

    // ... Inserting additional module (if needed) ...

    core->run();

    delete core;
}
