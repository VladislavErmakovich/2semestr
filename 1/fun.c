#include "fun.h"

int hash(const char *key, int capacity) {
    int char_sum = 0;
    int i = 0;
    while (key[i] != '\0') {
        char_sum += key[i];
        i++;
    }
    return char_sum % capacity;
}

void move_to_head(LRU_cache *cache, HASH_node *node) {
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

void add_to_head(LRU_cache *cache, HASH_node *node) {
    if (cache->size == 0) {
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

    cache->size++;
}

HASH_node *remove_tail(LRU_cache *cache) {
    HASH_node *node = cache->tail;

    if (cache->size == 1) {

        cache->head = NULL;
        cache->tail = NULL;
    } else {
        cache->tail = node->prev;
        cache->tail->next = NULL;
    }

    cache->size--;

    return node;
}

void add_to_hash_table(HASH_table *table, HASH_node *node) {
    int index = hash(node->key, table->capacity);

    node->col_next = table->table_array[index];
    table->table_array[index] = node;
}

void remove_from_hash_table(HASH_table *table, HASH_node *node) {
    int index = hash(node->key, table->capacity);
    HASH_node *current = table->table_array[index];

    if (current == node) {
        free(node->key);
        free(node->value);

        table->table_array[index] = node->col_next;
    } else {
        while (current != NULL) {
            if (current->col_next == node) {
                free(node->key);
                free(node->value);

                current->col_next = node->col_next;
                break;
            }
            current = current->col_next;
        }
    }
}

void init_hash_table(HASH_table *table, int capacity) {
    table->capacity = capacity;
    table->table_array = (HASH_node **) malloc(sizeof(HASH_node *) * capacity);

    for (int i = 0; i < capacity; i++) {
        table->table_array[i] = NULL;
    }
}

void delete_hash_table(HASH_table *table) {
    for (int i = 0; i < table->capacity; i++) {
        HASH_node *current = table->table_array[i];

        while (current != NULL) {
            HASH_node *next = current->col_next;

            free(current);

            current = next;
        }
    }

    free(table->table_array);
    table->capacity = 0;
}

void init_LRU_cache(LRU_cache *cache, int capacity) {
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = NULL;
    cache->tail = NULL;
    cache->table = (HASH_table *) malloc(sizeof(HASH_table));

    init_hash_table(cache->table, capacity);
}

void delete_LRU_cache(LRU_cache *cache) {
    delete_hash_table(cache->table);
    free(cache->table);

    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
    cache->capacity = 0;
}

HASH_node *get_from_hash_table(HASH_table *table, char *key) {
    int index = hash(key, table->capacity);
    HASH_node *current = table->table_array[index];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0)
            return current;
        current = current->col_next;
    }

    return NULL;
}

char *get(LRU_cache *cache, char *key) {
    HASH_node *node = get_from_hash_table(cache->table, key);

    if (node == NULL)
        return NULL;
    move_to_head(cache, node);

    return node->value;
}

void put(LRU_cache *cache, char *key, char *value) {
    HASH_node *node = get_from_hash_table(cache->table, key);

    if (node != NULL) {
        node->value = value;
        move_to_head(cache, node);
        return;
    }

    HASH_node *new_node = (HASH_node *) malloc(sizeof(HASH_node));

    new_node->key = malloc(strlen(key) + 1);
    strcpy(new_node->key, key);
    new_node->value = malloc(strlen(value) + 1);
    strcpy(new_node->value, value);

    add_to_head(cache, new_node);
    add_to_hash_table(cache->table, new_node);

    if (cache->size > cache->capacity) {
        HASH_node *last_node = remove_tail(cache);
        remove_from_hash_table(cache->table, last_node);
        free(last_node);
    }
}





void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int is_ip(char *ip) {
    if (ip == NULL) {
        return 0;
    }

    char *buffer = malloc(sizeof(ip) + 1);
    strcpy(buffer, ip);

    int count = 0;
    char *token = strtok(buffer, ".");

    while (token != NULL) {
        int value = atoi(token);

        if (value < 0 || value > 255) {
            free(buffer);
            return 0;
        }

        count++;
        token = strtok(NULL, ".");
    }
    free(buffer);

    if (count == 4)
        return 1;
    return 0;
}

char *get_value(char *buffer) {
    char *value = strstr(buffer, STR_PAR1);

    if (value != NULL) {
        value += strlen(STR_PAR1);
        value[strlen(value) - 2] = '\0';

        //strcpy(value1, value);
        return value;
    }
    value = strstr(buffer, STR_PAR2);

    if (value != NULL) {
        value += strlen(STR_PAR2);
        value[strlen(value) - 2] = '\0';

        //strcpy(value1, value);
        return value;
    }

    return NULL;
}

char *file_search(FILE *f_dns, char *key) {
    char *buffer = malloc(STR_SIZE);
    char *value = NULL;

    rewind(f_dns);
    while (!feof(f_dns)) {
        fgets(buffer, STR_SIZE - 1, f_dns);

        if (strstr(buffer, key) != NULL) {
            value = get_value(buffer);
            return value;
        }
    }
    puts("Error, no such domain in the base\n");
    return NULL;
}

char *domain_search(LRU_cache *cache, char *key, FILE *f_dns) {
    char *buffer = get(cache, key);

    if (buffer != NULL)
        return buffer;

    buffer = file_search(f_dns, key);

    if (buffer == NULL)
        return NULL;

    if (!is_ip(buffer)) {
        strcat(buffer, STR_PAR2);
        buffer = file_search(f_dns, buffer);
    }

    put(cache, key, buffer);

    return buffer;
}

void add_new_dns(char *domain, char *ip_str, FILE *f_dns) {
    char *buffer = malloc(sizeof(char) * STR_SIZE);

    rewind(f_dns);
    while (!feof(f_dns)) {
        fgets(buffer, STR_SIZE - 1, f_dns);

        if ((strstr(buffer, ip_str) != NULL && is_ip(ip_str)) || strstr(buffer, domain) != NULL) {
            puts("Error, such IP address or domain is already exists\n");
            getchar();

            return;
        }
    }

    fputs(domain, f_dns);

    if (is_ip(ip_str))
        fputs(STR_PAR2, f_dns);
    else
        fputs(STR_PAR1, f_dns);

    fputs(ip_str, f_dns);
    fputs("\r\n", f_dns);
}

int init_x(int min, int max) {
    int x;
    while (scanf("%d", &x) != 1 || x < min || x > max ||
           getchar() != '\n') {
        printf("Error1\n");
        rewind(stdin);
    }
    return x;
}

char *output_by_ip(FILE *f_dns, char *ip_str) {
    rewind(f_dns);
    char *buffer = malloc(STR_SIZE);
    char *domain;
    while (!feof(f_dns)) {
        fgets(buffer, STR_SIZE - 1, f_dns);
        if (strstr(buffer, ip_str) != NULL) {
            domain = strtok(buffer, " ");
            puts(domain);
            strcat(domain, "\r\n");
            return domain;
        }
    }
    return NULL;
}

void output_by_id(FILE *f_dns, char *domain_str) {
    rewind(f_dns);
    char *buffer = malloc(STR_SIZE);
    char *domain;
    while (!feof(f_dns)) {
        fgets(buffer, STR_SIZE - 1, f_dns);
        if (strstr(buffer, domain_str) != NULL) {
            domain = strtok(buffer, " ");
            puts(domain);
        }
    }
}

void show_cache(HASH_node *tail) {
    HASH_node *current = tail;
    while (current != NULL) {
        printf("%s - %s\n", current->key, current->value);
        current = current->prev;
    }
}

void menu(LRU_cache *cache, FILE *f_dns) {
    while (1) {
        char *domain_str;
        char *ip_str;
        //system("clear");
        puts("Menu\n"
             "1. Get IP address\n"
             "2. Add new domain\n"
             "3. Get domains by IP address\n"
             "0. Exit\n");
        int choice = init_x(0, 5);
        switch (choice) {
            case 0:
                return;
            case 1:
                domain_str = malloc(STR_SIZE);

               // system("clear");

                puts("Enter domain:\n");
                fgets(domain_str, STR_SIZE - 1, stdin);
                domain_str[strlen(domain_str) - 1] = '\0';

                puts("IP address:\n");
                ip_str = domain_search(cache, domain_str, f_dns);

                if (ip_str != NULL)
                    puts(ip_str);

                getchar();
                free(domain_str);
                break;
            case 2:
                domain_str = malloc(STR_SIZE);
                ip_str = malloc(STR_SIZE);

                //system("clear");

                puts("Enter domain:\n");
                fgets(domain_str, STR_SIZE - 1, stdin);
                domain_str[strlen(domain_str) - 1] = '\0';

                puts("Enter IP address:\n");
                fgets(ip_str, STR_SIZE - 1, stdin);
                ip_str[strlen(ip_str) - 1] = '\0';

                add_new_dns(domain_str, ip_str, f_dns);

                free(domain_str);
                free(ip_str);
                break;
            case 3:
                ip_str = malloc(STR_SIZE);

               // system("clear");

                puts("Enter IP address:\n");
                fgets(ip_str, STR_SIZE - 1, stdin);
                ip_str[strlen(ip_str) - 1] = '\0';

                domain_str = output_by_ip(f_dns, ip_str);
                output_by_id(f_dns, domain_str);

                getchar();
                free(ip_str);
                break;
            case 4:

                show_cache(cache->tail);
                getchar();
                break;
            default:
                puts("Undefined error");
                return;
        }
    }
}