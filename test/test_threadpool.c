#include <stdio.h>
#include <stdlib.h>
#include "threadpool.h"

void work(void *arg)
{
    int id = *(int*)arg;

    printf("Task %d running\n", id);

#ifdef _WIN32
    Sleep(500);
#else
    usleep(500000);
#endif

    printf("Task %d finished\n", id);

    free(arg);
}

int main()
{
    threadpool_t *pool = threadpool_create(4);

    if (!pool) {
        printf("Failed to create threadpool\n");
        return 1;
    }

    for (int i = 0; i < 20; i++) {

        int *arg = malloc(sizeof(int));
        *arg = i;

        threadpool_submit(pool, work, arg);
    }

    printf("Tasks submitted\n");

#ifdef _WIN32
    Sleep(5000);
#else
    sleep(5);
#endif

    threadpool_destroy(pool);

    printf("Threadpool destroyed\n");

    return 0;
}
