#ifndef _CODE_GEN_
#define _CODE_GEN_

#include"parser.h"
#include"cctype.h"
#include"tabgen.h"

namespace cccodegen{
    class codegen{
    public:
        typedef system::iol         iol;
        typedef ccparser::result    info;
        typedef cctype::right       right;
        typedef cctype::rightlist   rightlist;
        typedef cctype::prods       prods;
        typedef prods::prod         prod;
        typedef cctabgen::action_node   action_node;
        typedef cctype::item        item;
        typedef cctype::item_list   item_list;
        typedef cctype::item_set    item_set;
        typedef cctype::sym_t       sym_t;
        typedef ccparser::TYPE      TYPE;
        typedef ccparser::result::symbol_table  sym_map;/*map<str,sym_t>*/
        typedef ccparser::result::symbol_type   type_tab;/*vector<TYPE>*/
        typedef ccparser::result::symbol_set    sym_set;/*set<sym_t>*/
        typedef std::vector<sym_t>              sym_seq;
        typedef std::vector<sym_set>            ffsm_t;         
        typedef std::vector<action_node>        tab_line;
        typedef std::vector<tab_line>           table;
        typedef std::string                     string;

    private:
        info*       para;
        sym_map*    def_tab;
        type_tab*   svt;
        sym_set*    str_sym;
        prods*      ps;
        table*      tab; 
    public:
        codegen();
        ~codegen();
    public:
        /*
         *   set the calculate parameters and action table
         */
        void set_para(info*para,table*tab);
        /*
         *   generator code
         */
        void gen_code();
    private:
        void gen_shift();
        void gen_sym_def();
        void gen_sym_map();
        void gen_reduce();
        void gen_table();
        void gen_space(const int num);
        void gen_include(const string&filename);
        void gen_macro(const string&name,const int value);
        void gen_prod(const prod&p);
    };//class codegen

};//namespace codegen
#endif //_CODE_GEN_
