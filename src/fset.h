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
#include <atomic>
#include <mutex>
#include "optype.h"

template<typename T>
class FSet{
    private:
        FSetNode<T> * head;
        //std::mutex fset_mtx;
        //std::mutex read_mtx;
        bool _mutable;
    public:
        FSet(){
            this->head = new FSetNode<T>();
            _mutable = true;
        }

        FSet(std::unordered_set<T> hash_set){
            this->head = new FSetNode<T>(hash_set);
            _mutable = true;
        }

        bool invoke(FSetOp<T> &fop){
            //std::lock_guard<std::mutex> lck(fset_mtx);
            //check whether FSet is mutable and 
            //the operation was not applied before
            if(_mutable || fop.isDone()){
                OPType opt = fop.getOpt();
                bool status = false;
                
                if(opt == INS){
                  status = head->insert(fop.getKey());  
                 
                  if(!status)
                     std::runtime_error("Duplicate key");

                }else if(opt == DEL){
                  status = head->remove(fop.getKey());
                  
                  if(!status)
                   std::cerr<<"Key was not found\n";

                }else{
                    std::invalid_argument("Unsupported OPType");
                }
                fop.setResponse(status);
                fop.setStatus(true);
            }

            return fop.getStatus;
        }

        std::unordered_set<T> freeze(){
            //std::lock_guard<std::mutex> lck(fset_mtx) ;
            _mutable = false;
            return this->head->getSet();
        }

        bool hasMember(T &key){
            //std::lock_guard<std::mutex> lck(read_mtx) ;
            return this->head->hasMember(key);
        }

        bool getResponse(FSetOp<T> &op){
            //std::lock_guard<std::mutex> lck(fset_mtx) ;
            return op.getResponse();
        }

        FSetNode<T>* getHead(){
            return head;
        }
};
#endif
