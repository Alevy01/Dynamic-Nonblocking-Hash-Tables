/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-11-16T20:38:44-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: threadpool.h
 * @Last modified by:   andy
 * @Last modified time: 2017-11-17T12:17:28-05:00
 */
#ifndef __thread_pool_h__
#define __thread_pool_h__
#include <iostream>
#include <mutex>
#include <atomic>
#include <functional>
#include <condition_variable>
#include <vector>
#include <thread>
#include <deque>

namespace cc {

class ThreadPool {
    private:
        std::atomic_int job_count;
        std::atomic_bool exited;
        std::atomic_bool exit_signal;
        std::condition_variable job_signal;
        std::condition_variable wait_var;
        std::deque<std::function<void(void)>>  queue;
        std::vector<std::thread> threads;
        std::mutex terminate_mtx;
        std::mutex queue_mtx;
        void assignJob();
        std::function<void(void)> processJob();
    public:
        ThreadPool(int size);
        void enqueue(std::function<void(void)> job);
        int  jobCount();
        void joinAll();
        ~ThreadPool();
};
}

int main(void){
    cc::ThreadPool pool(5);

    for(int i=0; i<5; i++)
        pool.enqueue([](){ std::cout<<"Hello from: "<<std::this_thread::get_id()<<std::endl; });

    pool.joinAll();
    return 0;
}
#endif
