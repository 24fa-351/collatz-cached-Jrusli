#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "collatz.h"

int main(int argc, char *argv[]) {
  
    if (argc < 6) {
        fprintf(stderr, "Usage: %s <arg1> <arg2> <arg3> <policy> <cache_size>\n", argv[0]);
        return EXIT_FAILURE;
    }


    int arg1 = atoi(argv[1]);
    int arg2 = atoi(argv[2]);
    int arg3 = atoi(argv[3]);
    const char *policy = argv[4];
    int cache_size = atoi(argv[5]);

   
    init_cache(policy, cache_size);

   
    FILE *file = fopen("results.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return EXIT_FAILURE;
    }

 
    fprintf(file, "Number,Steps\n");

  
    for (int i = arg2; i <= arg3; i++) {
        CacheNumber random_number = rand() % arg1 + 1; 
        int steps = collatz_steps_cached(random_number);
        

        printf("Number: %llu, Steps: %d\n", random_number, steps);

       
        fprintf(file, "%llu,%d\n", random_number, steps);
    }


    printf("Cache Hits: %d\n", cache_hit);
    printf("Cache Misses: %d\n", cache_miss);
    printf("Total Cache Accesses: %d\n", cache_hit + cache_miss);

    if (cache_hit + cache_miss > 0) {
        double cache_hit_percentage = (double)cache_hit / (cache_hit + cache_miss) * 100;
        printf("Cache Hit Percentage: %.2f%%\n", cache_hit_percentage);
    }


    free_cache();


    fclose(file);

    return EXIT_SUCCESS;
}
