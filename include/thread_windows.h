#ifndef THREAD_WINDOWS_H
#define THREAD_WINDOWS_H

#include <windows.h>

typedef HANDLE thread_t;
typedef CRITICAL_SECTION thread_mutex;
typedef CONDITION_VARIABLE thread_cond;

// Thread creation
int thread_create(thread_t *thread, void (*func)(void*), void *arg);

// Mutex functions
void thread_mutex_init(thread_mutex *mutex);
void thread_mutex_lock(thread_mutex *mutex);
void thread_mutex_unlock(thread_mutex *mutex);
void thread_mutex_destroy(thread_mutex *mutex);

// Condition variable functions
void thread_cond_init(thread_cond *cond);
void thread_cond_wait(thread_cond *cond, thread_mutex *mutex);
void thread_cond_signal(thread_cond *cond);
void thread_cond_destroy(thread_cond *cond);

#endif
