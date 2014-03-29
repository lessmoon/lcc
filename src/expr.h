#ifndef _EXPR_H
#define _EXPR_H

#include"node.h"
#include"token.h"
#include"type.h"

#include<string>

namespace inter{
    class expr:public node{
        public:
            typedef expr        self;
            typedef self*       self_ptr;
            typedef std::string string;
            typedef lexer::token token;
            typedef symbols::type type;
            typedef token*     tok_ptr;
            typedef type*     type_ptr;
        public:
            tok_ptr     op;
            type_ptr    t;
        public:
            expr(tok_ptr op,type_ptr t)
            :op(op),t(t)
            {}
            virtual ~expr()
            {}
        public:
            self_ptr gen()
            {
                return this;
            }
            self_ptr reduce()
            {
                return this;
            }
    };//class expr
};//namespace inter

#endif //_EXPR_H
