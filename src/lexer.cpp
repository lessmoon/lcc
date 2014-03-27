#include"lexer.h"

#define CH_IS(C1,C2) ((C1)==(C2))

namespace lexer{
    const int MAX_LEXME_STR   =   0x255;
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
    :peek(' '),lineno(0){}
    std::string buf;
    buf.reserve(MAX_LEXME_STR);    

    bool lexer::check(const elem_t c)
    {
        readch();
        if(c != peek)
            return false;
        return true;
    }

    token_ptr lexer:: scan()
    {
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
                num = num*10 + peek - '0';
            }while(is_digit(peek));
            //TODO Return new number();
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
};//namespace lexer
