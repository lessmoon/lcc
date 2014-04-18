#ifndef _TAB_GEN_H
#define _TAB_GEN_H

#include"parser.h"

namespace cctabgen{
    class tabgen{
    public:
        typedef ccparser::result    info;
        typedef cctype::right       right;
        typedef cctype::rightlist   rightlist;
        typedef cctype::prods       prods;
        typedef cctype::item        item;
        typedef cctype::item_list   item_list;
        typedef cctype::item_set    item_set;
        typedef ccparser::TYPE      TYPE;
        typedef ccparser::result::symbol_table  sym_tab;
        typedef ccparser::result::symbol_type   type_tab;
        typedef ccparser::result::symbol_set    str_tab;
    public:
         
    
    private:
    };//class tabgen
};

#endif//_TAB_GEN_H
