#include "threadpool.h"
#ifdef _WIN32
#include "thread_windows.h"
#else
#include "thread_linux.h"
#endif

typedef struct task {
	void (*func)(void *);
	void *args;
	task_t *next;
} task_t;
typedef struct task_queue {
	task_t *head;
	task_t *tail;
	size_t size;
} tasks_t;

typedef struct threadpool {
	thread_t *threads;
	tasks_t *queue; 
	int thread_count;
	thread_mutex lock;
	thread_cond notify;
	int shutdown;
} threadpool_t;
void queue_init(tasks_t *queue) {
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
}

void enqueue(task_t *task, tasks_t *queue) {
	task->next = NULL;
	if (queue->size == 0) {
		queue->head = task;
	} else {
		queue->tail->next = task;
	}
	queue->tail = task;
	queue->size++;
}

task_t* dequeue(tasks_t *queue) {
	if (queue->size == 0) return NULL;
	task_t *item = queue->head;
	queue->head = queue->head->next;
	queue->size--;
	if (queue->size == 0) {
		queue->tail = NULL;
	}
	return item;
}

void worker_func(void* args) {
	threadpool_t* pool = (threadpool_t *) args;
	while (!pool->shutdown) {
		thread_mutex_lock(&pool->lock);	
		while (pool->queue->size == 0 && !pool->shutdown) {
			thread_cond_wait(&pool->notify, &pool->lock);
		}
		if (pool->shutdown && pool->queue->size == 0) {
			thread_mutex_unlock(&pool->lock);
			break;
		}
		task_t *task = dequeue(pool->queue);
		thread_mutex_unlock(&pool->lock);
		if (task) {
			task->func(task->args);
			free(task);
		}
	}
}

threadpool_t* threadpool_create(int thread_count){
	threadpool_t* pool = malloc(sizeof(threadpool_t));
	if (!pool) return NULL;
	pool->shutdown = 0;
	pool->thread_count = thread_count;
	pool->queue = malloc(sizeof(tasks_t));
	if (!pool->queue) {free(pool), return NULL;}
	queue_init(pool->queue);
	pool->threads = malloc(sizeof(thread_t)*thread_count);
	if (!pool->threads) {free(pool); return NULL;}
	thread_mutex_init(&pool->lock);
	thread_cond_init(&pool->notify);

	for (int i=0; i<thread_count; i++) {
		int	re = thread_create(&pool->threads[i], worker_func, pool);
		if (re == -1) {
			printf("unable to create thread\n");
			free(pool->threads);
			free(pool->queue);
			free(pool);
			return NULL;
		}
	}
	return pool;
}

int threadpool_submit(threadpool_t *pool, void (*func)(void *), void* args){
	task_t *task = malloc(sizeof(task_t));
	if (!task) return -1;
	task->func = func;
	task->args = args;
	task->next = NULL;
	
	thread_mutex_lock(&pool->lock);	
	if (pool->shutdown) {
		free(task);
		thread_mutex_unlock(&pool->lock);
		return -1;
	}
	enqueue(task, pool->queue);
	thread_cond_signal(&pool->notify);
	thread_mutex_unlock(&pool->lock);
	return 0;
}

void threadpool_destroy(threadpool_t *pool) {
	thread_mutex_lock(&pool->lock);
	pool->shutdown = 1;
	thread_cond_broadcast(&pool->notify);
	thread_mutex_unlock(&pool->lock);

	for (int i=0; i<pool->thread_count; i++) {
		thread_join(pool->threads[i]);
	}
	free(pool->threads);
	while (pool->queue->size != 0) {
		task_t *task = dequeue(pool->queue);
		free(task);
	}
	free(pool->queue);
	free(pool);
}	
