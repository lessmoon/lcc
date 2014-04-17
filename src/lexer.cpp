#include"lexer.h"

#define CH_IS(C1,C2) ((C1)==(C2))
#define CH_IS_NOT(C1,C2) (!CH_IS(C1,C2))

namespace lexer{

    lexer::lexer(iol_ptr sys_io)
    :peek(' '),lineno(0),sys_io(sys_io)
    {
        /*reserve TERM and VAR*/
        this -> reserve(new word("var",tag::VAR));
        this -> reserve(new word("term",tag::TERM));
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
 * And these class will clean the token memory after being deleted.
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
        /*Put the word token into the table*/
        words.insert(pair(w -> lexme,w));
    }
};//namespace lexer
