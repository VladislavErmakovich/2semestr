#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>



typedef struct FIFO{
    char* word;
    char* switch_word;
    int flag;
    int switch_flag;
    struct FIFO *next;
}FIFO;

typedef struct string{
    char** word;
    int len;
    int* flag;
}string;

void decompressor();