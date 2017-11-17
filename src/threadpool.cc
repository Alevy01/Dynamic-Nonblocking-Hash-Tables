/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-11-16T21:08:46-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: threadpool.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-11-17T12:15:45-05:00
 */
#include "threadpool.h"

namespace cc {
ThreadPool::ThreadPool(int size) {
    threads.resize( size );
    threads.shrink_to_fit();
    exited = false;
    exit_signal = false;
    job_count = 0;
    for( auto &thread : threads )
        thread = std::thread( [this]{ this->assignJob(); } );
}

void ThreadPool::assignJob() {
    while( !exit_signal ){
        processJob()();
        job_count--;
        wait_var.notify_one();
    }
}

std::function<void(void)> ThreadPool::processJob() {
    std::function<void(void)> result;
    std::unique_lock<std::mutex> lck(queue_mtx);

    job_signal.wait(lck, [this]() -> bool {return queue.size() || exit_signal; });

    if( !exit_signal ) {
        result = queue.front();
        queue.pop_front();
    }else{
        result = []{};
        job_count++;
    }
    return result;
}

void ThreadPool::enqueue( std::function<void(void)> job ) {
    std::lock_guard<std::mutex> lck( queue_mtx ) ;
    queue.push_back( job );
    job_count++;
    job_signal.notify_one();
}

int ThreadPool::jobCount() {
    std::lock_guard<std::mutex> lck( queue_mtx );
    return queue.size();
}

void ThreadPool::joinAll() {
    if( !exited ) {
        //waits for queue to be empty
        if( job_count > 0 ) {
            std::unique_lock<std::mutex> lck( terminate_mtx );
            wait_var.wait( lck, [this]{ return this->job_count == 0; } );
            lck.unlock();
        }

        exit_signal = true;
        job_signal.notify_all();

        for( auto &thread : threads )
            if( thread.joinable() )
                thread.join();

        exited = true;
    }
}

ThreadPool::~ThreadPool(){
    //calls all threads to exit on destruction
    joinAll();
}
}
