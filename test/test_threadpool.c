#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "threadpool.h"

void heavy_task(void *arg)
{
    int id = *(int*)arg;

    printf("Task %d starting\n", id);

    volatile long sum = 0;
    for (long i = 0; i < 100000000; i++)
        sum += i;

    printf("Task %d finished\n", id);

    free(arg);
}

int main()
{
    threadpool_t *pool = threadpool_create(4);

    clock_t start = clock();

    for (int i = 0; i < 8; i++)
    {
        int *arg = malloc(sizeof(int));
        *arg = i;

        threadpool_submit(pool, heavy_task, arg);
    }
	threadpool_wait(pool);

    threadpool_destroy(pool);

    clock_t end = clock();

    double time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Total time with threading: %.2f seconds\n", time);

	start = clock();

	for (int i=0; i<8; i++) {
		int *arg = malloc(sizeof(int));
		*arg = i;
		heavy_task(arg);
	}
	end = clock();

	time = (double)(end-start)/CLOCKS_PER_SEC;

    printf("Total time without threading: %.2f seconds\n", time);
}
