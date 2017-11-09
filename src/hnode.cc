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
void HNode<T>::resize(bool grow) {}

template<typename T>
bool HNode<T>::apply(OPType type, T &key) {
    std::cout << type << std::endl;
    std::cout << key << std::endl;
    return false;
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
        if(curr_size == (prev_size*2)) { // If we're growing
            new_bucket = prev_hnode->buckets[i % prev_size];
            new_set = new_bucket.freeze(); // freeze still needs to be implemented
        } else { //If we're shrinking
            new_bucket = prev_hnode->buckets[i];
            FSet<T> larger_bucket = prev_hnode->buckets[i + curr_size];
            new_set = new_bucket.freeze();
            FSet<T> tmp_set = larger_bucket.freeze();
            new_set.insert(tmp_set.begin(), tmp_set.end());
        }
        FSet<T> *return_set = new FSet<T>(new_set);
        std::atomic<FSet<T>> b = t.buckets[i];
        b.compare_exchange_weak(NULL, return_set);
    }
    return t.buckets[i];
}
