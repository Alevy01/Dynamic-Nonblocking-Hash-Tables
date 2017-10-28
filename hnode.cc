#include <iostream>
#include "hnode.h"

template<typename T>
HNode<T>::HNode(int size) {
   // this->buckets = new FSet[size];
    for(int i = 0; i<size; i++) {
        //this->buckets[i] = new FSet();
    }
    this->size = size;
    this->pred = NULL;
}

template<typename T>
bool HNode<T>::insert(T &key) {return false;}

template<typename T>
bool HNode<T>::remove(T &key) {return false;}

template<typename T>
bool HNode<T>::contains(T &key) {return false;}

template<typename T>
void HNode<T>::resize(bool grow) {}

template<typename T>
bool HNode<T>::apply(OPType type, T &key) {return false;}

template<typename T>
void HNode<T>::initBucket(HNode t, int hashIndex) {}

