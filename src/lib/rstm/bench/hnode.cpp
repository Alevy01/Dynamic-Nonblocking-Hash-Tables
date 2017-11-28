/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:44:33-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: hnode.cc
 * @Last modified by:   Michel Andy
 * @Last modified time: 2017-11-05T21:11:15-05:00
 */

#include <iostream>
#include "hnode.h"
#include <atomic>
#include <cstdlib>
#include <signal.h>
#include <pthread.h>
#include <api/api.hpp>
#include <common/platform.hpp>
#include <common/locks.hpp>
#include "bmconfig.hpp"
#include "../include/api/api.hpp"


const int THREAD_COUNT = 4;
const int NUM_TRANSACTIONS = 100000;

// shared variable that will be incremented by transactions
int x = 0;

Config::Config() :
    bmname(""),
    duration(1),
    execute(0),
    threads(THREAD_COUNT),
    nops_after_tx(0),
    elements(256),
    lookpct(34),
    inspct(66),
    sets(1),
    ops(1),
    time(0),
    running(true),
    txcount(0)
{
}

Config CFG TM_ALIGN(64);



template<typename T>
HNode<T>::HNode(int size) {
    this->buckets = new FSet<T>[size];
    this->size = size;
    this->pred = NULL;
}

template<typename T>
bool HNode<T>::insert(T &key) {
     // each thread must be initialized before running transactions
    TM_THREAD_INIT();
    bool response = false;
    TM_BEGIN(atomic) 
    {
        response = apply(INS, key);
        if(false/*based on heuristic*/){
            resize(true);
        }
    }
    TM_END;
    TM_THREAD_SHUTDOWN();
    return response;
}

template<typename T>
bool HNode<T>::remove(T &key) {
    bool response = apply(DEL, key);
    if(false /*based on heuristic*/){
        resize(false);
    }
    return response;
}

template<typename T>
FSet<T> HNode<T>::getBucket(int key) {
    return this->buckets[key];
}

template<typename T>
bool HNode<T>::contains(T &key) {
    FSet<T> curr_bucket = this->buckets[key % this->size];
    if(!curr_bucket.getHead()->is_mutable) {
        HNode *prev_hnode = this->pred;
        if(prev_hnode != NULL) {
            curr_bucket = prev_hnode->buckets[key % prev_hnode->size];
        }
    }
    return curr_bucket.hasMember(key);
}

template<typename T>
void HNode<T>::resize(bool grow){
    //calculate new size: grow or shrink
    int new_size = grow ? this->size*2 : this->size/2;
   
    //creating new buckets of at least the size of the old bucket
    //std::atomic<FSet<T> *>new_buckets = new FSet<T>[new_size];
    
    //if(new_buckets->size >= size && grow){
        for(int i=0; i<new_size; i++){
            //migrate each bucket from old to the new
            initBucket(i);
        }
        this->pred = NULL;
        
        //setting the old bucket to null
        //new_buckets.compare_exhange_weak(this->buckets, new_buckets);
    //}
}

template<typename T>
bool HNode<T>::apply(OPType type, T &key) {
    FSetOp<T> fSetOp(type, key);
    while(1) {
        int hash = key % this->size;
        FSet<T> bucket = this->getBucket(hash);
        
        if(bucket.getHead()->getSize() == 0) {
            bucket = initBucket(hash);
        }
        
        if(bucket.invoke(fSetOp)) {
            return fSetOp.getResponse();
        }
    }
    return true;
}

template<typename T>
FSet<T> HNode<T>::initBucket(int i) {
    FSet<T> curr_bucket = this->buckets[i];
    HNode *prev_hnode = this->pred;
    FSet<T> new_bucket;
    std::unordered_set<T> new_set;
    int curr_size = this->size;

    if(curr_bucket.getHead()->getSize() == 0 && prev_hnode != NULL) { 
        int prev_size = prev_hnode->size; 
        if(curr_size == (prev_size*2)) { 
            new_bucket = prev_hnode->buckets[i % prev_size];
            new_set = new_bucket.freeze(); 
        } else { 
            new_bucket = prev_hnode->buckets[i];
            FSet<T> larger_bucket = prev_hnode->buckets[i + curr_size];
            new_set = new_bucket.freeze();
            std::unordered_set<T> tmp_set = larger_bucket.freeze();
            new_set.insert(tmp_set.begin(), tmp_set.end());
        }

        FSet<T> return_set(new_set);
        std::atomic<FSet<T>> b {{this->buckets[i]}};

        if(b.load().getHead()->getSet().size() == 0)
               b.store(return_set);
    }
    return curr_bucket;
}

void* run_thread(HNode<int> hnode, void* i) {
    int j = 0;
    TM_THREAD_INIT();
    TM_BEGIN(atomic)
    {   
        hnode.insert(j);
	j++;
    }
    TM_END; // mark the end of the transaction
    TM_THREAD_SHUTDOWN();
}

int main(void){
    TM_SYS_INIT();

    // original thread must be initalized also
    TM_THREAD_INIT();
    void* args[256];
    pthread_t tid[256];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    for (uint32_t i = 0; i < CFG.threads; i++)
        args[i] = (void*)i;
    
    HNode<int> *hnode = new HNode<int>(5);

    // actually create the threads
    for (uint32_t j = 1; j < CFG.threads; j++) {
        // need to figure out how to call insert here on the hnode.
        // i++ should be the arg into insert
        pthread_create(&tid[j], &attr, &run_thread, &hnode, args[j]);
    }
    
    // all of the other threads should be queued up, waiting to run the
    // benchmark, but they can't until this thread starts the benchmark
    // too...
    run_thread(&hnode, args[0]);

    // everyone should be done.  Join all threads so we don't leave anything
    // hanging around
    for (uint32_t k = 1; k < CFG.threads; k++)
        pthread_join(tid[k], NULL);

    // And call sys shutdown stuff
    TM_SYS_SHUTDOWN();
    std::cout << "Testing" << std::endl;

    return 0;
}

