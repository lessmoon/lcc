#ifndef _TOKEN_H
#define _TOKEN_H
#include"tag.h"
    namespace lexer{
        struct token{
            const tag_t tag;
            token(const tag_t t):tag(t){}
            virtual ~token(){}
        };

        struct id : public token{

        };
    };
#endif // _TOKEN_H
