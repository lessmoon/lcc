#include"lexer.h"
#include<cstdio>

#define CH_IS(C1,C2) ((C1)==(C2))
#define CH_IS_NOT(C1,C2) (!CH_IS(C1,C2))
namespace lexer{
    const int MAX_LEXME_STR   =   0x100;
};

namespace {
    bool is_digit(const char c)
    {
        return (c >= '0' && c <= '9');
    }

    bool is_alpha(const char c)
    {
        return (c >= 'A' && c <= 'Z')
                || (c >= 'a' && c <= 'z');
    }
};

namespace lexer{

    lexer::lexer()
    :peek(' '),lineno(0)
    {
        this -> reserve(true_);this -> reserve(false_);
        this -> reserve(new word("if",tag::IF));
        this -> reserve(new word("else",tag::ELSE));
        this -> reserve(new word("while",tag::WHILE));
        this -> reserve(new word("for",tag::FOR));
        this -> reserve(new word("break",tag::BREAK));
        this -> reserve(symbols::int_);this -> reserve(symbols::float_);
        this -> reserve(symbols::char_);this -> reserve(symbols::bool_);
    }

    lexer::~lexer()
    {
        for(tab_iter iter = words.begin();
            iter != words.end();
            iter++)
            delete iter -> second;
        words.clear();
    }

    //stdz::string buf;
   // buf.reserve(MAX_LEXME_STR);

    bool lexer::check(const elem_t c)
    {
        return this -> readch(c);
    }

/*
 * NOTE: this function scan the input buffer and return the token
 * And because of the C++ no-auto do the clean job,So we should delete
 * the token(it is in the heap) manually.But I am working on the memory
 * manager.
 * TODO:create another token manager and do the collection job
 */
    lexer::token_ptr lexer::scan()
    {
        string buf;
        num_t num_v;
        real_t real_v;
        word_ptr w;
        tab_iter iter;
        token_ptr tok;
        buf.reserve(MAX_LEXME_STR);

        for(;;readch()){
            if(CH_IS(peek,' ') || CH_IS(peek , '\t'))
                continue;
            else if(CH_IS(peek , '\n'))
                lineno++;
            else if(CH_IS(peek , '/'))
                if(check('*')){//for the block comment
                    readch();//Just because the peek is ' ' now...
                    do{
                        while(CH_IS_NOT(peek,'*')){
                            if(peek == '\n')
                                lineno++;
                            readch();
                        }
                        readch();
                    }while(CH_IS_NOT(peek,'/'));
                }
                else if(CH_IS(peek,'/')){//for the line comment
                    while(!check('\n'));
                    lineno++;
                }
                else
                    return new token('/');
            else
                break;
        }

        if(is_digit(peek)){
            do{
                num_v = num_v*10 + peek - '0';
                readch();
            }while(is_digit(peek));
            return new num(num_v);
        }
        if(is_alpha(peek)||CH_IS(peek,'_')){
            do{
                buf += peek;
                readch();
            }while(is_alpha(peek)||is_digit(peek)||CH_IS(peek,'_'));
            iter = words.find(buf);
            if(iter != words.end())//If this word is in the table
                return iter -> second;
            w = new word(buf,tag::ID);//Or we should create a new word
            this -> reserve(w);//put it into the table.
            return w;
        }
        if(CH_IS(peek,'\"')){
            loop:
            readch();
            while(CH_IS_NOT(peek,'\"')){
                if(CH_IS(peek,'\\')){
                    readch();
                    switch(peek){
                    case '\\':case '\"':case '\'':
                        buf += peek;
                        break;
                    default:
                        ;//TODO error now.
                    }
                }
                else
                    buf += peek;
                readch();
            }
            if(check('\"'))//string cat.
                goto loop;
            else
                return new str(buf);
        }
        switch(peek){
            case '>':
                if(check('='))
                    return new token(tag::GE);
                else
                    return new token('>');
            case '<':
                if(check('='))
                    return new token(tag::LE);
                else
                    return new token('<');
            case '=':
                if(check('='))
                    return new token(tag::EQ);
                else
                    return new token('=');
            case '!':
                if(check('='))
                    return new token(tag::NE);
                else
                    return new token('!');
            case '+':
                if(check('+'))
                    return new token(tag::INC);
                else
                    return new token('+');
            case '-':
                if(check('-'))
                    return new token(tag::DEC);
                else
                    return new token('-');
            case '|':
                if(check('|'))
                    return new token(tag::OR);
                else
                    return new token('|');
            case '&':
                if(check('&'))
                    return new token(tag::AND);
                else
                    return new token('&');
            //case '':
            default:
           ;
        }
        tok = new token(peek);peek = ' ';
        return tok;
    }

    void lexer::readch()
    {
        peek = getchar_unlocked();
    }

    bool lexer::readch(const char c)
    {
        this -> readch();
        if(CH_IS_NOT(peek,c))
            return false;
        peek = ' ';
        return true;
    }

    void lexer::reserve(word_ptr w)
    {
        words.insert(pair(w -> lexme,w));
        /*Put the word token into the table*/
    }
};//namespace lexer
