#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
//#include "ImageUploader.h"

extern  unsigned short  image[];
extern  unsigned char   ascii_image[];
extern  int read_and_convert(char *infile);
extern  void save_image(char *outfile,int len);

void usage(void)
{
    printf("ImageUploader -f <images_path> : upload all images\n");
    printf("ImageUploader -E               : erases ALL flash!!\n");
}

int main(int argc, char **argv)
{
int len;
    len = read_and_convert("Logo1.bmp");
    save_image("logo",len);
    len = read_and_convert("noise.bmp");
    save_image("noise",len);
    len = read_and_convert("sqrw.bmp");
    save_image("sqrw",len);
    len = read_and_convert("tria.bmp");
    save_image("tria",len);
    len = read_and_convert("sine.bmp");
    save_image("sine",len);
    return 0;
}
