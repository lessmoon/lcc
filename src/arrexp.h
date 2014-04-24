#ifndef _ARR_EXPR_H
#define _ARR_EXPR_H

#include"id.h"

namespace inter{
    class arrexp:public expr{
    public:
        expr*idx;
        id*  i;
    public:
        arrexp(id*i,expr* e)
        :expr(NULL,symbols::int_),idx(e),i(i)
        {}

    public:
        void gen()const
        {
            i -> gen();
            this -> emitpush("%ebx");
            idx -> gen();
            this -> emitpop("%ebx");
            std::cout<<"negl %eax\n"
                     <<"leal (%ebx,%eax,4),%ebx\n"
                     <<"movl (%ebx),%eax\n";
        }
    };//class expr
};//namespace inter

#endif//_ARR_EXPR_H
