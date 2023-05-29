#include "fun.h"

void push(FIFO **head, FIFO **tail, char *word) {
    FIFO *tmp = (FIFO *) malloc(sizeof(FIFO));
    tmp->next = NULL;
    tmp->word = word;
    if (*head == NULL) {
        *head = tmp;
        *tail = tmp;
    } else {
        (*tail)->next = tmp;
        *tail = tmp;
    }
}

void pop(FIFO **head, FIFO **tail) {
    if (*head != NULL) {
        *head = (*head)->next;
    } else {
        *tail = NULL;
    }
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
    fclose(fp);
    return str;
}

void analiz(FIFO** head,FIFO** tail)
{
    FILE *fp = fopen("compress.txt", "rb");
    if (fp == NULL) exit(EXIT_FAILURE);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *s = calloc(255, 1);
    fgets(s, 255, fp);
    while (*s != '*') {
        fgets(s, 255, fp);
    }
    fgets(s, 255, fp);

    char *pos = calloc(255, 1);
    *pos = *s;

    long len = 0;
    while (!feof(fp)) {
        fgetc(fp);
        len++;
    }
    len = len+8;

    fclose(fopen("example.txt", "wb"));
    char *text = calloc(size - len, sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(text, 1, size - len, fp);
    fp = fopen("example.txt", "ab");
    fputs(text, fp);
    fclose(fp);

    fp = fopen("compress.txt", "rb");
    char *library = calloc(len, sizeof(char));
    fclose(fopen("ex_lib.txt", "wb"));

    while (*s != '*') {
        fgets(s, 255, fp);
    }

    FILE* ex=fopen("ex_lib.txt", "ab");
    while(!feof(fp)){
        fgets(library, len, fp);
        fputs(library, ex);
    }
    fclose(ex);
    fclose(fp);
    ex=fopen("ex_lib.txt", "rb");
    fseek(ex,0L,SEEK_SET);
    fseek(ex,0L,SEEK_END);
    long len_lab= ftell(ex);

    fseek(ex,0L,SEEK_SET);
    string *str = malloc(sizeof(string));
    str->word = NULL;
    str->len = 0;
    char *lab = calloc(len_lab, sizeof(char));
    fread(lab, 1, len_lab, ex);
    char delim[] = " \n";
    char *table = strtok(lab, delim);
    while (table != NULL) {
        str->word = realloc(str->word, (str->len + 1) * sizeof(char *));
        str->word[str->len++] = table;
        table = strtok(NULL, delim);
    }


    for (int i = 0; i < str->len; i++) {
        if (compare(*head, str->word[i]))continue;
        if (strstr(str->word[i], "\n") != NULL)continue;
        push(&(*head), &(*tail), str->word[i]);
    }
     FIFO *p=*head;
    FIFO * k=p->next;
    while(k!=NULL) {
        if (p->flag != 1) {
        p->switch_word = k->word;
        p->flag = 2;
        p->switch_flag=1;
        k->switch_word = p->word;
        k->switch_flag=0;
        k->flag = 1;
    }
        p=p->next;
        k=k->next;
    }
    fclose(ex);
   remove("ex_lib.txt");
   free(p);
   free(k);
   free(pos);
   free(s);
}



void decompressor(){
    setlocale(LC_ALL, "rus");
    FIFO* head=NULL;
    FIFO* tail=NULL;
    analiz(&head,&tail);
    fclose(fopen("decompress.txt","wb"));
    FILE *fp= fopen("compress.txt","rb");
    fseek(fp,0,SEEK_END);
    long size= ftell(fp);
    fseek(fp,0,SEEK_SET);
    fclose(fp);
    printf("\n\tSize compress,txt %ld",size);
    string *table = read("example.txt");

    FIFO *tmp=head;
    while(tmp!=NULL)
    {
        for(int i=0;i<table->len;i++)
        {
           // puts(table->word[i]);
            if(strcmp(tmp->switch_word,table->word[i])==0)
            {
                if(tmp->flag==1)
                {
                    table->word[i]=tmp->word;
                }
            }
            else
            {
                if(strcmp(table->word[i],tmp->word)==0)
                {
                    if(tmp->flag==1)
                    {
                        table->word[i]=tmp->switch_word;
                    }
                }
            }
        }
        tmp=tmp->next;
    }
   fp= fopen("decompress.txt","ab");
    if (fp == NULL) exit(0);
    for (int i = 0; i <table->len; ++i) {
        fputs(table->word[i], fp);
        if (i != table->len-1) fputs(" ", fp);
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    printf("\n\tSize decompress,txt %ld",size);
    fclose(fp);
free(tmp);
free(table);
free(head);
remove("example.txt");

}//доделать 424930