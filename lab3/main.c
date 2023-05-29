#include "fun.h"

int main(int argc, char** argv) {

        if (argc != 2) {
            printf("Failed!!!!\n");
            return 1;
        }

        int choose=0;

        while((choose=menu())!=5)
         {
            if(choose==1)
            {
              negative(argv[1]);
            }
            if(choose==2)
            {
                black_and_white(argv[1]);
            }
            if(choose==3)
            {
                gamma_correction(argv[1]);
            }
            if(choose==4)
            {
                median_correction(argv[1]);
            }
         }
    system("cls");
    printf("\n\tGoodluck\3\n");
    getchar();
    system("cls");
    return 0;
}
