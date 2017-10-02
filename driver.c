#include <stdio.h>
#include <unistd.h> // notice this! you need it!
#include "graphics.h"


int main(int argc, char **argv)
{
    init_graphics();

	void *buf = new_offscreen_buffer();

    draw_line(buf, 50, 50, 60, 400, RGB(0, 5, 15));

    int i = 0;  
    while(i < 100000){
        char k = '\0';
        k = getkey();
        if(k != '\0')
            printf("%c\n", k);
        
        draw_pixel(buf, i%100, i%100, 0xF800);
        i++;
    }


    exit_graphics();    
}