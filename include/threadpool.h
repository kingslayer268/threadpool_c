#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<stdio.h>
typedef struct threadpool threadpool_t;
threadpool_t* threadpool_create(int threads);
int threadpool_submit(threadpool_t *pool,
					  void (*func)(void *),
					  void* args);
void threadpool_destroy(threadpool_t *pool);
void threadpool_wait(threadpool_t *pool);
#endif
