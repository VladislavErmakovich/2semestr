#include "fun.h"

int main() {
    int choose=0;
    choose=menu();
    Node *root = NULL;

   while(choose!=2)
   {

        FILE *fp = fopen("data.txt", "r");
        if (fp == NULL) {
            printf("\nSorry,the file was not found\n");
            getch();
        }

        root = read(fp);
        fclose(fp);

        game(root);

        fp= fopen("data.txt","w");
        if (fp == NULL) {
            printf("\nSorry,the file was not found\n");
            //PlaySound(TEXT("chto-tyi-nesesh.wav"), NULL, SND_ASYNC | SND_FILENAME);
            getch();
        }
        info_to_data(root,fp);
        fclose(fp);
        choose=menu();
    }
    free_tree(root);
        printf("\n\tSEE YOU SPACE COWBOY...\n\t");
   PlaySound(TEXT("a-muha-toje-vertolet-polnaya-versiya-pesni.wav"), NULL, SND_ASYNC | SND_FILENAME);
    getchar();
    return 0;
}
