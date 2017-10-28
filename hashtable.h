/**
 * @Author: Andy Michel
 * @Date:   2017-10-25T20:14:27-04:00
 * @Last modified by:   floki
 * @Last modified time: 2017-10-25T20:18:04-04:00
 */


#ifndef __hashtable_h__
#define __hashtable_h__

#include<iostream>
#include<unordered_set>

template<typename T>
struct Node{
    bool is_mutable;
    std::unordered_set<T> set;
};

template<typename T>
class HashTable {
    private:
        int size;
        int key;
        bool done;
        bool response;
    public:
        HashTable();
        bool insert(T &val);//returns true if the key was not in the set
        bool remove(T &val);//returns true if the key was in the set
        bool contains();
        void freeze();
        void invoke();
};

int main(void){
    return 0;
}

#endif
