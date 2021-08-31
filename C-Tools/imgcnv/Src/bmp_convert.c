#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
//#include "ImageUploader.h"


#define MAX_BMP_SIZE    131072
#define DATA_OFFSET_OFFSET 0x000A
#define WIDTH_OFFSET 0x0012
#define HEIGHT_OFFSET 0x0016
#define BITS_PER_PIXEL_OFFSET 0x001C
#define HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

#define BUF_SIZE    131072
unsigned short  bmpfile[BUF_SIZE],image[BUF_SIZE];
unsigned char   ascii_image[BUF_SIZE*4];
int             bmp_len;
FILE            *fpin,*fpout;


unsigned short swap_byte(unsigned short in)
{
    return (in << 8) | (in >> 8);
}

int read_and_convert(char *infile)
{
int         i,j,k,dataOffset,width,height,pixel_index;
unsigned short  bitsPerPixel;

    printf( "On %s\n",infile ) ;
    fpin = fopen (infile, "r");
    if ( fpin == NULL )
    {
        printf( "Unable to open %s for read\n",infile ) ;
        return -1;
    }

    fseek(fpin, BITS_PER_PIXEL_OFFSET, SEEK_SET);
    fread(&bitsPerPixel, 2, 1, fpin);
    if ( bitsPerPixel != 16 )
    {
        printf("Required 16 bpp image, found %d image\n",bitsPerPixel) ;
        fclose(fpin);
    }

    fseek(fpin, WIDTH_OFFSET, SEEK_SET);
    fread(&width, 4, 1, fpin);
    fseek(fpin, HEIGHT_OFFSET, SEEK_SET);
    fread(&height, 4, 1, fpin);

    fseek(fpin, DATA_OFFSET_OFFSET, SEEK_SET);
    fread(&dataOffset, 4, 1, fpin);
    fseek(fpin, dataOffset, SEEK_SET);
    bmp_len = fread(bmpfile, 2, width*height, fpin);
    printf( "Found %d %d bit pixels ( %d bytes )\n",bmp_len,bitsPerPixel,bmp_len*2) ;
    printf( "Found %d x %d image\n",width,height) ;
    fclose(fpin);

    pixel_index=0;
    for(i=height-1;i>=0;i--)
    {
        k = (i-1)*width;
        for(j=0;j<width;j++)
        {
            image[pixel_index] = swap_byte(bmpfile[k+j]);
            pixel_index++;
        }
    }
    //printf( "bmp_len*2 %d \n",bmp_len) ;
    return bmp_len;
}

void save_image(char *outfile,int len)
{
int i,k=0;
char fname[256];
    sprintf(fname,"../../Core/Src/bB101_VCO/c_images/%s.c",outfile);
    printf("Writing on %s\n",fname);
    fpout = fopen (fname, "w");
    fprintf(fpout,"#include \"main.h\"\n");
    fprintf(fpout,"__attribute__((section(\".table\"))) const uint16_t %s[%d] = \n",outfile,len);
    fprintf(fpout,"{\n");
    //printf("%d bytes\n",len*2);
    //printf("{\n");
    for(i=0;i<len;i++, k++)
    {
        if ( k == 16)
        {
            k=0;
            fprintf(fpout,"\n");
        }
        fprintf(fpout,"0x%04x,",image[i]);
        //printf("\n");
    }
    fprintf(fpout,"\n};\n");
    //printf("};\n");
    fclose(fpout);

}

