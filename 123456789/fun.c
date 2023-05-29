#include "fun.h"

void push(FIFO **head, FIFO **tail, char *word, int repeat) {
    FIFO *tmp = (FIFO *) malloc(sizeof(FIFO));
    tmp->next = NULL;
    tmp->word = word;
    tmp->repeat = repeat;
    tmp->flag = 0;
    tmp->switch_word = malloc(0);
    if (*head == NULL) {
        *head = tmp;
        *tail = tmp;
    } else {
        (*tail)->next = tmp;
        *tail = tmp;
    }
}

void pop(FIFO **head) {
    if (*head != NULL) {
        free((*head)->word);
        free((*head)->switch_word);
        *head = (*head)->next;
    } else {
        printf("retytr");
    }
}

string *read(char file[]) {
    FILE *fp = fopen(file, "rb");
    if (fp == NULL) {
        printf("Error\n");
        return 0;
    }
    string *str = malloc(sizeof(string));
    str->word = NULL;
    str->len = 0;
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *text = calloc(len, sizeof(char));
    fread(text, 1, len, fp);
    char delim[] = " ";
    char *table = strtok(text, delim);
    while (table != NULL) {
        str->word = realloc(str->word, (str->len + 1) * sizeof(char *));
        str->word[str->len++] = table;
        table = strtok(NULL, delim);
    }
    /* int i = 0;
     // printf("\n\n%s\n",str->word[i]);
     while (i != str->len) {
         printf("%s %d\n", str->word[i], i + 1);
         i++;
     }
     printf("\n%d\n", str->len);*/
    fclose(fp);
    return str;
}

int compare(FIFO *head, char *word) {
    while (head) {
        if (strcmp(head->word, word) == 0) {
            return 1;
        } else {
            head = head->next;
        }
    }
    return 0;

}

void creat_FIFO(FIFO **head, FIFO **tail, string *table) {
    for (int i = 0; i < table->len; i++) {
        if (compare(*head, table->word[i]))continue;
        if (strstr(table->word[i], "\n") != NULL)continue;

        int repeat = 0;
        for (int j = 0; j < table->len; j++) {
            if (strcmp(table->word[i], table->word[j]) == 0) {
                repeat++;
            }
        }
        push(&(*head), &(*tail), table->word[i], repeat);
    }
}

void len_words(FIFO **head) {
    FIFO *t = *head;
    while (t != NULL) {
        t->len = strlen(t->word);
        t = t->next;
    }
}

void members_FIFO(FIFO *head, FIFO **first, FIFO **last) {

    FIFO *tmp1 = NULL;
    FIFO *tmp2 = NULL;

    if (head == NULL || head->next == NULL) {
        (*first) = head;
        (*last) = NULL;
        return;
    }

    tmp2 = head;
    tmp1 = head->next;

    while (tmp1 != NULL) {
        tmp1 = tmp1->next;
        if (tmp1 != NULL) {
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }
    }
    (*first) = head;
    (*last) = tmp2->next;
    tmp2->next = NULL;

}

void merging(FIFO *a, FIFO *b, FIFO **c) {

    FIFO tmp;
    *c = NULL;

    if (a == NULL) {
        *c = b;
        return;
    }

    if (b == NULL) {
        *c = a;
        return;
    }

    if (a->repeat * a->len > b->repeat * b->len) {
        *c = a;
        a = a->next;
    } else {
        *c = b;
        b = b->next;
    }

    tmp.next = *c;

    while (a && b) {
        if (a->repeat * a->len > b->repeat * b->len) {
            (*c)->next = a;
            a = a->next;
        } else {
            (*c)->next = b;
            b = b->next;
        }
        (*c) = (*c)->next;
    }

    if (a) {
        while (a) {
            (*c)->next = a;
            (*c) = (*c)->next;
            a = a->next;
        }
    }

    if (b) {
        while (b) {
            (*c)->next = b;
            (*c) = (*c)->next;
            b = b->next;
        }
    }
    *c = tmp.next;

}

void merge_sort(FIFO **head) {

    FIFO *first = NULL;
    FIFO *last = NULL;

    if ((*head == NULL) || ((*head)->next == NULL)) return;

    members_FIFO(*head, &first, &last);
    merge_sort(&first);
    merge_sort(&last);
    merging(first, last, head);

}

void switch_words(FIFO *head) {
    FIFO *s1 = head;
    while (s1) {


        FIFO *s2 = head;
        while (s2) {
            int con = 0;
            con = (s1->repeat * s1->len + s2->repeat + s2->len) - (s1->len * s2->repeat + s2->len * s1->repeat) - 2 -
                  s1->len - s2->len;
            // printf("\n%d\n",con);
            if (s1->len > s2->len && s2->flag != 1 && s1->flag != 2 && s1->flag != 1 && s2->flag != 2 && s2->len < 3 &&
                s2->repeat < s1->repeat && con > 0) {
                s2->flag = 2;
                s1->flag = 1;
                // printf("\n%s\n",word);
                s1->switch_word = s2->word;
                s2->switch_word = s1->word;
            }
            s2 = s2->next;
            // printf("\n%s-%s\n ",s1->word,s1->switch_word);
        }
        s1 = s1->next;
    }
    fclose(fopen("library.txt", "wb"));

    FIFO *h = head;
    FILE *fptr = fopen("library.txt", "ab");
    if (fptr == NULL) exit(0);
    while (h != NULL) {
        if (h->flag == 1) {
            fputs(h->word, fptr);
            fputs(" ", fptr);
            fputs(h->switch_word, fptr);
            fputs("\n", fptr);
        }
        h = h->next;
    }
    fclose(fptr);
}


void compressor(char file[]) {
    setlocale(LC_ALL, "rus");
    string *table = read(file);

    FILE *fptr = fopen(file, "rb");

    fseek(fptr, 0L, SEEK_END);
    long size = ftell(fptr);
    printf("\n\tSize file: %ld", size);

    FIFO *head = NULL;
    FIFO *tail = NULL;

    creat_FIFO(&head, &tail, table);
    len_words(&head);

    merge_sort(&head);
    switch_words(head);

    FIFO *p = head;
    while (p != NULL) {
        for (int i = 0; i < table->len; i++) {
            if (strcmp(table->word[i], p->switch_word) == 0) {
                if (p->flag == 1) {
                    table->word[i] = p->word;
                }
            } else {
                if (strcmp(table->word[i], p->word) == 0) {
                    if (p->flag == 1)table->word[i] = p->switch_word;
                }
            }
        }
        p = p->next;
    }
    fclose(fopen("compress.txt", "wb"));

    fclose(fptr);
    fptr = fopen("compress.txt", "ab");
    if (fptr == NULL) exit(0);
    for (int i = 0; i < table->len; ++i) {
        fputs(table->word[i], fptr);
        if (i != table->len - 1) fputs(" ", fptr);
    }
    FIFO *tmp = head;
    fputs("\n*\n", fptr);
    while (tmp != NULL) {
        if (tmp->flag == 1 || tmp->flag == 2) {
            fputs(tmp->word, fptr);
            fputs(" ", fptr);
            fputs(tmp->switch_word, fptr);
            fputs("\n", fptr);
        }
        tmp = tmp->next;
    }


    fseek(fptr, 0L, SEEK_END);
    size = ftell(fptr);
    printf("\n\tSize file new: %ld\n", size);
    fclose(fptr);
    fptr = fopen("library.txt", "rb");
    fseek(fptr, 0L, SEEK_END);
    size = ftell(fptr);
    printf("\n\tSize file new: %ld\n", size);

    free(head);
    free(tail);
    free(p);
    //free(tmp);
    fclose(fptr);

}