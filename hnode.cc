#include <iostream>
#include "hnode.h"

template<typename T>
HNode<T>::HNode(int size) {
    this->buckets = new FSet<T>[size];
    for(int i = 0; i<size; i++) {
        this->buckets[i] = new FSet<T>();
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

template<typename T>
FSet<T>::FSet(){

}

template<typename T>
bool FSet<T>::invoke(OPType &op){
    return true;
}

template<typename T>
T* FSet<T>::freeze(){
    return new T;
}

template<typename T>
bool FSet<T>::hasMember(T &key){
    return true;
}

template<typename T>
bool FSet<T>::getResponse(OPType &op){
    return true;
}

