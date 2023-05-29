#include "fun.h"


Node* create(char* question){
    Node* node=(Node*) malloc(sizeof(Node));
    node->question= strdup(question);
    node->no=NULL;
    node->yes=NULL;

    return node;
}

Node* read(FILE *fp){
    char question[STRING];
    fgets(question,sizeof (question),fp);
    if(question[DEV]=='&')
    {
        return NULL;
    }
    Node* node=create(question);
    node->yes=read(fp);
    node->no=read(fp);
    return node;
}

char* get_answer() {//fgets не работает()()()

    int len = 1;
    char* str = (char *) malloc(len * sizeof(char));
    for (int i = 0;; i++) {
        str[i] = (char) getchar();
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        len++;
        str = (char *) realloc(str, len);
    }

    return str;

}

void game(Node* node)
{
    printf("\n%s\n",node->question);

    char *answer=get_answer();

    printf("\n%s\n",answer);
    while (strcmp(answer, "yes") != 0 && strcmp(answer, "no") != 0) {
        PlaySound(TEXT("nepravilno-poprobuy-esch-raz.wav"), NULL, SND_ASYNC | SND_FILENAME);
        printf("Sorry, you entered incorrectly\n");
        printf("Input \"yes\" or \"no\"\n");
        rewind(stdin);

       answer=get_answer();
    }

    if(strcmp(answer,"yes")==0)
    {
        if(node->yes==NULL)
        {
            printf("\nYes, I win. I predict your person.\nWho is next?)\n");
            PlaySound(TEXT("muzyika-s-proydennoy-missiey-iz-gta-san-andreas.wav"), NULL, SND_ASYNC | SND_FILENAME);
            free(answer);
            getchar();
        }
        else
        {
            free(answer);
            game(node->yes);
        }

    }
    else
        {
            free(answer);
            if(node->no==NULL)
            {
                printf("Sorry, I don't know.Who is it?\n");
                PlaySound(TEXT("lose.wav"), NULL, SND_ASYNC | SND_FILENAME);
                char* person=get_answer();

                printf("What question should I ask to guess it?\n");
                char* question=get_answer();
                question=realloc(question,strlen(question)+1);
                strcat(question, "\n");

                printf("Please, give me answer for this person\n");
                char* answer_for_person=get_answer();

                Node* new_node=create(question);
                free(question);

                person=realloc(person,strlen(person)+1);
                strcat(person,"\n");
                create_new_node(answer_for_person,person,new_node);
                free(answer_for_person);
                memcpy(node,new_node,sizeof (Node));
                free(new_node);
            }
            else
            {
                game(node->no);
            }

        }

}

void info_to_data(Node *node, FILE *fp) {

    if (node == NULL) {

        fprintf(fp, "&\n");
        return;
    }

    fprintf(fp, "%s", node->question);
    info_to_data(node->yes, fp);
    info_to_data(node->no, fp);

}

int menu()
{
    PlaySound(TEXT("o-privet.wav"), NULL, SND_ASYNC | SND_FILENAME);
    int ch=0;
    printf("\nAkinator");
    printf("\n\3Menu\n1.Play game\n2.Exit\n");
    while((scanf_s("%d",&ch))!=1||ch>2||ch<1||getchar()!='\n') {
        printf("\nError, please try again...\n\t");
        PlaySound(TEXT("chto-tyi-nesesh.wav"), NULL, SND_ASYNC | SND_FILENAME);
        getchar();
        rewind(stdin);
    }
    return ch;
}
void free_tree(Node* root)
{
    if(root==NULL)
    {
        return;
    }
    free_tree(root->no);
    free_tree(root->yes);
    free(root->question);
    free(root);

}

void create_new_node(char* answer_for_person,char* person,Node* node)
{
    if(strcmp(answer_for_person,"yes")==0)
    {
        node->yes= create(person);
        node->no= create(node->question);
        free(person);
    }
    else
    {
        node->yes=create(node->question);
        node->no=create(person);
        free(person);
    }
}