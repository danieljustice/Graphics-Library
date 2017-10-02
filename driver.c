#include "library.c"
#include <stdio.h>
#include <unistd.h> // notice this! you need it!

int main(int argc, char **argv)
{
    init_graphics();
    int i = 0;  
    while(i < 100000){
        char k = '\0';
        k = getkey();
        if(k != '\0')
            printf("%c\n", k);
        i++;
    }
    exit_graphics();    
}