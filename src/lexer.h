#ifndef _LEXER_H
#define _LEXER_H
//#include"instream.h"
#include"token.h"
#include"word.h"
#include"real.h"
#include"num.h"
#include"tag.h"
#include"type.h"

#include<map>
#include<string>

namespace lexer
{
    class lexer{
        public:
            typedef std::string                string;
            typedef word*                      word_ptr;
            typedef std::map<string,word_ptr>  w_table;
            typedef char                       elem_t;
            typedef token*                     token_ptr;
            typedef num::num_t                 num_t;
            typedef real::real_t               real_t;
            typedef w_table::value_type        pair;           
            typedef w_table::iterator          tab_iter; 
        public:
            elem_t peek;
            int    lineno;
        public:
            lexer();
            ~lexer();
            void read();
            bool check(const elem_t c);
            token_ptr scan();
        private:
            void readch();
            void reserve(word_ptr word);
            bool readch(const elem_t c);
            w_table words;
      };
};
#endif // _LEXER_H
