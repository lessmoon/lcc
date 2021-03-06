#ifndef _LEXER_H
#define _LEXER_H
//#include"instream.h"
#include"token.h"
#include"word.h"
#include"real.h"
#include"num.h"
#include"tag.h"
#include"type.h"
#include"str.h"
#include"iol.h"
#include"token_storage.h"

#include<map>
#include<string>

namespace lexer
{
    class lexer{
        public:
            typedef lexer                      self;
            typedef self*                      self_ptr;
            typedef std::string                string;
            typedef word*                      word_ptr;
            typedef std::map<string,word_ptr>  w_table;
            typedef system::elem_t             elem_t;
            typedef token*                     token_ptr;
            typedef num::num_t                 num_t;
            typedef real::real_t               real_t;
            typedef w_table::value_type        pair;
            typedef w_table::iterator          tab_iter;
            typedef system::iol                iol;
            typedef iol*                       iol_ptr;
            typedef token_storage              tok_st;
            typedef tok_st*                    tok_st_ptr;
        public:
            elem_t  peek;
            static int     lineno;
        private:
            iol_ptr sys_io;
        public:
            lexer(iol_ptr sys_io);
            ~lexer();
            void read();
            bool check(const elem_t c);
            token_ptr scan();
        private:
            void readch();
            void reserve(word_ptr word);
            bool readch(const elem_t c);
            token_ptr store(token_ptr tpr)
            {
                return this -> storage.push(tpr);
            }
        private:
            tok_st storage;
            w_table words;
      };
};
#endif // _LEXER_H
