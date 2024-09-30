#include <stdlib.h>
#include <string.h>
#include <stdio.h>  // For printf
#include "cache.h"

// Declare global variables
CacheEntry *cache_head = NULL; // Head of the linked list for cache entries
int cache_size = DEFAULT_CACHE_SIZE; // Set the initial cache size
int cache_policy = CACHE_POLICY_NONE; // Set the default cache policy
int current_cache_usage = 0; // Track the current usage of the cache
int usage_counter = 0; // Counter for usage for LRU policy

int cache_hit = 0; // Track cache hits
int cache_miss = 0; // Track cache misses

// Function to initialize the cache with a given policy and size
void init_cache(const char *policy, int size) {
    cache_size = size;
    current_cache_usage = 0;
    usage_counter = 0;
    cache_hit = 0;  // Reset cache hit counter
    cache_miss = 0; // Reset cache miss counter
    cache_head = NULL; // Initialize cache head to NULL

    if (strcmp(policy, "none") == 0) {
        cache_policy = CACHE_POLICY_NONE;
    } else if (strcmp(policy, "LRU") == 0) {
        cache_policy = CACHE_POLICY_LRU;
    } else if (strcmp(policy, "LIFO") == 0) {
        cache_policy = CACHE_POLICY_LIFO;
    }
}

// Function to add a new entry to the cache
void add_to_cache(CacheNumber number, int steps) {
    CacheEntry *new_entry = (CacheEntry *)malloc(sizeof(CacheEntry));
    new_entry->number = number;
    new_entry->steps = steps;
    new_entry->usage = ++usage_counter; // Increment usage counter
    new_entry->next = cache_head; // Link new entry to head
    cache_head = new_entry; // Set new entry as the head
}

// Function to find a number in the cache
CacheEntry *find_in_cache(CacheNumber number) {
    CacheEntry *current = cache_head;
    while (current != NULL) {
        if (current->number == number) {
            current->usage = ++usage_counter; // Update usage for LRU
            return current; // Return found entry
        }
        current = current->next; // Move to the next entry
    }
    return NULL; // Return NULL if not found
}

// Function to replace the most recently added cache entry (LIFO policy)
void replace_lifo(CacheNumber number, int steps) {
    // Check if there are entries in the cache
    if (cache_head == NULL) return;

    // Remove the last added entry (head in this case)
    CacheEntry *old_entry = cache_head; // Assume the head is the most recently added
    cache_head = old_entry->next; // Move head to the next entry

    free(old_entry); // Free the removed entry

    // Add the new entry
    add_to_cache(number, steps);
}

// Function to find the least recently used node (for LRU policy)
CacheEntry *find_lru_node() {
    // Placeholder for LRU node search
    CacheEntry *lru = cache_head;
    CacheEntry *current = cache_head;

    // Search for the least recently used entry
    while (current != NULL) {
        if (current->usage < lru->usage) {
            lru = current; // Update least recently used
        }
        current = current->next;
    }
    return lru; // Return least recently used entry
}

// Function to replace the least recently used cache entry (LRU policy)
void replace_lru(CacheNumber number, int steps) {
    CacheEntry *lru = find_lru_node(); // Find the LRU node
    if (lru != NULL) {
        lru->number = number; // Replace its number
        lru->steps = steps; // Replace its steps
        lru->usage = ++usage_counter; // Update usage
    }
}

// Function to calculate and return the cache hit percentage
double get_hit_percentage() {
    int total_accesses = cache_hit + cache_miss;
    if (total_accesses == 0) {
        return 0.0; // Avoid division by zero
    }
    return ((double)cache_hit / total_accesses) * 100;
}

// Wrapper function with caching (supporting all policies)
int collatz_steps_cached(CacheNumber num) {
    // Use the cache if enabled, otherwise compute directly
    if (cache_policy == CACHE_POLICY_NONE) {
        return collatz_steps(num); // Call actual Collatz calculation if no caching
    }

    // Check for a cache hit
    CacheEntry *entry = find_in_cache(num);
    if (entry != NULL) {
        cache_hit++; // Increment the hit counter
        return entry->steps; // Return the cached steps
    }

    // Cache miss: calculate the steps
    int steps = collatz_steps(num);
    
    // Add the result to the cache based on the policy
    if (current_cache_usage < cache_size) {
        add_to_cache(num, steps);
        current_cache_usage++;
    } else if (cache_policy == CACHE_POLICY_LRU) {
        replace_lru(num, steps);
    } else if (cache_policy == CACHE_POLICY_LIFO) {
        replace_lifo(num, steps);
    }

    cache_miss++; // Increment the miss counter
    return steps; // Return the calculated steps
}

// Function to print the hit percentage if the LIFO policy is used
void print_lifo_hit_percentage() {
    if (cache_policy == CACHE_POLICY_LIFO) {
        double hit_percentage = get_hit_percentage();
        printf("LIFO Cache Hit Percentage: %.2f%%\n", hit_percentage);
    }
}

// Function to free the cache memory
void free_cache() {
    CacheEntry *current = cache_head;
    while (current != NULL) {
        CacheEntry *temp = current;
        current = current->next; // Move to the next entry
        free(temp); // Free the current entry
    }
    cache_head = NULL; // Reset head to NULL
}
