/**
 * @file ThreadPool.h
 * @author Bartosz Han
 * @brief This file contains ThreadPool class,
 * which implements thread pool design pattern.
 *
 */

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>


/**
 * @brief Thread pool for running tasks concurrently.
 *
 * ThreadPool manages a fixed number of worker threads that execute tasks from a queue.
 * Tasks can be enqueued using enqueue(), and waitForAll() blocks until all tasks are finished.
 */
class ThreadPool{

private:

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queue_mutex_;
    std::condition_variable condition_variable_;
    std::atomic<int> n_active_tasks_ = 0;
    std::condition_variable wait_for_all_cv_;
    std::mutex wait_for_all_mutex_;
    bool stop_ = false;

public:

    /**
     * @brief Construct a new ThreadPool object.
     * @param n_threads Number of worker threads to create.
     */
    ThreadPool(std::size_t n_threads = std::thread::hardware_concurrency());

    /**
     * @brief Enqueue a new task to be executed by the thread pool.
     * @param task The task to execute.
     */
    void enqueue(std::function<void()> task);

    /**
     * @brief Block until all tasks have been completed.
     */
    void waitForAll();

    ~ThreadPool();

};

#endif