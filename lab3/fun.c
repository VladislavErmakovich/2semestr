#include "fun.h"

int menu(){
    system("cls");
    int choose=0;
    printf("\t\t\t\3BMP-redactor\3\n\t\t1.Negative\n\t\t2.Black-and-white\n\t\t3.Gamma correction\n\t\t4.Median correction\n\t\t5.Exit\n\t\t");
    while (scanf_s("%d", &choose) != 1 || choose < 1 || choose > 5 || getchar() != '\n')
    {
       printf("Error...\n");
        rewind(stdin);
    }
    system("cls");

    return choose;
}

void negative(char* picture_file) {

    BMPHeader header;
    BMPInfoHeader info;
    BMPColor pixel;

    FILE* fp = fopen(picture_file, "rb");
    if(fp == NULL) {
        printf("Error...\n");
        exit(1);
    }
    fread(&header, sizeof(header), 1, fp);
    FILE* new_file = fopen("negative.bmp","wb");

    if(header.type != 0x4D42) {

        printf("Error...\n");
        fclose(fp);
        fclose(new_file);
        exit(1);

    }


    fread(&info, sizeof(info), 1, fp);
    fwrite(&header, sizeof(header), 1, new_file);
    fwrite(&info, sizeof(info), 1, new_file);

    for (int y = 0; y < info.height; y++) {

        for (int x = 0; x < info.width; x++) {

            fread(&pixel, sizeof(pixel), 1, fp);

            pixel.red = 255 - pixel.red;
            pixel.green = 255 - pixel.green;
            pixel.blue = 255 - pixel.blue;

            fwrite(&pixel, sizeof(pixel), 1, new_file);

        }

    }

    fclose(fp);
    fclose(new_file);

    printf("\n\tNegative\n\tDone\3\n");
    getch();
    system("cls");

}

void black_and_white(char* picture_file) {

    BMPHeader header;
    BMPInfoHeader info;
    BMPColor pixel;

    FILE* fp = fopen(picture_file, "rb");
    if(fp == NULL) {
        printf("Error...\n");
        exit(1);
    }
    fread(&header, sizeof(header), 1, fp);
    FILE* new_file = fopen("bw.bmp", "wb");

    if(header.type != 0x4D42) {

        printf("Error...\n");
        fclose(fp);
        fclose(new_file);
        exit(1);

    }


    fread(&info, sizeof(info), 1, fp);
    fwrite(&header, sizeof(header), 1, new_file);
    fwrite(&info, sizeof(info), 1, new_file);

    for (int y = 0; y < info.height; y++) {

        for (int x = 0; x < info.width; x++) {

            fread(&pixel, sizeof(pixel), 1, fp);

            unsigned char gray = (unsigned char)((pixel.red + pixel.green + pixel.blue)/3);//среднее значеніе
            pixel.red = gray;
            pixel.green = gray;
            pixel.blue = gray;

            fwrite(&pixel, sizeof(pixel), 1, new_file);

        }

    }

    fclose(fp);
    fclose(new_file);

    printf("\n\tBlack-and-white\n\tDone\3\n");
    getch();
    system("cls");
}

void gamma_correction(char* picture_file)
{
    BMPHeader header;
    BMPInfoHeader info;

    FILE* fp = fopen(picture_file, "rb");
    if(fp == NULL) {
        printf("Error...\n");
        exit(1);
    }
    fread(&header, sizeof(header), 1, fp);
    FILE* new_file = fopen("gc  .bmp", "wb");

    if(header.type != 0x4D42) {

        printf("Error...\n");
        fclose(fp);
        fclose(new_file);
        exit(1);

    }

    //fread(&header, sizeof(header), 1, fp);
    fread(&info, sizeof(info), 1, fp);
    fwrite(&header, sizeof(header), 1, new_file);
    fwrite(&info, sizeof(info), 1, new_file);

    float gamma;
    printf("Gamma-correction index:\n");
    while (scanf_s("%fl", &gamma) != 1.0 || getchar() != '\n')
    {
        printf("Error...\n");
        rewind(stdin);
    }
    system("cls");

    int padding = (4 - (info.width*3) % 4) % 4;
    int pixel_size = 3;
    unsigned char* row = malloc((info.width*pixel_size + padding) * sizeof(unsigned char));

    for (int y = 0; y < info.height; ++y) {

        fread(row, info.width*pixel_size, 1, fp);

        for (int x = 0; x < info.width; ++x) {

            int red = row[x*pixel_size + 2];//извлечение в обратном порядке, т к такова структура BMP файла (BGR)
            int green = row[x*pixel_size + 1];
            int blue = row[x*pixel_size];

            double new_red = pow(red/255.0, gamma)*255.0;
            double new_green = pow(green/255.0, gamma)*255.0;
            double new_blue = pow(blue/255.0, gamma)*255.0;

            row[x*pixel_size] = (unsigned char)new_blue;
            row[x*pixel_size + 1] = (unsigned char)new_green;
            row[x*pixel_size + 2] = (unsigned char)new_red;

        }

        fwrite(row, info.width*pixel_size, 1, new_file);
        fwrite(&row[info.width*pixel_size], padding, 1, new_file);

    }

    free(row);

    fclose(fp);
    fclose(new_file);
    printf("\n\tGamma-correction\n\tDone\3\n");
    getch();
    system("cls");

}

BMPColor get_pixel(const BMPColor *data, int width, int height, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        BMPColor pixel = {0, 0, 0};
        return pixel;
    }
    return data[y * width + x];
}

int compare(const void* first, const void* second)
{
    BMPColor* first_color=(BMPColor*)first;
    BMPColor* second_color=(BMPColor*)second;
    int brightness_first=(int)first_color->red+(int)first_color->green+(int)first_color->blue;
    int brightness_second=(int)second_color->red+(int)second_color->green+(int)second_color->blue;
    return brightness_first-brightness_second;
}

void median_correction(char* picture_file) {
    BMPHeader header;
    BMPInfoHeader info;

    FILE *fp = fopen(picture_file, "rb");
    if (fp == NULL) {
        printf("Error...\n");
        exit(1);
    }

    fread(&header, sizeof(header), 1, fp);
    fread(&info, sizeof(info), 1, fp);

    BMPColor* picture_color_data = (BMPColor *) malloc(info.width*info.height*sizeof(BMPColor));
    fread(picture_color_data, sizeof(BMPColor), info.height * info.width, fp);

    fclose(fp);

    BMPColor *new_file_color_data = (BMPColor *) malloc(info.width * info.height * sizeof(BMPColor));
    int kernel_size = 0;
    printf("\n\tKernel size(pls input number-odd) ");
    while (scanf_s("%d", &kernel_size) != 1 || kernel_size < 1 || kernel_size > 11 || kernel_size % 2 == 0 ||getchar() != '\n') {
        printf("\n\tError...\nare you stupid?\n");
        rewind(stdin);
    }
    int offset = kernel_size / 2;//определение границ области вокруг каждого пикселя

    BMPColor *kernel = malloc(kernel_size * kernel_size* sizeof(BMPColor));

    for (int y = 0; y < info.height; ++y) {

        for (int x = 0; x < info.width; ++x) {

            int index = 0;

            for (int ky = -offset; ky <= offset ; ++ky) {

                for (int kx = -offset; kx <= offset; ++kx) {

                    BMPColor pixel = get_pixel(picture_color_data, info.width, info.height, x + kx, y + ky);
                    kernel[index] = pixel;
                    index++;

                }

            }
            qsort(kernel, kernel_size * kernel_size, sizeof(BMPColor), compare);
            BMPColor median_pixel=kernel[kernel_size*kernel_size / 2];
            new_file_color_data[y*info.width+x]=median_pixel;
        }
    }
    free(kernel);

    FILE *new_file = fopen("median.bmp", "wb");

    if(new_file==NULL)
    {
        printf("ERORR\n");
        free(picture_color_data);
        free(new_file_color_data);
        exit(1);
    }
    if (header.type != 0x4D42) {

        printf("Error...\n");
        fclose(fp);
        fclose(new_file);
        exit(1);

    }

    fwrite(&header, sizeof(header), 1, new_file);
    fwrite(&info, sizeof(info), 1, new_file);

    fwrite(new_file_color_data,sizeof (BMPColor),info.width*info.height,new_file);
    fclose(new_file);

    printf("\n\tMedian correction\n\tDone\3\n");
    getch();
    system("cls");

}
