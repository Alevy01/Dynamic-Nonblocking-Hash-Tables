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
#include <unordered_set>

template<typename T>
class FSetNode{
    public:
        bool is_mutable;
        std::unordered_set<T> m_set;
        FSetNode(){
            this->is_mutable = true;
        }
        
        FSetNode(std::unordered_set<T> &m_set) {
            this->m_set = m_set;
            this->is_mutable = true;
        }
       
        bool remove(T &key){
            auto ret = m_set.erase(key);
            return (ret == this->m_set.end()) ? false : true;
        }

        bool insert(T &key){
            auto ret = m_set.insert(key);
            return ret.second;
        }

        bool hasMember(T &key){
            typename std::unordered_set<T>::const_iterator got = this->m_set.find(key);
            return got == this->m_set.end();
        }

        int getSize(){
            return m_set.size();
        }
};
#endif
