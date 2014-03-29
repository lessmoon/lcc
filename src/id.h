#ifndef _ID_H
#define _ID_H

#include"expr.h"
#include"type.h"

namespace inter{
    class id:public expr{
        public:
            typedef id self;
            typedef self self_ptr;
            typedef symbols::type type;
            typedef type* type_ptr;
            typedef lexer::word word;
            typedef word*       word_ptr;
            
        public:
             int offset; 
        public:
             id(word_ptr i,type_ptr p,const int b)
             :expr(i,p),offset(b)
             {}
    };//class id
};//namespace inter

#endif//_ID_H
