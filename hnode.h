#include<iostream>
#include<unordered_set>

enum OPType {
  INS,
  DEL
};

//FSetOps
class FSetOp {
    public:
        OPType type;
        int key;
        bool done;
        bool response;
        FSetOp(OPType op, int key);
        bool getResponse();
};

//FSetNode
template<typename T>
class FSetNode{
    public:
        bool is_mutable;
        std::unordered_set<T> hash_set;
        FSetNode();
        bool hasMember(T &key);
};


// FSet 
template<typename T>
class FSet{
    public:
        FSetNode<T> * head;
        FSet();
        bool invoke(OPType &op);
        T* freeze();
        bool hasMember(T &key);
        bool getResponse(FSetOp &op);
};


// HNode
template<typename T>
class HNode {
    public:
        static HNode head;  //don't think we actually need this  
        FSet<T> *buckets;
        int size;
        HNode *pred;
        HNode(int size);
        bool insert(T &key);
        bool remove(T &key);
        bool contains(T &key);
        void resize(bool grow);
        bool apply(OPType type, T &key);
        void initBucket(HNode t, int hashIndex);
};

int main(void){
    return 0;
}
