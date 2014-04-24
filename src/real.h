#ifndef _TOKEN_REAL
#define _TOKEN_REAL

#include"token.h"
#include"tag.h"

#include<string>

namespace lexer{
class real:public token{
    public:
        typedef double real_t;
        typedef real  self;
        typedef self* self_ptr;
        typedef std::string string;
    public:
        const real_t value;
        real(const real_t value)
        :token(tag::REAL),value(value){}
        virtual ~real(){} 
    private:
    };
};//namespace lexer

#endif//_TOKEN_REAL
