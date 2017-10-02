#include "library.c"

#define CLEAR_CODE "\033[2J" 
//1111 1000 0000 0000
//F    8    0    0
#define REDMASK(color) ((color << 11) & 0xF800)
//0000 0111 1110 0000
//0    7    E    0
#define GREENMASK(color) ((color << 5) & 0x07E0)
//0000 0000 0001 1111
//0    0    1    F
#define BLUEMASK(color) (color & 0x001F)

#define RGB(R,G,B) (REDMASK(R) | GREENMASK(G) | BLUEMASK(B))

typedef unsigned short color_t;


void init_graphics();
void exit_graphics();
char getkey();
void sleep_ms(long ms);
void clear_screen(void *img);
void draw_pixel(void *img, int x, int y, color_t color);
void draw_line(void *img, int x1, int y1, int x2, int y2, color_t c);
void *new_offscreen_buffer();
void blit(void *src);