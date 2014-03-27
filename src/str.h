#ifndef _STRING_H_LEXER
#define _STRING_H_LEXER
#include"token.h"
#include"tag.h"

#include<string>

namespace lexer{
    class str:public token{
        public:
            typedef std::string str_t;
            typedef str         self;
            typedef self*       self_ptr;
        public:
            const str_t value;
            str(const str_t&value)
            :token(tag::STR),value(value){}
            virtual ~str(){}
    };
};//namespace lexer

#endif//_STRING_H_LEXER
