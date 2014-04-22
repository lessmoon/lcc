#ifndef __ARRAY_H
#define __ARRAY_H

#include"type.h"

namespace symbols{
    class array:public type{
        public:
            type*of;
            int  size;
            array(const int sz,type*p)
            :type("[]",p -> tag,sz * p->width),of(p),size(sz){

            }
    };//class array
};//namespace lexer

#endif //__ARRAY_H
