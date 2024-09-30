#ifndef COLLATZ_H
#define COLLATZ_H

#include "cache.h" 

// Declaration of the Collatz function
int collatz_steps(CacheNumber num);
int collatz_steps_cached(CacheNumber num); // Function declaration for the cached version

#endif
