#ifndef CACHE_H
#define CACHE_H

// Include standard libraries
#include <stddef.h>

// Define constants for cache policies
#define CACHE_POLICY_NONE 0
#define CACHE_POLICY_LRU 1
#define CACHE_POLICY_LIFO 2

// Define the default cache size
#define DEFAULT_CACHE_SIZE 100 // You can adjust this size as needed
#define MAX_CACHE_SIZE 1000     // Maximum cache size

typedef unsigned long long CacheNumber; // Ensure CacheNumber is defined

// Declare the CacheEntry structure
typedef struct CacheEntry {
    CacheNumber number;  // The number being cached
    int steps;           // Steps calculated for the Collatz function
    int usage;           // Usage counter for LRU policy
    struct CacheEntry *next; // Pointer to the next entry in the linked list
} CacheEntry;

// External variables to keep track of cache hits and misses
extern int cache_hit;
extern int cache_miss;

// Function declarations
void init_cache(const char *policy, int size);
void add_to_cache(CacheNumber number, int steps);
CacheEntry *find_in_cache(CacheNumber number);
void free_cache();
int collatz_steps_cached(CacheNumber num);
int collatz_steps(CacheNumber num); // Ensure this matches the type
void replace_lru(CacheNumber number, int steps);
void replace_lifo(CacheNumber number, int steps);
double get_hit_percentage(); // Function to get the hit percentage

#endif
