#include<iostream>

enum OPType {
  INS,
  DEL
};


class HNode {
        
    public:
        static HNode head;
        FSet buckets[];
        int size;
        HNode *pred;
        HNode(int size);
        bool insert(int key);
        bool remove(int key);
        bool contains(int key);
        void resize(bool grow);
        bool apply(OPType type, int key);
        void initBucket(HNode t, int hashIndex);
        ~HNode();
};
