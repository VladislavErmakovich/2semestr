#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <locale.h>

#define _CRT_SECURE_NO_WARNINGS

typedef struct FIFO{
  char* word;
  char* switch_word;
  int flag;
  long len;
  int repeat;
  struct FIFO *next;
}FIFO;

typedef struct string{
    char** word;
    int len;
}string;

void compressor(char file[]);
