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
            type(const string&s,const lexer::tag_t tag,const int width)
            :lexer::word(s,tag),width(width)
            {}
            virtual ~type(){}
        public:
            //
        private:
            //
    };//class type

    extern type::self_ptr int_,float_,char_,bool_;
};//namespace symbols
#endif // _TYPE_H
