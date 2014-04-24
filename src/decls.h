#ifndef _DECLS_H
#define _DECLS_H

#include"array.h"
#include"id.h"
#include"type.h"
#include"token.h"

#include<vector>

namespace inter{
   struct declarator:public node{
    lexer::word*  name;
       typedef lexer::token token;
       typedef token*       tok_ptr;
       typedef symbols::type type;
       typedef type*          type_ptr;
       typedef id*             id_ptr;
        virtual void gen(tok_ptr ) = 0;
        virtual id_ptr produce(tok_ptr )const = 0;
        void gen()const{}
   };//declarator

    struct single_id:public declarator{
        expr* init;
        int offset;
        void gen(tok_ptr t)
        {
            offset = node::esp_offset + 4;
            if(init != NULL){
                init -> gen();
                this -> emitpush("%eax");
            }else{
                this -> emitstack(-4);
            }

        }

        id* produce(lexer::token* t)const
        {
            return new id(name,symbols::int_,offset);
        }

        void gen()const
        {}

    };//

    struct arr_decl:public declarator{
        lexer::num*  size;
        std::vector<expr*>*initlist;
        int     offset;
        void gen(tok_ptr t)
        {
            int i = 0;
            int sz = size -> value;
            offset = node::esp_offset + 4;

            if(initlist != NULL)
                if(initlist -> size() > sz)
                    throw "Declaration is out of range";
                for(i = 0;i < initlist -> size();i++){
                    initlist -> at(i) -> gen();
                    this -> emitpush("%eax");
                }
            if(i < sz)
                this -> emitstack((i - sz)*4);
        }

        id* produce(lexer::token* t)const
        {
            int sz = size -> value;
            symbols::array* arr =  new symbols::array(sz,symbols::int_);
            return new id(name,arr,offset);
        }
    };//struct arr_decl

    struct idlist:public node{

        std::vector<declarator*> list;

        void gen(lexer::token* t)const
        {
            for(int i = 0 ; i < list.size();i++)
                list[i] -> gen(t);
        }
        void gen()const{}
    };//struct idlist

    struct decl:public node{
        lexer::token*   t;
        idlist*         idl;

        void gen()const
        {
            if(idl&&t)
                idl -> gen(t);
        }
    };//struct decl

    struct decls:public node{
        decls* d1;
        decl*  d2;
        void gen()const
        {
            if(d1 != NULL)
                d1 -> gen();
            if(d2 != NULL)
                d2 -> gen();
        }
    };//struct decls

};//namespace inter

#endif//_DECLS_H
