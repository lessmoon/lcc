#ifndef _INTER_NODES_H
#define _INTER_NODES_H

#include"node.h"
#include"expr.h"
#include"stmt.h"
#include"decls.h"

#include<vector>

namespace inter{

    struct for_:public stmt{
        public:
            expr* e1;expr* c;expr* e2;
            stmt* s;
        public:
            for_()
            {

            }
            for_(expr* e1,expr* c,expr* e2,stmt* s)
            :e1(e1),c(c),e2(e2),s(s)
            {}
        public:
            void gen()const
            {
                int lc = this -> newlabel();
                int le1 = this -> newlabel();
                int le2 = this -> newlabel();
                int ls = this -> newlabel();
                int lend = this -> newlabel();
                this -> emitlabel(le1);
                e1 -> gen();
                this -> emitlabel(lc);
                c -> gen();
                this -> emit("cmpl $0,%eax\n");
                this -> emitjmp(lend,JZ);
                this -> emitlabel(ls);
                s -> gen();
                this -> emitlabel(le2);
                e2 -> gen();
                this -> emitjmp(lc,JMP);
                this -> emitlabel(lend);
            }

    };//for_

    struct while_:public stmt{
        expr* c;
        stmt* s;

       public:
            void gen()const
            {
                int lc = this -> newlabel();
                int ls = this -> newlabel();
                int lse = this -> newlabel();
                this -> emitlabel(lc);
                c -> gen();
                this -> emit("cmpl $0,%eax\n");
                this -> emitjmp(lse,JZ);
                this -> emitlabel(ls);
                s -> gen();
                this -> emitjmp(lc,JMP);
                this -> emitlabel(lse);
            }

    };//struct while_

    struct dowhile:public stmt{
        stmt* s;
        expr* c;

        public:
            void gen()const
            {
                int ls = this -> newlabel();
                int lc = this -> newlabel();
                this -> emitlabel(ls);
                s -> gen();
                this -> emitlabel(lc);
                c -> gen();
                this -> emit("cmpl $0,%eax\n");
                this -> emitjmp( ls,JNZ);
            }
    };//struct dowhile

    struct if_:public stmt{
        expr* c;
        stmt* s;

        public:
            void gen()const
            {
                int ce = this -> newlabel();
                int se = this -> newlabel();
                c -> gen();
                this -> emitlabel(ce);
                this -> emit("cmpl $0,%eax\n");
                this -> emitjmp(se,JZ);
                s -> gen();
                this -> emitlabel(se);
            }

    };//struct if_

    struct ifelse:public stmt{
        expr* c;
        stmt* strue;
        stmt* sfalse;

        public:
            void gen()const
            {
                int l1 = this -> newlabel();
                int l2 = this -> newlabel();
                c -> gen();
                this -> emitjmp(l1,JZ);
                strue -> gen();
                this -> emitjmp(l2,JMP);
                this -> emitlabel(l1);
                sfalse -> gen();
                this -> emitlabel(l2);
            }

    };//struct ifelse

    struct sexpr:public stmt{
        expr*e;
        void gen()const
        {
            e -> gen();
        }
    };//struct func_printf


    struct func_printf:public stmt{
        std::vector<expr*>* paralist;

        void gen()const
        {
            if(paralist)
                for(int i = paralist -> size() - 1; i >= 0 ;i--){
                    paralist -> at(i) -> gen();
                    this -> emitpush("%eax");
                }
            this -> emit("call printf\n");
            this -> emitstack(paralist -> size()*4);
        }
    };//struct func_printf
};//namespace inter

#endif //_INTER_NODES_H
