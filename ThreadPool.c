#include "ThreadPool.h"
#include <stdlib.h>
#include <pthread.h>
#include "c_data_structures/Queue.h"
#include <stdbool.h>
#include <stdio.h>

void *thread_job(void *arg);
void add_task(ThreadPool *thread_pool, Task *task);

ThreadPool *thread_pool_constructor(int num_threads)
{
    ThreadPool *thread_pool = (ThreadPool *)malloc(sizeof(ThreadPool));
    thread_pool->num_threads = num_threads;
    thread_pool->add_task = add_task;
    thread_pool->active = 1;
    thread_pool->pool = (pthread_t *)malloc(sizeof(pthread_t[num_threads]));
    thread_pool->task_queue = queue_constructor(false, 0);
    thread_pool->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    thread_pool->cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    pthread_mutex_lock(&thread_pool->mutex);
    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&thread_pool->pool[i], NULL, thread_job, thread_pool);
    }
    pthread_mutex_unlock(&thread_pool->mutex);
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
    pthread_mutex_destroy(&thread_pool->mutex);
    pthread_cond_destroy(&thread_pool->cond);
}

Task *task_constructor(void *(*task_fn)(void *arg), void *args)
{
    Task *task = (Task *)malloc(sizeof(Task));
    task->task_fn = task_fn;
    task->args = args;

    return task;
}

void *thread_job(void *arg)
{
    ThreadPool *thread_pool = (ThreadPool *)arg;
    while (thread_pool->active)
    {
        pthread_mutex_lock(&thread_pool->mutex);
        pthread_cond_wait(&thread_pool->cond, &thread_pool->mutex);
        Task *task = (Task *)thread_pool->task_queue.peek(&thread_pool->task_queue);
        Task task_temp;
        task_temp.task_fn = task->task_fn;
        task_temp.args = task->args;
        thread_pool->task_queue.dequeue(&thread_pool->task_queue);
        pthread_mutex_unlock(&thread_pool->mutex);
        if (task_temp.task_fn != NULL)
        {
            task_temp.task_fn(task_temp.args);
        }
    }
    return NULL;
}

void add_task(ThreadPool *thread_pool, Task *task)
{
    pthread_mutex_lock(&thread_pool->mutex);
    thread_pool->task_queue.enqueue(task, sizeof(*task), &thread_pool->task_queue);
    pthread_cond_signal(&thread_pool->cond);
    pthread_mutex_unlock(&thread_pool->mutex);
}