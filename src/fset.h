/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:45:06-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: fset.h
 * @Last modified by:   Michel Andy
 * @Last modified time: 2017-11-05T21:03:39-05:00
 */

#ifndef _fset_h_
#define _fset_h_
#include "fsetop.h"
#include "fsetnode.h"

template<typename T>
class FSet{
    public:
        FSetNode<T> * head;
        FSet(){
            this->head = new FSetNode<T>();
        }

        FSet(std::unordered_set<T> hash_set){
            this->head = new FSetNode<T>(hash_set);
        }

        bool invoke(OPType &op){
            return true;
        }

        T* freeze(){
            return new T;
        }

        bool hasMember(T &key){
            return this->head->hasMember(key);
        }

        bool getResponse(FSetOp &op){
            return op.getResponse();
        }
};
#endif
