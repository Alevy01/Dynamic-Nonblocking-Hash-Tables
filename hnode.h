#include<iostream>
#include<unordered_set>

enum OPType {
  INS,
  DEL
};

template<typename T>
struct FSetNode{
    bool is_mutable;
    std::unordered_set<T> hash_set;
};

template<typename T>
class FSet{
    private:
        struct FSetNode<T> * head;
    public:
        FSet();
        bool invoke(OPType &op);
        T* freeze();
        bool hasMember(T &key);
        bool getResponse(OPType &op);
};

template<typename T>
class HNode {
    private:
        static HNode head;
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
        void initBucket(HNode t, int hashIndex);
        ~HNode();
};

int main(void){
    return 0;
}
