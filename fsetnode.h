/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:42:51-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: fsetnode.h
 * @Last modified by:   Michel Andy
 * @Last modified time: 2017-11-05T21:03:21-05:00
 */

#ifndef _fsetnode_h_
#define _fsetnode_h_
#include<unordered_set>

template<typename T>
class FSetNode{
    public:
        bool is_mutable;
        std::unordered_set<T> hash_set;
        FSetNode(){
            this->is_mutable = true;
            std::unordered_set<T> hash_set;
        }

        bool hasMember(T &key){
            typename std::unordered_set<T>::const_iterator got = this->hash_set.find(key);
            return got == this->hash_set.end();
        }
};
#endif
