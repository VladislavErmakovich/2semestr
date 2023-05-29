#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>


#pragma pack(push,1)

typedef struct {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

typedef struct {
    unsigned int size;
    int width;
    int height;
    unsigned short planes;
    unsigned short bits_per_pixels;
    unsigned int compression;
    unsigned int image_size;
    int X_pixels_per_meter;
    int Y_pixels_per_meter;
    unsigned int colors_uses;
    unsigned int colors_important;

} BMPInfoHeader;

typedef struct {
    unsigned char blue, green, red;
} BMPColor;//RGB(BGR)

#pragma pack(pop)

int menu();
void negative(char* picture_file);
void black_and_white(char* picture_file);
void gamma_correction(char* picture_file);
BMPColor get_pixel(const BMPColor *data, int width, int height, int x, int y);
int compare(const void* first, const void* second);
void median_correction(char* picture_file);
