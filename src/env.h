#ifndef _ENV_H
#define _ENV_H

#include"id.h"
#include"token.h"

#include<map>

namespace symbols {
    class env{
    public:
        typedef env                      self;
        typedef self*                    self_ptr;
        typedef lexer::token             token;
        typedef token*                   tok_ptr;
        typedef inter::id*               id_ptr;
        typedef std::map<tok_ptr,id_ptr> id_map;
        typedef id_map::value_type       pair;
        typedef id_map::iterator         iterator;

    private:
        id_map id_table;
    protected:
        self_ptr prev;
    public:
        env(self_ptr prev)
        :prev(prev)
        {}
        virtual ~env()
        {}

        void put(tok_ptr w,id_ptr i);
        
        id_ptr get(tok_ptr w);
    };//class env    
};//namespace symbols

#endif//_ENV_H
