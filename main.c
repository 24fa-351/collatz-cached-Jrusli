#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "collatz.h"

int main(int argc, char *argv[]) {
    // Ensure there are enough arguments
    if (argc < 6) {
        fprintf(stderr, "Usage: %s <arg1> <arg2> <arg3> <policy> <cache_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse command line arguments
    int arg1 = atoi(argv[1]);
    int arg2 = atoi(argv[2]);
    int arg3 = atoi(argv[3]);
    const char *policy = argv[4];
    int cache_size = atoi(argv[5]);

    // Initialize cache with the given policy and size
    init_cache(policy, cache_size);

    // Open a CSV file for writing
    FILE *file = fopen("results.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return EXIT_FAILURE;
    }

    // Write header to CSV file
    fprintf(file, "Number,Steps\n");

    // Perform operations with the cache and Collatz function
    for (int i = arg2; i <= arg3; i++) {
        CacheNumber random_number = rand() % arg1 + 1; // Random number generation
        int steps = collatz_steps_cached(random_number);
        
        // Print results (example)
        printf("Number: %llu, Steps: %d\n", random_number, steps);

        // Write results to CSV file
        fprintf(file, "%llu,%d\n", random_number, steps);
    }

    // Print cache statistics
    printf("Cache Hits: %d\n", cache_hit);
    printf("Cache Misses: %d\n", cache_miss);
    printf("Total Cache Accesses: %d\n", cache_hit + cache_miss);

    if (cache_hit + cache_miss > 0) {
        double cache_hit_percentage = (double)cache_hit / (cache_hit + cache_miss) * 100;
        printf("Cache Hit Percentage: %.2f%%\n", cache_hit_percentage);
    }

    // Free cache memory
    free_cache();

    // Close the CSV file
    fclose(file);

    return EXIT_SUCCESS;
}
