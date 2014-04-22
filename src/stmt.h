#ifndef _STMT_H
#define _STMT_H


#include"node.h"
#include"decls.h"

namespace inter{
class stmt:public node{
    public:
    void gen()const{}
};//class stmt

struct block:public stmt{
    public:
        inter::decls*ds;
        inter::decls*ss;
    public:
        void gen()const
        {
            if(ds)
                ds -> gen();
            if(ss)
                ss -> gen();
        }
};//struct block
};//namespace inter
#endif//_STMT_H
