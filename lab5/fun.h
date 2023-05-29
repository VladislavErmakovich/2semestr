#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "fun_cache_hash.h"

#define _CRT_SECURE_NO_WARNINGS
#define CACHE_SIZE 5
#define SIZE_STRING 255
#define STR_TYPE_1 " IN A "
#define STR_TYPE_2 " IN CNAME "


int menu();

int check_ip(char *ip);

char *get_ip_mother(char *buffer);

char *search_file(FILE *fp, char *domain);

char *search_domain(cache_LRU *cache, char *domain, FILE *fp);

void add_new_domain(char *domain, char *ip, FILE *fp);

char *output_by_ip(FILE *fp, char *ip);

void output_by_id(FILE *fp, char *str_domain);

void output_cache(node_hash *tail);

void task_1(cache_LRU *cache, FILE *fp);

void task_2(FILE *fp);

void task_3(FILE *fp);

void task_4(cache_LRU *cache);