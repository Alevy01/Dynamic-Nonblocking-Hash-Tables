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

class FSetOp {
    private:
        OPType type;
        int key;
        bool done;
        bool response;
    public:
        FSetOp(OPType op, int key){
            this->type = type;
            this->key = key;
            this->done = false;
            this->response = false;
        }
        bool getResponse(){
            return response;
        };
};
#endif
