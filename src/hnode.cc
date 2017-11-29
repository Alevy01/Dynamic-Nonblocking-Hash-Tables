/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:44:33-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: hnode.cc
 * @Last modified by:   floki
 * @Last modified time: 2017-11-29T17:36:32-05:00
 */

#include <iostream>
#include "hnode.h"
#include <atomic>
#include <cstring>

template<typename T>
HNode<T>::HNode(int size) {
    this->buckets = new FSet<T>[size];
    this->size = size;
    this->pred = NULL;
    this->used = 0;
}

template<typename T>
bool HNode<T>::insert(T &key) {
    bool response = apply(INS, key);
    //simple heuristic for growing
    if(this->used >= (this->size/2)){
        resize(true);
    }
    return response;
}

template<typename T>
bool HNode<T>::remove(T &key) {
    bool response = apply(DEL, key);
    //simple heuristic for shrinking
    if(this->used < (this->size/2)){
        resize(false);
    }
    return apply(DEL, key);
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
    std::cout<<"RESISZE\n";
    int new_size = grow ? this->size*2 : this->size/2;

    //creating new buckets of at least the size of the old bucket
    //std::atomic<FSet<T> *>new_buckets = new FSet<T>[new_size];
    if(grow){
      FSet<T>* newArr = new FSet<T>[new_size];
      std::memcpy(newArr, this->buckets, size * sizeof(FSet<T>));
      delete [] this->buckets;
      this->buckets = newArr;
    }
    //if(new_buckets->size >= size && grow){
        for(int i=0; i<new_size; i++){
            //migrate each bucket from old to the new
            initBucket(i);
        }
        this->pred = NULL;
        this->size = new_size;
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
            if(type == INS){
              used++;
            }
        }else if(bucket.getHead()->getSize() == 1){
          if(type == DEL) {
              used--;
          }
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
