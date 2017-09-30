#include <fcntl.h>          /* for open() syscall ref:http://pubs.opengroup.org/onlinepubs/009695399/functions/open.html*/
#include <stropts.h>        /* for ioctl() syscall ref: http://pubs.opengroup.org/onlinepubs/009695399/functions/ioctl.html*/
#include <sys/mman.h>       /* for mmap() syscall ref:http://pubs.opengroup.org/onlinepubs/009695399/functions/mmap.html*/
#include <linux/fb.h>       /* good example  of ioctl http://www.ummon.eu/Linux/API/Devices/framebuffer.html*/

//for mmap in init
int file;
int *address;
int len;

//for ioctl
struct fb_var_screeninfo variable_info;
struct fb_fix_screeninfo fixed_info;
int yres;
int line_length;


void init_graphics(){
    file = open("/dev/fb0", O_RDWR);
    
    //populate variable_info struc
    ioctl(file, FBIOGET_VSCREENINFO, &variable_info);
    //populate fixed_info struc
    ioctl(file, FBIOGET_FSCREENINFO, &fixed_info);
    //size of the mmapped file
    len = variable_info.yres_virtual * fixed_info.line_length;
    address = mmap(0, len, PROT_WRITE, MAP_PRIVATE, file, 0);
    //clears the screen
    print("\033[2J");
    

}