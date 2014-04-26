#include"tabgen.h"
#include<iostream>

namespace cctabgen{
    tabgen::tabgen()
    :para(NULL){}
    tabgen::~tabgen()
    {
        srtable.clear();
    }

    void tabgen::set_para(info* para)
    {
        this -> clear();
        this -> para     = para;
        /*do the dirty work*/
        this -> sym_name =  &(para -> def_table);
        this -> svt      =  &(para -> type_table);
        this -> str_sym  =  &(para -> start_symbols);
        this -> ps       =  &(para -> productions);
    }

    tabgen::table*  tabgen::calculate()
    {
#define END_SYM     1
        if(!para)
            return NULL;
        if(!srtable.empty())
            return &srtable;
        this -> clear();
        sym_set has_visited;
        ffsm.reserve(svt -> size());
        ffsm.resize(svt -> size());

        /*At first we calculate the first symbols set of every symbol*/
        for(sym_t i = 0 ; i < svt -> size() ; i++){
            /*Just in case!*/
            if(svt -> at(i) != ccparser::VAR
               || ps -> at(i) != NULL)
                this -> first_term(i,ffsm[i],has_visited);
            has_visited.clear();
            has_empty.clear();
        }
        item_list I0;

        item tmp(0,-1,NULL,END_SYM);
        std::vector<right*> dcltor; //collector the garbage
        
        for(sym_set::iterator iter = str_sym -> begin();
            iter != str_sym -> end();
            iter++){
            /*TODO:collect the right created here*/
            right* temp = new right;
            dcltor.push_back(temp);
            try{
                temp -> add(*iter);
                tmp.set(0,-1,temp,END_SYM);
                I0.add(tmp);
            }catch(...){
                for(int i = 0; i < dcltor.size();i++)
                    delete (dcltor.at(i));
                throw ;
            }
        }
        closure(I0);
        this -> closure_set(I0);
        for(int i = 0; i < dcltor.size();i++)
               delete (dcltor.at(i));

        return &srtable;
#undef  END_SYM
    }

    bool tabgen::first_term(const sym_t sym,sym_set& fset,
                            sym_set& has_visited)
    {
#define sym_type(x) (svt->at(x))
#define EMPTY       0
        bool res = false;
        int size;
        bool test;
        right* tmp;
        rightlist* rl;
        if(sym_type(sym) == ccparser::TERM){
            fset.insert(sym);
            return (sym == EMPTY);
        }

        if(has_visited.count(sym) > 0){//this var has been visited
            return (has_empty.count(sym) > 0);
        }

        has_visited.insert(sym);//put the sym into visited
        rl = ps -> at(sym);
        if(rl == NULL)
            this -> error("Symbol `" +  para -> get_name(sym) + "' has no productions");

        size = -1;
        while(size < (int)fset.size()){
            size = fset.size();
            for(int i = 0; i < rl -> size();i++){
                tmp = (rl -> at(i));
                for(int j = 0;j < tmp -> size();j++){
                    /*
                     * A funny bug: Code With Bugs Before is following
                     * res = res || first_sym(...)
                     */
                    test =  first_term(  tmp -> at(j),fset,
                                        has_visited) ;
                    if(!test){
                            break;
                    }else{
                            res = true;
                            has_empty.insert(sym);
                    }
                }
            }
        }
        return res;
#undef EMPTY
#undef sym_type//define sym_type(x)
    }

    bool tabgen::first_term(const sym_seq&syms,sym_set&fset)
    {
        for(int i = 0;i < syms.size();i++){
            sym_set&tmp = ffsm[syms[i]];
            fset.insert(tmp.begin(),tmp.end());
            if(tmp.count(0) == 0){
                break;
            }
        }
        return fset.count(0);
    }

    void tabgen::closure(item_list&I)
    {
        int size;
        right*r;
        int l;
        int now;
        rightlist*rl;
        item tmp(0,0,0,0);
        sym_seq seq;
        sym_set fset;
        sym_set has_visited;

        right* tmpr = NULL;
        do{
            size = I.size();
            for(int i = 0;i < I.size();i++){
                r = I.at(i).r;
                now = I.at(i).now;
                /*Be careful !*/
                if(now >= r -> size())
                    continue;
                l = r -> at(now);
                rl = ps -> at(l);
                if(rl == NULL)//if it is not a var symbol
                    continue;
                seq.clear();
                fset.clear();
                has_visited.clear();
                //has_empty.clear();
                for(int j = now + 1;j < r -> size();j++){
                    seq.push_back(r -> at(j));
                }
                seq.push_back(I.seq[i].a);
                //calculate the first_set of the sym seq
                first_term(seq,fset);
                //for each prod l => ?
                for(int j = 0; j < rl -> size();j++){
                    tmpr = rl -> at(j);

                    /*
                     * if the production is like 
                     * a => %|T;
                     * b => %|F;
                     * c => a b;
                     * when calculate the closure 
                     * it will sometimes like
                     * ********************
                     * * c => .a b [1]    *
                     * * a => .%   [1]    *
                     * ********************
                     * Obviously,a => .% should be a => %.
                     * So we should skip the %.
                     * this seems to be weird,but it is important
                     */
                    int now = 0;
                     while(now < tmpr -> size()){
                        if(tmpr -> at(now)==0)
                            now++;
                        else
                            break;
                    }

                    for(sym_set::iterator iter = fset.begin();
                        iter != fset.end();
                        iter++){
                        tmp.set(now,l,tmpr,*iter);
                        if(!I.find(tmp))
                            I.add(tmp);
                    }
                }
            }
        }while(I.size() > size);
    }

    void tabgen::closure_set(item_list&I0)
    {
#define sym_type(x) (svt -> at(x))
        item_set s;
        item_list* il;
        s.add(I0);
        this -> addline();
        item* it = NULL;
        item_list tmp1;
        //for each Item Set in the s
        for(int i = 0; i < s.size();i++){
            tmp1.reserve(10);
            //for each context grammer symbols
            //Because 0 is the empty symbol,ignore it
            for(sym_t k = 0;k < svt -> size();k++){
                il = &(s.at(i));
                for(int j = 0;j < il -> size();j++){
                    it = &(il -> at(j));
                    int n = it -> now;
                    while(n < it -> size()){
                        if(it -> at(n) == 0)
                            n++;
                        else
                            break;
                    }
                    if(n < it -> size()){
                        if(it -> at(n) == k){
                            item tmp(n + 1,it -> l,it -> r,it -> a);
                            if(!tmp1.find(tmp)){
                                  tmp1.add(tmp);
                                  closure(tmp1);
                            }
                        }
                    }else{
                        if(it -> l != -1){
                            /*it should be reduced now!*/
                            if(srtable[i][it -> a].t != ERROR){
                                if(!(srtable[i][it -> a].t == REDUCE && srtable[i][it ->a].where == ps -> get_id(it -> l,it -> r) + 1 ))
                                    std::cerr<<"At I"<<i<<"["<<i<<","<<it -> a<<"]"<<" has conflict with "<<(srtable[i][it -> a].t == SHIFT?'s'
                                                                                :(srtable[i][it -> a].t == REDUCE)?'r'
                                                                                :'n')<<srtable[i][it ->a].where
                                                                                <<" and r"<<ps -> get_id(it -> l,it -> r)+1<<"\n";
                                //throw 78;
                            }
                            srtable[i][it -> a] = action_node(REDUCE,ps -> get_id(it -> l,it -> r) + 1);
                        }else{
                            /*it is the state of accept*/
                            if(k == 1)
                                srtable[i][k] = action_node(REDUCE,0);
                        }
                    }
                }
                if(tmp1.size() > 0){
                    int f = s.find(tmp1);
                    if(f < 0){
                        this -> addline();
                        f = s.add(tmp1);
                    }
                    if(srtable[i][k].t != ERROR){
                      if(!(srtable[i][k].t == REDUCE && srtable[i][k].where == ps -> get_id(it -> l,it -> r) + 1 ))
                         std::cerr<<"At I"<<i<<"["<<i<<","<<k<<"] has conflict with "<<(srtable[i][k].t == SHIFT?'s'
                                                                     :(srtable[i][k].t == REDUCE)?'r'
                                                                     :'n')<<srtable[i][k].where
                                                                    <<" and s"<<f<<"\n";

                    }
                    srtable[i][k] = action_node((sym_type(k)==ccparser::TERM?SHIFT
                                                                :JUSTGO),f);
                }
                tmp1.clear();
            }
        }

#undef sym_type
    }

    void tabgen::clear()
    {
        srtable.clear();
        ffsm.clear();
        has_empty.clear();
    }

    void tabgen::error(const string&info)
    {
        throw info;
    }

    void tabgen::addline()
    {
        srtable.reserve(srtable.size() + 1);
        srtable.resize(srtable.size() + 1);
        srtable.back().reserve( svt -> size());
        srtable.back().resize(svt -> size());
    }
};//namespace cctabgen
