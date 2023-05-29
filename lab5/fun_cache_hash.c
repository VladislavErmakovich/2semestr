#include "fun_cache_hash.h"

int hash_fun(const char *domain, int size) {
    int sum = 0;
    int i = 0;
    while (domain[i] != '\0') {
        sum += domain[i];
        i++;
    }
    return sum % size;
}

void move_to_head(node_hash *node, cache_LRU *cache) {
    if (cache->head == node) {
        return;
    }

    if (cache->tail == node) {
        cache->tail = node->prev;
        cache->tail->next = NULL;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

    node->next = cache->head;
    cache->head->prev = node;
    node->prev = NULL;
    cache->head = node;
}

void add_to_head(node_hash *node, cache_LRU *cache) {
    if (cache->current_size == 0) {
        cache->head = node;
        cache->tail = node;
        node->next = NULL;
        node->prev = NULL;
    } else {
        node->next = cache->head;
        cache->head->prev = node;
        node->prev = NULL;
        cache->head = node;
    }
    cache->current_size++;
}

node_hash *clear_tail(cache_LRU *cache) {
    node_hash *node = cache->tail;

    if (cache->current_size == 1) {
        cache->head = NULL;
        cache->tail = NULL;
    } else {
        cache->tail = node->prev;
        cache->tail->next = NULL;
    }
    cache->current_size--;

    return node;
}

void add_to_hash_table(hash_table *table, node_hash *node) {
    int i = hash_fun(node->domain, table->size);
    node->next_collision = table->mas[i];
    table->mas[i] = node;
}

void clear_from_hash_table(node_hash *node, hash_table *table) {
    int i = hash_fun(node->domain, table->size);
    node_hash *current = table->mas[i];

    if (current == node) {
        free(node->domain);
        free(node->ip);
        table->mas[i] = node->next_collision;
    } else {
        while (current != NULL) {
            if (current->next_collision == node) {
                free(current->domain);
                free(current->ip);
                current->next_collision = node->next_collision;
                break;
            }
            current = current->next_collision;
        }
    }
}

void create_hash_table(int size, hash_table *table) {
    table->size = size;
    table->mas = (node_hash **) malloc(sizeof(node_hash *) * size);
    for (int i = 0; i < size; i++) {
        table->mas[i] = NULL;
    }
}

void delete_hash_table(hash_table *table) {
    for (int i = 0; i < table->size; i++) {
        node_hash *current = table->mas[i];
        while (current != NULL) {
            node_hash *next = current->next_collision;
            free(current);
            current = next;
        }
    }
    free(table->mas);
    table->size = 0;
}

void create_cache_LRU(int size, cache_LRU *cache) {
    cache->size = size;
    cache->current_size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->table = (hash_table *) malloc(sizeof(hash_table));
    create_hash_table(size, cache->table);
}

void delete_cache_LRU(cache_LRU *cache) {
    delete_hash_table(cache->table);
    free(cache->table);

    cache->head = NULL;
    cache->tail = NULL;
    cache->current_size = 0;
    cache->size = 0;
}

node_hash *get_value_from_hash_table(hash_table *table, char *domain) {
    int i = hash_fun(domain, table->size);
    node_hash *current = table->mas[i];

    while (current != NULL) {
        if (strcmp(current->domain, domain) == 0) {
            return current;
        }
        current=current->next_collision;
    }
    return NULL;
}

char *get_domain(char *domain, cache_LRU *cache) {
    node_hash *node = get_value_from_hash_table(cache->table, domain);
    if (node == NULL) {
        return NULL;
    }
    move_to_head(node, cache);
    return node->domain;
}

void put_domain(char *domain, char *ip, cache_LRU *cache) {
    node_hash *node = get_value_from_hash_table(cache->table, domain);
    if (node != NULL) {
        node->ip = ip;
        move_to_head(node, cache);
        return;
    }

    node_hash *new_node = (node_hash *) malloc(sizeof(node_hash));
    new_node->domain = malloc(strlen(domain) + 1);
    strcpy(new_node->domain, domain);
    new_node->ip = malloc(strlen(ip) + 1);
    strcpy(new_node->ip, ip);

    add_to_head(new_node, cache);
    add_to_hash_table(cache->table, new_node);

    if (cache->current_size > cache->size) {
        node_hash *node_last = clear_tail(cache);
        clear_from_hash_table(node_last, cache->table);
        free(node_last);
    }
}
