#ifndef _TAB_GEN_H
#define _TAB_GEN_H

#include"parser.h"

#include<vector>
#include<string>

namespace cctabgen{
    enum action{REDUCE,SHIFT,JUSTGO,ERROR};
    struct action_node{                           
        action_do t;
        int where;
        action_node(){t = ERROR;where =0;}
        action_node(action_do t,int where)
        :t(t),where(where){}
    };

    class tabgen{
    public:
        typedef ccparser::result    info;
        typedef cctype::right       right;
        typedef cctype::rightlist   rightlist;
        typedef cctype::prods       prods;
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
        ffsm_t      ffsm;
        table       srtable;

        info*       para;
        
        sym_set     has_empty;
        
        sym_map*    sym_name;
        prods*      ps;
        sym_set*    str_sym;
        type_tab*   svt;
    public:
        tabgen();
        ~tabgen();
    public:
        void        set_para(info* para);
        table*      calculate();
    private:
        bool first_term(const sym_t sym,sym_set& res,
                        sym_set&has_visited);
        void first_term(const sym_seq&syms,sym_set&res);
        void closure(item_list&I);
        void closure_set(item_list&I);
        void clear();
    private:
        void error(const string&info);
    private:
        void addline();
        /**/
    };//class tabgen
};

#endif//_TAB_GEN_H
