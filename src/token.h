#ifndef _TOKEN_H
#define _TOKEN_H
#include"tag.h"
#include<sstream>
#include<string>
    namespace lexer{
        struct token{
            const tag_t tag;
            token(const tag_t t):tag(t){}
            virtual ~token(){}
            virtual std::string toString()
            {
                std::string str;
                std::stringstream ss;
                ss<<tag;
                ss>>str;
                return str;
            }
        };

        struct id : public token{

        };
    };
#endif // _TOKEN_H
