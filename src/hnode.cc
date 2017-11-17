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

template<typename T>
HNode<T>::HNode(int size) {
    this->buckets = new FSet<T>[size];
    this->size = size;
    this->pred = NULL;
}

template<typename T>
bool HNode<T>::insert(T &key) {
    return apply(INS, key);
}

template<typename T>
bool HNode<T>::remove(T &key) {
    return apply(DEL, key);
}

template<typename T>
FSet<T> HNode<T>::getBucket(int key) {
    return this->buckets[key];
}

template<typename T>
bool HNode<T>::contains(T &key) {
    FSet<T> curr_bucket = this->buckets[key % this->size];
    if(!curr_bucket.head->is_mutable) {
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
    std::atomic<FSet<T> *>new_buckets = new FSet<T>[new_size];
    
    if(new_buckets->size >= size && grow){
        for(int i=0; i<new_buckets->size -1; i++){
            //migrate each bucket from old to the new
            initBucket(new_buckets, i);
        }
        
        //setting the old bucket to null
        new_buckets.compare_exhange_weak(this->buckets, new_buckets);
    }
}

template<typename T>
bool HNode<T>::apply(OPType type, T &key) {
    FSetOp *fSetOp = new FSetOp(type, key);
    while(1) {
        FSet<T> bucket = this->buckets[key % this->size];
        if(bucket == NULL) {
            bucket = initBucket(this, key % this->size);
        }
        if(bucket.invoke(fSetOp)) {
            return fSetOp->getResponse();
        }
    }
}

template<typename T>
FSet<T> HNode<T>::initBucket(HNode t, int i) {
    FSet<T> curr_bucket = t->buckets[i];
    HNode *prev_hnode = t.pred;
    FSet<T> new_bucket;
    FSet<T> new_set;
    int prev_size = prev_hnode->size;
    int curr_size = t->size;

    if(curr_bucket == NULL && prev_hnode != NULL) {        
        if(curr_size == (prev_size*2)) { 
            new_bucket = prev_hnode->buckets[i % prev_size];
            new_set = new_bucket.freeze(); 

        } else { 
            new_bucket = prev_hnode->buckets[i];
            FSet<T> larger_bucket = prev_hnode->buckets[i + curr_size];
            new_set = new_bucket.freeze();
            FSet<T> tmp_set = larger_bucket.freeze();
            new_set.insert(tmp_set.begin(), tmp_set.end());
        }
        FSet<T> *return_set = new FSet<T>(new_set);
        std::atomic<FSet<T> > b = t.buckets[i];
        b.compare_exchange_weak(NULL, return_set);
    }
    return t.buckets[i];
}

