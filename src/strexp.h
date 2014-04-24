#ifndef _STR_EXP_H
#define _STR_EXP_H

#include"node.h"
#include"str.h"


#include<string>

namespace inter{
    class strexp:public expr{
    public:
        typedef std::string string;
    public:
        lexer::str*         str_tok;
    public:
        strexp(lexer::str*)
        :expr(NULL,symbols::char_),str_tok(str_tok)/*TODO:the type is not right now!*/
        {}

        void gen()const
        {
            int l = this -> newlabel();
            int le = this -> newlabel();
            this -> emitjmp(le,JMP);
            this -> emitlabel(l);
            this -> gen_str();
            this -> emitlabel(le);
            this -> mvlabelto(l,"%eax");
        }
    private:
        void gen_str()const
        {
            const string&str = str_tok -> value;
            this -> emit(".byte ");
            for(int i = 0; i < str.size();i++){
                std::cout<<(int)str[i]<<",";
            }
            this -> emit("0\n");
        }
    };

};//namespace inter

#endif//_STR_EXP_H
