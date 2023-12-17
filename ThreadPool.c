#include "ThreadPool.h"
#include <stdlib.h>
#include <pthread.h>
#include "c_data_structures/Queue.h"
#include <stdbool.h>

void *thread_job(void *arg);
void add_task(ThreadPool *thread_pool, Task task);

ThreadPool thread_pool_constructor(int number_of_threads)
{
    ThreadPool thread_pool;
    thread_pool.number_of_threads = number_of_threads;
    thread_pool.active = 1;
    thread_pool.pool = (pthread_t *)malloc(sizeof(pthread_t) * number_of_threads);
    thread_pool.task_queue = queue_constructor(false, 0);
    for (int i = 0; i < number_of_threads; i++)
    {
        pthread_create(&thread_pool.pool[i], NULL, thread_job, NULL);
    }
    thread_pool.lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    thread_pool.cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

    return thread_pool;
}

void thread_pool_destructor(ThreadPool *thread_pool)
{
    thread_pool->active = 0;
    pthread_cond_broadcast(&thread_pool->cond);
    for (int i = 0; i < thread_pool->num_threads; i++)
    {
        pthread_join(thread_pool->pool[i], NULL);
    }
    free(thread_pool->pool);
    queue_destructor(&thread_pool->task_queue);
    pthread_mutex_destroy(&thread_pool->lock);
    pthread_cond_destroy(&thread_pool->cond);
}

Task task_constructor(void (*task_fn)(void *arg), void *arg)
{
    Task task;
    task.task_fn = task_fn;
    task.arg = arg;

    return task;
}

void *thread_job(void *arg)
{
    ThreadPool *thread_pool = (ThreadPool *)arg;
    while (thread_pool->active)
    {
        pthread_mutex_lock(&thread_pool->lock);
        pthread_cond_wait(&thread_pool->cond, &thread_pool->lock);
        Task task = *(Task *)thread_pool->task_queue.peek(&thread_pool->task_queue);
        thread_pool->task_queue.dequeue(&thread_pool->task_queue);
        pthread_mutex_unlock(&thread_pool->lock);
        if (task.task_fn != NULL)
        {
            task.task_fn(task.arg);
        }
    }
    return NULL;
}

void add_task(ThreadPool *thread_pool, Task task)
{
    pthread_mutex_lock(&thread_pool->lock);
    thread_pool->task_queue.enqueue(&task, sizeof(Task), &thread_pool->task_queue);
    pthread_cond_signal(&thread_pool->cond);
    pthread_mutex_unlock(&thread_pool->lock);
}