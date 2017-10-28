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




// FSet 
template<typename T>
FSet<T>::FSet(){ 
    this->head = new FSetNode<T>();
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
    return this->head->hasMember(key);
}

template<typename T>
bool FSet<T>::getResponse(FSetOp &op){
    return op.getResponse();
}




//FSetNode
template<typename T>
FSetNode<T>::FSetNode() {
    this->is_mutable = true;
    std::unordered_set<T> hash_set;
}

template<typename T>
bool FSetNode<T>::hasMember(T &key) {
    typename std::unordered_set<T>::const_iterator got = this->hash_set.find(key);
    return got == this->hash_set.end();
}


//FSetOP
FSetOp::FSetOp(OPType type, int key) {
    this->type = type;
    this->key = key;
    this->done = false;
    this->response = false;
}

bool FSetOp::getResponse() {
    return this->response;
}