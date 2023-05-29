#include "fun.h"

#define CACHE_SIZE 5

int main() {
    LRU_cache *cache = (LRU_cache *) malloc(sizeof(LRU_cache));
    FILE *f_dns = fopen("DNS.txt", "a+");

    init_LRU_cache(cache, CACHE_SIZE);

    menu(cache, f_dns);

    delete_LRU_cache(cache);
    return 0;
}

