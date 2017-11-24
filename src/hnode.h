/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:44:59-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: hnode.h
 * @Last modified by:   andy
 * @Last modified time: 2017-11-17T14:04:30-05:00
 */
#include <iostream>
#include "fset.h"
#include <atomic>
#include "threadpool.h"
#include <unordered_set>

template<typename T>
class HNode {
    private:
        FSet<T> *buckets;
        int size;
        HNode *pred;
    public:
        HNode(int size);
        bool insert(T &key);
        bool remove(T &key);
        bool contains(T &key);
        void resize(bool grow);
        bool apply(OPType type, T &key);
        FSet<T> getBucket(int key);
        FSet<T> initBucket(int hashIndex);
};


int main(void){
    // cc::ThreadPool pool;
    // cc::Job j;

    // for(int i=0; i<5; i++)
    //     pool.enqueue([&j](){ std::cout<<"Hello from: "<<std::this_thread::get_id()<<std::endl; j.run();});

    // pool.joinAll();

    HNode<int> *hnode = new HNode<int>(5);
    
    int i = 1;
    hnode->insert(i);

    return 0;
}
