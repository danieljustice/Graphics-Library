#include "library.c"
#include <stdio.h>
#include <unistd.h> // notice this! you need it!

int main(int argc, char **argv)
{
    init_graphics();
    sleep(5);
    exit_graphics();    
}