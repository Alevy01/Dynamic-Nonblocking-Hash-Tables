/**
 * @Author: Michel Andy, Levy Adam
 * @Date:   2017-11-05T20:43:08-05:00
 * @Email:  Andymic12@gmail.com
 * @Project: Dynamic-Nonblocking-Hash-Table
 * @Filename: fsetop.h
 * @Last modified by:   Michel Andy
 * @Last modified time: 2017-11-05T21:03:07-05:00
 */

#ifndef _fsetop_h_
#define _fsetop_h_
#include "optype.h"

template<typename T>
class FSetOp {
    private:
        OPType otype;
        T key;
        bool done;
        bool response;
    public:
        FSetOp(OPType otype, int key){
            this->otype = otype;
            this->key = key;
            this->done = false;
            this->response = false;
        }
        
        bool isDone(){
            return done;
        }
        
        OPType getOpt(){
            return otype;
        }
        
        void setKey(T &key){
            this->key = key;
        }
        
        T getKey(){
            return key;
        }

        void setResponse(bool val){
            response = val;
        }

        T getResponse(){
            return response;
        };

        void setStatus(bool val){
            done = val;
        }

        bool getStatus(){
            return done;
        }
};
#endif
