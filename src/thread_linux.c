
#include "thread_internals.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct thread_args_t {
	void (*func)(void *);
	void *args;
} args_t;

// Only allowed one argument so we pack the func and arg into a struct.
static void* wrapper_func(void* arg) {
	args_t* wrapper = (args_t*)arg;
	wrapper->func(wrapper->args);
	free(wrapper);
	return NULL;
}

int thread_create(thread_t* thread, void(*func)(void*), void*args) {
	args_t *wrapper = malloc(sizeof(args_t));
	if (!wrapper) return -1;
	wrapper->func = func;
	wrapper->args = args;
	 if (pthread_create(thread, NULL, wrapper_func, wrapper) != 0) {
		 free(wrapper);
		 return -1;
	 }
	 return 0;
}

void thread_join(thread_t *thread)
{
    pthread_join(*thread, NULL);
}

/* Mutex */

void thread_mutex_init(thread_mutex *mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void thread_mutex_lock(thread_mutex *mutex)
{
    pthread_mutex_lock(mutex);
}

void thread_mutex_unlock(thread_mutex *mutex)
{
    pthread_mutex_unlock(mutex);
}

void thread_mutex_destroy(thread_mutex *mutex)
{
    pthread_mutex_destroy(mutex);
}

/* Condition variables */

void thread_cond_init(thread_cond *cond)
{
    pthread_cond_init(cond, NULL);
}

void thread_cond_wait(thread_cond *cond, thread_mutex *mutex)
{
    pthread_cond_wait(cond, mutex);
}

void thread_cond_signal(thread_cond *cond)
{
    pthread_cond_signal(cond);
}

void thread_cond_broadcast(thread_cond *cond)
{
    pthread_cond_broadcast(cond);
}

void thread_cond_destroy(thread_cond *cond)
{
    pthread_cond_destroy(cond);
}
