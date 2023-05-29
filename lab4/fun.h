#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "string.h"
#include "windows.h"
#include "stdlib.h"
#include "conio.h"
#define STRING 100
#define DEV 0
/*typedef enum choose{
YES=1,
NO=2
}choose;
*/
typedef struct Node {
    char* question;
    struct Node* yes;
    struct Node* no;
}Node;
void game(Node* node);
Node* read(FILE *file);
Node* create(char* question);
char* get_answer();
void info_to_data(Node *node, FILE *fp);
int menu();
void free_tree(Node* root);
void create_new_node(char* answer_for_person,char* person,Node* node);



