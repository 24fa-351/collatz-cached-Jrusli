#ifndef CACHE_H
#define CACHE_H


#include <stddef.h>


#define CACHE_POLICY_NONE 0
#define CACHE_POLICY_LRU 1
#define CACHE_POLICY_LIFO 2


#define DEFAULT_CACHE_SIZE 100 
#define MAX_CACHE_SIZE 1000  

typedef unsigned long long CacheNumber; 


typedef struct CacheEntry {
    CacheNumber number;  
    int steps;          
    int usage;           
    struct CacheEntry *next; 
} CacheEntry;


extern int cache_hit;
extern int cache_miss;


void init_cache(const char *policy, int size);
void add_to_cache(CacheNumber number, int steps);
CacheEntry *find_in_cache(CacheNumber number);
void free_cache();
int collatz_steps_cached(CacheNumber num);
int collatz_steps(CacheNumber num);
void replace_lru(CacheNumber number, int steps);
void replace_lifo(CacheNumber number, int steps);
double get_hit_percentage(); 

#endif
