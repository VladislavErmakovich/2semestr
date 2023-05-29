#include "fun.h"

int main() {
    int choose = 0;
    cache_LRU *cache = (cache_LRU *) malloc(sizeof(cache_LRU));
    FILE *fp = fopen("DNS.txt", "a+");
    if (fp == NULL) {
        printf("\nSorry,the file was not found\n");
        getchar();
    }
    create_cache_LRU(CACHE_SIZE, cache);

    choose = menu();
    while (choose != 0) {
        if (choose == 1) {
            task_1(cache, fp);
        }
        if (choose == 2) {
            task_2(fp);
        }
        if (choose == 3) {
            task_3(fp);
        }
        if (choose == 4) {
            task_4(cache);
        }

        choose = menu();
    }
    delete_cache_LRU(cache);
    printf("\nGoodbye\n");
    return 0;
}
