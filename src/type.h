#ifndef _TYPE_H
#define _TYPE_H

#include"word.h"
#include"tag.h"

#include<iostream>

namespace symbols{
    class type: public lexer::word{
        public:
            typedef std::string string;
            typedef type self;
            typedef self* self_ptr;
        public:
            const int width;
        public:
            type(string s,lexer::tag_t tag,int width)
            :lexer::word(s,tag),width(width)
            {}
        public:
            //
        private:
            //
    };//class type

    extern type::self_ptr int_   = new type("int",lexer::tag::BASIC,4),
                          float_ = new type("float",lexer::tag::BASIC,8),
                          char_  = new type("char",lexer::tag::BASIC,4),
                          bool_  = new type("bool",lexer::tag::BASIC,4);
};//namespace symbols
#endif // _TYPE_H
