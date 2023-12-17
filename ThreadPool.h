#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "c_data_structures/Queue.h"

typedef struct Task
{
    void (*task_fn)(void *);
    void *args;
} Task;

typedef struct ThreadPool
{
    int num_threads;
    int active;
    Queue task_queue;
    pthread_t *pool;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    void (*add_task)(struct ThreadPool *thread_pool, Task task);
} ThreadPool;

ThreadPool thread_pool_constructor(int num_threads);
Task task_constructor(void (*task_fn)(void *), void *args);

#endif // THREADPOOL_H