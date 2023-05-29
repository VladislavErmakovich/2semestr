#include "string.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct node_hash {
    char *domain;
    char *ip;
    struct node_hash *next;
    struct node_hash *prev;
    struct node_hash *next_collision;
    struct node_hash *prev_collision;
} node_hash;

typedef struct hash_table {
    int size;
    node_hash **mas;
} hash_table;

typedef struct cache_LRU {
    hash_table *table;
    node_hash *head;
    node_hash *tail;
    int size;
    int current_size;
} cache_LRU;


int hash_fun(const char *domain, int size);

node_hash *get_value_from_hash_table(hash_table *table, char *domain);

void add_to_hash_table(hash_table *table, node_hash *node);

void add_to_head(node_hash *node, cache_LRU *cache);

void clear_from_hash_table(node_hash *node, hash_table *table);

void create_hash_table(int size, hash_table *table);

void delete_hash_table(hash_table *table);

char *get_domain(char *domain, cache_LRU *cache);

void put_domain(char *domain, char *ip, cache_LRU *cache);

void move_to_head(node_hash *node, cache_LRU *cache);

node_hash *clear_tail(cache_LRU *cache);

void create_cache_LRU(int size, cache_LRU *cache);

void delete_cache_LRU(cache_LRU *cache);