#include "fun.h"

int check_ip(char *ip) {
    if (ip == NULL) {
        return 0;
    }
    char *buffer = malloc(sizeof(ip) + 1);
    strcpy(buffer, ip);

    int cnt = 0;

    char *token = strtok(buffer, ".");
    while (token != NULL) {
        int value = atoi(token);

        if (value < 0 || value > 255) {
            free(buffer);
            return 0;
        }
        cnt++;
        token = strtok(NULL, ".");
    }

    if (cnt == 4) {
        return 1;
    }
    return 0;
}

char *get_ip_mother(char *buffer) {
    char *value = strstr(buffer, STR_TYPE_2);
    if (value != NULL) {
        value += strlen(STR_TYPE_2);
        value[strlen(value) - 1] = '\0';
        return value;
    }
    value = strstr(buffer, STR_TYPE_1);
    if (value != NULL) {
        value += strlen(STR_TYPE_1);
        value[strlen(value) - 1] = '\0';
        return value;
    }
    return NULL;
}

char *search_file(FILE *fp, char *domain) {
    char *buffer = malloc(SIZE_STRING);
    char *value = NULL;

    rewind(fp);
    while (!feof(fp)) {
        fgets(buffer, SIZE_STRING - 1, fp);
        if (strstr(buffer, domain) != NULL) {
            value = get_ip_mother(buffer);
            return value;
        }
    }
    puts("Error\n");
    return NULL;
}

char *search_domain(cache_LRU *cache, char *domain, FILE *fp) {
    char *buffer = get_domain(domain, cache);
    if (buffer != NULL) {
        return buffer;
    }

    buffer = search_file(fp, domain);
    if (buffer == NULL) {
        return NULL;
    }
    if (!check_ip(buffer)) {
        strcat(buffer, STR_TYPE_1);
        buffer = search_file(fp, buffer);
    }
    put_domain(domain, buffer, cache);
    return buffer;
}

void add_new_domain(char *domain, char *ip, FILE *fp) {
    char *buffer = malloc(sizeof(char) * SIZE_STRING);
    rewind(fp);
    while (!feof(fp)) {
        fgets(buffer, SIZE_STRING - 1, fp);
        if ((strstr(buffer, ip) != NULL && check_ip(ip)) || strstr(buffer, domain) != NULL) {
            puts("Error\n");
            getchar();
            return;
        }
    }
    fputs(domain, fp);
    if (check_ip(ip)) {
        fputs(STR_TYPE_1, fp);
    } else {
        fputs(STR_TYPE_2, fp);
    }
    fputs(ip, fp);
    fputs("\n", fp);

}

char *output_by_ip(FILE *fp, char *ip) {
    if(check_ip(ip)==1) {
        rewind(fp);
        char *buffer = malloc(SIZE_STRING);
        char *domain;
        while (!feof(fp)) {
            fgets(buffer, SIZE_STRING - 1, fp);
            if (strstr(buffer, ip) != NULL) {
                domain = strtok(buffer, " ");
                puts(domain);
                strcat(domain, "\n");
                return domain;
            }
        }
    }
    return NULL;
}

void output_by_id(FILE *fp, char *str_domain) {
    rewind(fp);
    char *buffer = malloc(SIZE_STRING);
    char *domain;
    while (!feof(fp)) {
        fgets(buffer, SIZE_STRING - 1, fp);
        if (strstr(buffer, str_domain) != NULL) {
            domain = strtok(buffer, " ");
            puts(domain);
        }
    }
}

void output_cache(node_hash *tail) {
    node_hash *current = tail;
    while (current != NULL) {
        printf("%s-%s\n", current->domain, current->ip);
        current = current->prev;
    }
}

void task_1(cache_LRU *cache, FILE *fp) {
    char *domain = malloc(SIZE_STRING);
    char *ip;
    puts("Enter the domain:\n");
    fgets(domain, SIZE_STRING - 1, stdin);
    domain[strlen(domain) - 1] = '\0';
    puts("IP: ");
    node_hash *node=cache->head;
    while(node!=NULL)
    {
        if(strcmp(domain,node->domain)==0)
        {
            puts(node->ip);
            put_domain(domain, node->ip, cache);
            return;
        }
        node=node->next;
    }

        ip = search_domain(cache, domain, fp);

    if (ip != NULL) {
        puts(ip);
    }
    free(domain);

}

void task_2(FILE *fp) {
    char *domain = malloc(SIZE_STRING);

    puts("Enter the domain:\n");
    fgets(domain, SIZE_STRING - 1, stdin);
    domain[strlen(domain) - 1] = '\0';

    char *ip = malloc(SIZE_STRING);
    puts("Enter the IP:\n");
    fgets(ip, SIZE_STRING - 1, stdin);
    ip[strlen(ip) - 1] = '\0';

    add_new_domain(domain, ip, fp);

    free(domain);
    free(ip);
}

void task_3(FILE *fp) {
    char *ip = malloc(SIZE_STRING);
    puts("Enter the IP:\n");
    fgets(ip, SIZE_STRING - 1, stdin);
    ip[strlen(ip) - 1] = '\0';

    char *domain = output_by_ip(fp, ip);
    if(domain!=NULL) {
        output_by_id(fp, domain);
    }
    else
    {
        printf("\nError\n");
    }
    getchar();
    free(ip);
}

void task_4(cache_LRU *cache) {
    output_cache(cache->tail);
}

int menu() {
    int ch = 0;
    printf("\n\tMenu\n");
    printf("\t1.IP search\n\t2.Add new domain\n\t3.Domain search by IP\n\t4.Cache output\n\t0.Exit\n");
    while ((scanf_s("%d", &ch)) < 0 || ch > 4 || getchar() != '\n') {
        printf("\nError, please try again...\n\t");
        rewind(stdin);
    }
    return ch;
}