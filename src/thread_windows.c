#include "thread_windows.h"
#include <stdlib.h>

typedef struct thread_args_t {
	(*func)(void *);
	void *args;
} args_t;

// Only allowed one argument so we pack the func and arg into a struct.
DWORD WINAPI wrapper_func(LPVOID lpParam) {
	args_t* wrapper = (args_t*)lpParam;
	wrapper->func(wrapper->args);
	free(wrapper);
	return 0;
}

int create_thread(thread_t* thread, void(*func)(void*), void*args) {
	args_t *wrapper = malloc(sizeof(args_t));
	wrapper->func = func;
	wraper->args = args;
	*thread = CreateThread(NULL, 0, wrapper_func, wrapper, 0, NULL);
	if (!*thread) {
		free(wrapper);
		return -1;
	}
	return 0;
}

void thread_mutex_init(thread_mutex *mutex) {
    InitializeCriticalSection(mutex);
}

void thread_mutex_lock(thread_mutex *mutex) {
    EnterCriticalSection(mutex);
}

void thread_mutex_unlock(thread_mutex *mutex) {
    LeaveCriticalSection(mutex);
}

void thread_mutex_destroy(thread_mutex *mutex) {
    DeleteCriticalSection(mutex);
}

void thread_cond_init(thread_cond *cond) {
    InitializeConditionVariable(cond);
}

void thread_cond_wait(thread_cond *cond, thread_mutex *mutex) {
    SleepConditionVariableCS(cond, mutex, INFINITE);
}

void thread_cond_signal(thread_cond *cond) {
    WakeConditionVariable(cond);
}

void thread_cond_broadcast(thread_cond *cond) {
    WakeAllConditionVariable(cond);
}


void thread_join(thread_t *thread) {
	WaitForSingleObject(*thread, INFINITE);
	CloseHandle(*thread);
}

void thread_cond_destroy(thread_cond *cond) {
    // No-op on Windows; CONDITION_VARIABLE doesn’t need explicit destroy
	return;
}
