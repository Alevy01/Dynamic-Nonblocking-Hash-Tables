#include<iostream>

enum OPType {
  INS,
  DEL
};

template<typename T>
class HNode {
    private:
        static HNode head;
        //FSet buckets[];
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
