/**
 * @Author: Andy Micheli
 * @Date:   2017-10-25T20:14:32-04:00
 * @Last modified by:   floki
 * @Last modified time: 2017-10-25T20:18:16-04:00
 */



#include "hashtable.h"

template<typename T>
HashTable<T>::HashTable(){

}

template<typename T>
bool HashTable<T>::insert(T &val){
    return true;
}

template<typename T>
bool HashTable<T>::remove(T &val){
    return true;
}

template<typename T>
void HashTable<T>::freeze(){

}

template<typename T>
void HashTable<T>::invoke(){

}

template class HashTable<int>;
