#ifndef _PARSER_H
#define _PARSER_H

#include"lexer.h"
#include"cctype.h"
#include"tag.h"
#include"word.h"

#include<set>
#include<string>

namespace ccparser{
    enum TYPE{UNKNOWN = -1,VAR,TERM};

    struct result{
    public:
        typedef cctype::sym_t       sym_t;
        typedef cctype::prods       prods;
        typedef lexer::token        token;
        typedef lexer::lexer        lexer;
        typedef std::string          string;
        typedef std::vector<TYPE>   symbol_type;
        typedef std::set<sym_t>     symbol_set;
        typedef std::map<string,sym_t> symbol_table;
        typedef symbol_table::iterator iterator;
        typedef cctype::prods::prod  prod_t;
    public:
        symbol_table  def_table;
        symbol_type   type_table;
        symbol_set    start_symbols;
        prods         productions;
    public:
        const string&get_name(const sym_t s)
        {
            const static string res = "";
            iterator iter = def_table.begin();
            while(iter != def_table.end()){
                if(iter -> second == s)
                    return iter -> first;
                iter++;
            }
            return res;//just for safety we use const static string
        }

        const prod_t&get_prod(const int idx)const
        {
            return productions.get_prod(idx);
        }

    };//result

    /*
     *   NOTE:Don't delete the class unless you wouldn't
     *        use the result it returned anymore!It will delete the result
     *        automatically.
     */
    class parser{
    public:
        typedef cctype::sym_t       sym_t;
        typedef cctype::right       right;
        typedef cctype::prods       prods_t;
        typedef lexer::token        token;
        typedef lexer::lexer        lexer;
        typedef std::string          string;
        typedef std::vector<TYPE>   symbol_type;
        typedef std::set<sym_t>     symbol_set;
        typedef std::map<string,sym_t> symbol_table;
        typedef symbol_table::iterator iterator;
        typedef ::lexer::tag_t      tag_t;
    private:
        symbol_table*   def_table;
        symbol_type*    type_table;
        symbol_set*     start_symbols;
        prods_t*        productions;
        lexer*          lex;
        token*          look;
        result*         res;
    public:
        /*constructor*/
        parser(lexer* lex);
        /*destructor*/
        ~parser();
    private:
        /*read a token from the input buffer*/
        void read();
        /*match tag,if not matched throw error*/
        bool match(const tag_t tag);
        /*throw error*/
        void error(const std::string&info);
        /*
         * May result in exception!
         * just for conveniece,convert the look token to string
         */
        string toString();
    private:
        /*define a symbol,if it's existed throw error*/
        void do_def(const string& name,const TYPE t);
        /*get type of the */
        TYPE sym_type(const string& name)const;
    public:
        /*top-down parser*/
        /*parser&generate result*/
        result*  stmts();
    private:
        /*get the start symbols*/
        void begin();
        /*get the definations of symbols*/
        void def();
        /*get productions*/
        void prods();
        /*get single production*/
        right* prod();
    };//class parser
};//namespace par

#endif//_PARSER_H
