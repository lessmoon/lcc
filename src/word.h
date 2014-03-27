#ifndef __WORD_H
#define __WORD_H
#include<string>

#include"token.h"
#include"tag.h"

namespace lexer{
class word:public token{
    public:
        typedef word    self;
        typedef self*   self_ptr;
    public:
    word(const std::string&lexme,const int tag)
    :token(tag),lexme(lexme){}
    private:
    std::string lexme;
};
  
word::self_ptr and_  = new word("&&",tag::AND),
               or_   = new word("||",tag::OR),
               eq_   = new word("==",tag::EQ),
               ne_   = new word("!=",tag::NE),
               le_   = new word("<=",tag::LE),
               ge_   = new word(">=",tag::GE),
               true_ = new word("true",tag::TRUE),
               false_= new word("t",tag::FALSE),
               temp  = new word("t",tag::TEMP);

};//namespace std

#endif//__WORD_H
