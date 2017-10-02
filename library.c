#include <fcntl.h>          /* for open() syscall ref:http://pubs.opengroup.org/onlinepubs/009695399/functions/open.html*/
#include <stropts.h>        /* for ioctl() syscall ref: http://pubs.opengroup.org/onlinepubs/009695399/functions/ioctl.html*/
#include <sys/mman.h>       /* for mmap() syscall ref:http://pubs.opengroup.org/onlinepubs/009695399/functions/mmap.html*/
#include <linux/fb.h>       /* good example of ioctl http://www.ummon.eu/Linux/API/Devices/framebuffer.html*/
#include <unistd.h>         /* for write() http://man7.org/linux/man-pages/man2/write.2.html*/
#include <termios.h>        /* for keyboard control ref: https://linux.die.net/man/3/termios*/
#include <termio.h>         //need this too for some reason ¯\_(ツ)_/¯
#include <sys/select.h>
#include <sys/types.h>      
#include <time.h>           /* for nanosleep()*/
//function prototypes
void init_graphics();
void clear_screen();
void exit_graphics();
void key_presses(int);
char getkey();
void sleep_ms();
void * new_offscreen_buffer();

//for mmap in init
int file;
char *process_address;
int len;

//for ioctl
struct fb_var_screeninfo variable_info;
struct fb_fix_screeninfo fixed_info;
int yres;
int line_length;

#define CLEAR_CODE "\033[2J" 


void init_graphics(){
    file = open("/dev/fb0", O_RDWR);
    
    //populate variable_info struc
    ioctl(file, FBIOGET_VSCREENINFO, &variable_info);
    //populate fixed_info struc
    ioctl(file, FBIOGET_FSCREENINFO, &fixed_info);
    //size of the mmapped file
    len = variable_info.yres_virtual * fixed_info.line_length;
    process_address = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    //clears the screen
    clear_screen();
    //turns off keypresses
    key_presses(0);

}

//https://en.wikipedia.org/wiki/Select_(Unix)
//https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
//http://man7.org/linux/man-pages/man2/select.2.html

char getkey(){
    char c = '\0';                      //set to empty char ref: https://stackoverflow.com/questions/18410234/how-does-one-represent-the-empty-char
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    struct timeval tv ={0L, 0L};
    int count = select(1, &fds, NULL, NULL, &tv);

    if(count > 0){
            /* If a character was pressed then we get it and exit */
            read(0, &c, sizeof(char)); 
    }

    return c;   
}

//ref: https://stackoverflow.com/questions/7684359/how-to-use-nanosleep-in-c-what-are-tim-tv-sec-and-tim-tv-nsec
void sleep_ms(int milli_secs){
    struct timespec tim;
    tim.tv_sec = 0;                         //dont need whole seconds
    tim.tv_nsec = milli_secs * 1000000;     //nanosecs to milliseconds

    nanosleep(&tim, NULL);

}

void exit_graphics(){
    //Turn Keypresses ON
    key_presses(1);
    
    //try to close file for safety
    close(file);

    //unmap the data
    munmap(process_address, len);
}

void clear_screen(){
    write(1, CLEAR_CODE, sizeof CLEAR_CODE);        //https://en.wikipedia.org/wiki/Write_(system_call)
}

//ref: https://android.googlesource.com/platform/prebuilts/gcc/linux-x86/host/x86_64-linux-glibc2.7-4.6/+/master/sysroot/usr/include/bits/termios.h 
//ref: https://stackoverflow.com/questions/31999358/how-are-flags-represented-in-the-termios-library 
void key_presses(int bool){
    //bool == 0 --> turn off keyboard
    //bool == 1 --> turn on keyboard

    struct termios terminal;
    //gets current terminal settings
    ioctl(1, TCGETS, &terminal);        //1 for std ouput... i think?  ref: https://en.wikipedia.org/wiki/Write_(system_call)

    if(bool == 0){
        terminal.c_lflag &= ~ICANON;   //ands the c_lflags with not ICANON, turns ICANON OFF
        terminal.c_lflag &= ~ECHO;     //ands the c_lflags with not ECHO, turn ECHO OFF
    }else{
        terminal.c_lflag |= ICANON;    //ors the c_lflags with ICANON, turns ICANON ON
        terminal.c_lflag |= ECHO;      //ors the c_lflags with ECHO, turns ECHO ON
    }
    
    //sets terminal settings
    ioctl(1, TCSETS, &terminal);
}

void * new_offscreen_buffer(){
    void *buf = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return buf;
}

//Need to cast everything
void blit(void * buf){
    char *screen_address;
    int i = 0;
    for(i = 0; i<len; i++){
        screen_address = (char *)process_address + i;
        *screen_address = *((char *)(buf+i));
    }
}