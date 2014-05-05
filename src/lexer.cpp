#include"lexer.h"
#include<cstdio>

#define CH_IS(C1,C2) ((C1)==(C2))
#define CH_IS_NOT(C1,C2) (!CH_IS(C1,C2))
#define HEX_CONV(C) ((C>='0'&&C<='9')?(C-'0'):((C>='a'&&C<='f')?(C-'a'+0xa):(C-'A'+0xa)))
#define OCT_CONV(C) ((C) - '0')
#define DO_OCT_CONV(C,peek) {C <<= 3; \
                             C |= OCT_CONV(peek);}

namespace lexer{
    const int MAX_LEXME_STR   =   0x100;
};

namespace {
    inline bool is_digit(const char c)
    {
        return (c >= '0' && c <= '9');
    }

    inline bool is_alpha(const char c)
    {
        return (c >= 'A' && c <= 'Z')
                || (c >= 'a' && c <= 'z');
    }

    inline bool is_hex(const char c)
    {
        return is_digit(c)|| (c >= 'a' && c <= 'f');
    }

    inline bool is_oct(const char c)
    {
        return (c >= '0' && c <= '7');
    }
};

namespace lexer{
    int lexer::lineno = 1;

    lexer::lexer(iol_ptr sys_io)
    :peek(' '),sys_io(sys_io)
    {
        this -> reserve(true_);this -> reserve(false_);
        this -> reserve(new word("if",tag::IF));
        this -> reserve(new word("else",tag::ELSE));
        this -> reserve(new word("while",tag::WHILE));
        this -> reserve(new word("for",tag::FOR));
        this -> reserve(new word("do",tag::DO));
        this -> reserve(new word("break",tag::BREAK));
        this -> reserve(new word("printf",tag::PRINTF_FUNC));
        this -> reserve(new word("return",tag::RETURN));
        this -> reserve(new word("main",tag::MAIN_ID));
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
        storage.clear();
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
        string buf = "";
        num_t num_v = 0;
        real_t real_v = 0;
        real_t real_t = 0.1;
        word_ptr w = NULL;
        tab_iter iter;
        token_ptr tok = NULL;
        char c;
        buf.reserve(MAX_LEXME_STR);

        for(;;readch()){
            if(CH_IS(peek,' ') || CH_IS(peek , '\t') || CH_IS(peek , '\r'))
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
            if(CH_IS_NOT(peek,'.'))
                return store(new num(num_v));
            readch();
            while(is_digit(peek)){
                real_v += real_t*(peek - '0');
                real_t /= 10;
                readch();
            }
            //TODO if has exponent
            return store(new real(num_v + real_v));
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
            next:
            while(CH_IS_NOT(peek,'\"')){
                c = 0;
                if(CH_IS(peek,'\\')){
                    readch();
                    switch(peek){
                    case '\\':case '\"':case '\'':
                        buf += peek;
                        break;
                    case 'n':
                        buf += '\n';
                        break;
                    case 't':
                        buf += '\t';
                        break;
                    case 'a':
                        buf += '\a';
                        break;
                    case 'r':
                        buf += '\r';
                        break;
                    /*case '0':
                        /**
                        buf += '\0';*/
                       // break;
                    case 'b':
                        buf += '\b';
                        break;
                    case 'v':
                        buf += '\v';
                        break;
                    case 'x'://it is the two hex digits
                        readch();
                        if(!is_hex(peek))
                            ;//TODO the error check
                        c = HEX_CONV(peek)<<4;
                        readch();
                        if(!is_hex(peek))
                            ;//TODO the error check
                        c |= HEX_CONV(peek);
                        buf += c;
                        break;
                    default:
                        if(is_oct(peek)){
                            do{
                                c = OCT_CONV(peek);
                                readch();
                                if(!is_oct(peek))
                                    goto next;
                                DO_OCT_CONV(c,peek);
                                readch();
                                if(!is_oct(peek))
                                    goto next;
                                DO_OCT_CONV(c,peek);
                            }while(false);
                            dest:
                            buf += c;
                            //means goto gate;
                        }else{
                            buf += '\\';
                          /*TODO *MAYBE* it should be an error here.
                           *     Will check it in the C Gramma Guide.
                           */
                            continue;//means goto next
                        }
                    }
                }
                else
                    buf += peek;
                gate:
                readch();
            }
            if(check('\"'))//string cat.
                goto loop;
            else
                return store(new str(buf));
        }
        switch(peek){
            case '>':
                if(check('='))
                    return store(new token(tag::GE));
                else
                    return store(new token('>'));
            case '<':
                if(check('='))
                    return store(new token(tag::LE));
                else
                    return store(new token('<'));
            case '=':
                if(check('='))
                    return store(new token(tag::EQ));
                else
                    return store(new token('='));
            case '!':
                if(check('='))
                    return store(new token(tag::NE));
                else
                    return store(new token('!'));
            case '+':
                if(check('+'))
                    return store(new token(tag::INC));
                else
                    return store(new token('+'));
            case '-':
                if(check('-'))
                    return store(new token(tag::DEC));
                else
                    return store(new token('-'));
            case '|':
                if(check('|'))
                    return store(new token(tag::OR));
                else
                    return store(new token('|'));
            case '&':
                if(check('&'))
                    return store(new token(tag::AND));
                else
                    return store(new token('&'));
            //case '':
            default:
           ;
        }
        tok = new token(peek);peek = ' ';
        return store(tok);
    }

    void lexer::readch()
    {
        peek = sys_io -> readch();
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
