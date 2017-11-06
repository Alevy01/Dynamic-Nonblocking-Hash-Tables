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
        HNode *pred = this->pred;
        if(pred != NULL) {
            curr_bucket = pred->buckets[key % pred->size];
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
void HNode<T>::initBucket(HNode t, int hashIndex) {}
