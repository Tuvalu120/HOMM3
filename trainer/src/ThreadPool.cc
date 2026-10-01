/**
 * @file ThreadPool.cc
 * @author Bartosz Han
 * @brief This file contains definition of class ThreadPool.
 *
 * @note This class and file uses thread pool design pattern implementation from
 * https://www.geeksforgeeks.org/cpp/thread-pool-in-cpp/ .
 */
#include "ThreadPool.h"
#include <cstddef>
#include <functional>
#include <mutex>

using namespace std;

ThreadPool::ThreadPool(size_t n_threads){

    for(size_t i=0; i<n_threads; ++i){
        threads_.emplace_back([this](){

            while(true){
                function<void()> task;

                {
                    unique_lock<mutex> lock(queue_mutex_);

                    condition_variable_.wait(lock, [this]{
                        return !tasks_.empty() || stop_;
                    });

                    if(stop_ && tasks_.empty()){
                        return;
                    }

                    task = std::move(tasks_.front());
                    tasks_.pop();

                }

                task();

                {
                    lock_guard<mutex> lock(queue_mutex_);
                    --n_active_tasks_;
                }

                wait_for_all_cv_.notify_all();

            }


        });
    }
}

ThreadPool::~ThreadPool(){
    {
        unique_lock<mutex> lock(queue_mutex_);
        stop_ = true;
    }
    condition_variable_.notify_all();

    for(auto& thread: threads_){
        thread.join();
    }

}

void ThreadPool::enqueue(function<void()> task){
    {
        unique_lock<mutex> lock(queue_mutex_);
        tasks_.emplace(std::move(task));
        ++n_active_tasks_;
    }

    condition_variable_.notify_one();
}

void ThreadPool::waitForAll(){
    unique_lock<mutex> lock(queue_mutex_);
    wait_for_all_cv_.wait(lock, [this](){return n_active_tasks_.load() == 0 && tasks_.empty();});
}
