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
    bool response = apply(INS, key);
    if(false/*based on heuristic*/){
        resize(true);
    }
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

int main(void){

    std::cout << "Testing" << std::endl;

    // HNode<int> *hnode = new HNode<int>(5);
    
    // int i = 1;
    // hnode->insert(i);

    return 0;
}

