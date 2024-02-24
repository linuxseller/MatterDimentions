#include <stdatomic.h>

#ifdef _WIN32
#include <windows.h>
#define sleep Sleep
#else
#include <unistd.h>
#endif

#include "types.h"

void *calculation_loop(void *arg)
{
    (void) arg;
    while(1){
        for (size_t i = 0; i<atomic_load(&game_progress.dims_unlocked) && i < DIMENTIONS_TOTAL; i++) {
            size_t dim_count = atomic_load(&dimentions[i].amount);
            size_t amount = atomic_load(&game_progress.matter_amount);
            size_t earn_rate = atomic_load(&dimentions[i].earn_rate);
            atomic_exchange(&game_progress.matter_amount, amount+dim_count*earn_rate);
        }
        sleep(1);
    }
    return NULL;
}
