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
             static int esp_offset;
        public:
             id(word_ptr i,type_ptr p,const int b)
             :expr(i,p),offset(b)
             {}

            void gen()const
            {
                std::cout<<"movl "<<((esp_offset - offset)*4)<<"(%esp),%eax\n";
                std::cout<<"leal %esp,%ebx\n";
                std::cout<<"addl "<<((esp_offset - offset)*4)<<",%ebx\n";
            }
    };//class id
};//namespace inter

#endif//_ID_H
