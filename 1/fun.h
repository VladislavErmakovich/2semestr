#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 255
#define STR_PAR1 " IN CNAME "
#define STR_PAR2 " IN A "

typedef struct HASH_node {
    char *key;
    char *value;
    struct HASH_node *next;
    struct HASH_node *prev;
    struct HASH_node *col_next;
    struct HASH_node *col_prev;
} HASH_node;

typedef struct HASH_table {
    HASH_node **table_array;
    int capacity;
} HASH_table;

typedef struct LRU_cache {
    HASH_table *table;
    HASH_node *head;
    HASH_node *tail;
    int capacity;
    int size;
} LRU_cache;

int hash(const char *key, int capacity);

void move_to_head(LRU_cache *cache, HASH_node *node);

void add_to_head(LRU_cache *cache, HASH_node *node);

HASH_node *remove_tail(LRU_cache *cache);

void add_to_hash_table(HASH_table *table, HASH_node *node);

void remove_from_hash_table(HASH_table *table, HASH_node *node);

void init_hash_table(HASH_table *table, int capacity);

void delete_hash_table(HASH_table *table);

void init_LRU_cache(LRU_cache *cache, int capacity);

void delete_LRU_cache(LRU_cache *cache);

HASH_node *get_from_hash_table(HASH_table *table, char *key);

char *get(LRU_cache *cache, char *key);

void put(LRU_cache *cache, char *key, char *value);



void clear_stdin();

int is_ip(char *ip);

char *get_value(char *buffer);

char *file_search(FILE *f_dns, char *key);

int init_x(int min, int max);

char *domain_search(LRU_cache *cache, char *key, FILE *f_dns);

void add_new_dns(char *domain, char *ip_str, FILE *f_dns);

char *output_by_ip(FILE *f_dns, char *ip_str);

void output_by_id(FILE *f_dns, char *domain_str);

void show_cache(HASH_node *tail);

void menu(LRU_cache *cache, FILE *f_dns);