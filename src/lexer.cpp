#include"lexer.h"

#define CH_IS(C1,C2) ((C1)==(C2))

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

    lexer::token_ptr lexer::scan()
    {
        string buf;
        num_t num_v;
        real_t real_v;
        buf.reserve(MAX_LEXME_STR);
        for(;;readch()){
            if(peek == ' ' || peek == '\t')
                continue;
            else if(peek == '\n')
                lineno++;
            else 
                break;
        }
        
        if(is_digit(peek)){
            do{
                num_v = num_v*10 + peek - '0';
            }while(is_digit(peek));
            return new num(num_v);
        }

        if(is_alpha(peek)||CH_IS(peek,'_')){
            do{
                buf += peek;
                readch();
            }while(is_alpha(peek)||is_digit(peek)||CH_IS(peek,'_'));
            // TODO this -> find(reserve);
            // TODO Return new token();
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
            default:
           ;
        }
        
    }

    void lexer::readch()
    {
        //peek = getchar();
    }
 
    bool lexer::readch(const char c)
    {
        this -> readch();
        if(CH_IS(peek,c))
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
