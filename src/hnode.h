/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:44:59-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: hnode.h
 * @Last modified by:   floki
 * @Last modified time: 2017-11-29T18:28:33-05:00
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
        HNode *pred;
        int used;
    public:
        int size;
        int num_resize;
        HNode(int size);
        bool insert(T &key);
        bool remove(T &key);
        bool contains(T &key);
        void resize(bool grow);
        bool apply(OPType type, T &key);
        FSet<T> getBucket(int key);
        FSet<T> initBucket(int hashIndex);
};

template<typename T>
class Job {
  private:
    int count, rm, ct;
    HNode<T>* hnode;
  public:
    Job(HNode<T>* hnode, int count=0){
      this->hnode = hnode;
      this->count = 0;
      this->count = rm = ct = count;
    }
    void insert(){
      hnode->insert(count);
      count++;
    }

    void contains(){
      hnode->contains(ct);
      ct++;
    }

    void remove(){
      hnode->remove(rm);
      rm++;
    }
};

void bench(){

}

int main(void){

    cc::ThreadPool pool;
    //cc::Job j;
    int bench = 10000;
    HNode<int> *hnode = new HNode<int>(2);
    clock_t begin = clock();
    Job<int> j(hnode);
    for(int i=0; i<bench*.33; i++)
        pool.enqueue([&j](){j.insert();});

    for(int i=0; i<bench*.34; i++)
        pool.enqueue([&j](){ j.contains();});

    for(int i=0; i<bench*.33; i++)
        pool.enqueue([&j](){ j.remove();});

    pool.joinAll();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Total Elapsed Seconds: ";
    std::cout << elapsed_secs << std::endl;
    std::cout << "Total Resize Operations: ";
    std::cout << hnode->num_resize << std::endl;

     return 0;
}
