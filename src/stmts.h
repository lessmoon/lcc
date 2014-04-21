#ifndef _STMTS_H
#define _STMTS_H
#include"stmt.h"
namespace inter{
class stmts:public stmt{
    public:
        stmts* s1;stmt* s2;
    public:
        stmts(stmts*s1,stmt* s2)
        :s1(s1),s2(s2){}
    public:
        void gen()const
        {
            if(s1 != NULL)
                s1 -> gen();
            if(s2 != NULL)
                s2 -> gen();
        }   
};//class stmts
};//namespace inter
#endif// _STMTS_H
