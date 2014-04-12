#include"env.h"

namespace symbols {
    void env::put(tok_ptr w,id_ptr i)
    {
        id_table.insert(pair(w,i));
    }

    env::id_ptr env::get(tok_ptr w)
    {
        iterator iter;
        self_ptr e = this;
        do{
            if((iter = e -> id_table.find(w))
                != e -> id_table.end())
                return (iter -> second);
            e = e -> prev;
        }while(e != NULL);
        return NULL;
    }
};//namespace symbols
