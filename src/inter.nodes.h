#ifndef _INTER_NODES_H
#define _INTER_NODES_H

#include"node.h"
#include"expr.h"

#include"stmt.h"

namespace inter{
    
    struct for_:public stmt{
        public:
            expr* e1;expr* c;expr* e2;
            stmt* s;
        public:
            for_(expr* e1,expr* c,expr* e2,stmt* s)
            :e1(e1),e2(e2),c(c),s(s)
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
                this -> emitjmp(lend,JNZ);
                this -> emitjmp(ls,JMP);
                this -> emitlabel(le2);
                this -> emitjmp(lend,JMP);
                e2 -> gen();
                this -> emitlabel(ls);
                s -> gen();
                this -> emitjmp(le2,JMP);
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
                this -> emitjmp(lse,JZ);
                this -> emitlabel(ls);
                s -> gen();
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


};//namespace inter

#endif //_INTER_NODES_H
