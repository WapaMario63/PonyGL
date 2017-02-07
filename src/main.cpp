#include <stdio.h>
#include "PonyGL.h"

int main()
{
    printf("************** PonyGL 2 v0.0.1a ***************\n");
    printf("********** PoneNgine Graphics Engine **********\n");
    printf("***** (c) The Wapa Castle Team 2015-2017 ******\n");
    printf("\n");
    printf("* This software is licensed under the MIT license.\n");
    printf("* Please check LICENCE.txt for more information.\n");
    printf("\n");
    printf("Initializing engine...\n");
    PonyGL* win = new PonyGL();
    win->run();

    return 0;
}
