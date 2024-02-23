typedef struct Vector2 {
    ssize_t x, y;
} Vector2;

typedef struct Dimention {
    size_t id;
    size_t earn_rate;
    size_t tick_speed;
    size_t amount;
} Dimention;

typedef struct GameProgress {
    size_t dims_unlocked;
    size_t matter_amount;
} GameProgress;

