#include <stdatomic.h>
#ifndef _H_TYPES
#define _H_TYPES
typedef struct Vector2 {
    ssize_t x, y;
} Vector2;

typedef struct Dimention {
    atomic_size_t id;
    atomic_size_t earn_rate;
    atomic_size_t tick_speed;
    atomic_size_t amount;
} Dimention;

typedef struct GameProgress {
    atomic_size_t dims_unlocked;
    atomic_size_t matter_amount;
} GameProgress;
#endif
