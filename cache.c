#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include "cache.h"


CacheEntry *cache_head = NULL; 
int cache_size = DEFAULT_CACHE_SIZE; 
int cache_policy = CACHE_POLICY_NONE; 
int current_cache_usage = 0; 
int usage_counter = 0; 

int cache_hit = 0; 
int cache_miss = 0; 


void init_cache(const char *policy, int size) {
    cache_size = size;
    current_cache_usage = 0;
    usage_counter = 0;
    cache_hit = 0;  
    cache_miss = 0; 
    cache_head = NULL;

    if (strcmp(policy, "none") == 0) {
        cache_policy = CACHE_POLICY_NONE;
    } else if (strcmp(policy, "LRU") == 0) {
        cache_policy = CACHE_POLICY_LRU;
    } else if (strcmp(policy, "LIFO") == 0) {
        cache_policy = CACHE_POLICY_LIFO;
    }
}


void add_to_cache(CacheNumber number, int steps) {
    CacheEntry *new_entry = (CacheEntry *)malloc(sizeof(CacheEntry));
    new_entry->number = number;
    new_entry->steps = steps;
    new_entry->usage = ++usage_counter; 
    new_entry->next = cache_head; 
    cache_head = new_entry; 
}


CacheEntry *find_in_cache(CacheNumber number) {
    CacheEntry *current = cache_head;
    while (current != NULL) {
        if (current->number == number) {
            current->usage = ++usage_counter; 
            return current; 
        }
        current = current->next; 
    }
    return NULL;
}


void replace_lifo(CacheNumber number, int steps) {
  
    if (cache_head == NULL) return;

 
    CacheEntry *old_entry = cache_head;
    cache_head = old_entry->next;

    free(old_entry); 

 
    add_to_cache(number, steps);
}


CacheEntry *find_lru_node() {
   
    CacheEntry *lru = cache_head;
    CacheEntry *current = cache_head;

    
    while (current != NULL) {
        if (current->usage < lru->usage) {
            lru = current; 
        }
        current = current->next;
    }
    return lru; 
}


void replace_lru(CacheNumber number, int steps) {
    CacheEntry *lru = find_lru_node();
    if (lru != NULL) {
        lru->number = number; 
        lru->steps = steps; 
        lru->usage = ++usage_counter; 
    }
}


double get_hit_percentage() {
    int total_accesses = cache_hit + cache_miss;
    if (total_accesses == 0) {
        return 0.0;
    }
    return ((double)cache_hit / total_accesses) * 100;
}


int collatz_steps_cached(CacheNumber num) {
   
    if (cache_policy == CACHE_POLICY_NONE) {
        return collatz_steps(num); 
    }

   
    CacheEntry *entry = find_in_cache(num);
    if (entry != NULL) {
        cache_hit++; 
        return entry->steps; 
    }

    
    int steps = collatz_steps(num);
    
    
    if (current_cache_usage < cache_size) {
        add_to_cache(num, steps);
        current_cache_usage++;
    } else if (cache_policy == CACHE_POLICY_LRU) {
        replace_lru(num, steps);
    } else if (cache_policy == CACHE_POLICY_LIFO) {
        replace_lifo(num, steps);
    }

    cache_miss++;
    return steps; 
}


void print_lifo_hit_percentage() {
    if (cache_policy == CACHE_POLICY_LIFO) {
        double hit_percentage = get_hit_percentage();
        printf("LIFO Cache Hit Percentage: %.2f%%\n", hit_percentage);
    }
}


void free_cache() {
    CacheEntry *current = cache_head;
    while (current != NULL) {
        CacheEntry *temp = current;
        current = current->next; 
        free(temp); 
    }
    cache_head = NULL; 
}
