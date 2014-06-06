#include"cctype.h"

namespace cctype{
    /*struct right*/

    void right::add(const sym_t id)
    {
        seq.push_back(id);
    }

    sym_t right::at(const int idx)const
    {
        return seq.at(idx);
    }

    int right::size()const
    {
        return seq.size();
    }

    bool right::operator==(const self&b)const
    {
        list::const_iterator iter1,iter2;
        if(this -> size() != b.size())
            return false;
        if(&b == this)/*simple situation*/
            return true;
        iter1 = seq.begin();
        iter2 = b.seq.begin();
        for(;iter1 != seq.end();iter1++,iter2++)
            if( *iter1 != *iter2 )
                return false;
        return true;
    }

    /*struct rightlist*/

    void rightlist::add(right_ptr r)
    {
        seq.push_back(r);
    }

    int rightlist::size()const
    {
        return seq.size();
    }

    rightlist::right_ptr rightlist::at(const int idx)
    {
        return seq.at(idx);
    }

    const rightlist::right_ptr rightlist::at(const int idx)const
    {
        return seq.at(idx);
    }

    bool rightlist::find(const right_ptr r)const
    {
        list::const_iterator iter = seq.begin();
        while(iter != seq.end()){
            if(*iter == r)
                return true;
            else if(*(*iter) == *r)/**/
                return true;
            iter++;
        }
        return false;
    }

    int rightlist::get_id(const right_ptr r)const
    {
        for(int i = 0;i < this -> size();i++)
            if(r == this -> at(i))
                return i;
        return -1;
    }
    /*struct prods*/

    prods::prods()
    {}

    void prods::add(const sym_t l,right_ptr r)
    {
        map::iterator iter = seq.find(l);
        if(iter != seq.end())
            iter -> second -> add(r);
        else {
            rl_ptr rl = new rightlist;
            rl -> add(r);
            //idseq.insert(id_map::value_type(l,r));
            seq.insert(map::value_type(l,rl));
        }
        prod tmp = {l,r};
        pseq.push_back(tmp);
    }

    prods::rl_ptr prods::at(const sym_t left)
    {
        map::iterator iter = seq.find(left);
        return ((iter == seq.end())?NULL : iter -> second);
    }

    const prods::prod&prods::get_prod(const int idx)const
    {
        /*TODO:create a sequence to store every product!*/
        return pseq.at(idx);
    }

    int prods::get_id(const sym_t l,right_ptr r)const
    {
        for(int i = 0;i < pseq.size();i++)
            if(pseq.at(i).l == l && pseq.at(i).r == r)
                return i;
        return -1;
    }

    int prods::size()const
    {
        return pseq.size();
    }
    /*struct item*/

    item::item(const int now,const sym_t l,right*r,const int a)
    :now(now),l(l),r(r),a(a)
    {}

    void item::set(const int now,const sym_t l,right*r,const int a = 0)
    {
        this -> now = now;
        this -> l   = l;
        this -> r   = r;
        this -> a   = a;
    }

    bool item::operator==(const item&i)const
    {
#if defined LR_1
#warning(LR_1 is defined)
        return (now == i.now)&&(l == i.l)&&(this -> a == i.a)&&(*r == (*i.r));
#elif defined LALR
#warning(LALR is defined)
        return (now == i.now)&&(l == i.l)&&(*r  == (*i.r));
#endif
    }

    sym_t item::at(const int idx)const
    {
        return r -> at(idx);
    }

    int item::size()const
    {
        return r -> size();
    }

    /*struct item_list*/

    item_list::item_list()
    {}

    item_list::item_list(const self&z)
    :seq(z.seq)
    {}

    item_list&item_list::operator=(const item_list&z)
    {
        seq = z.seq;
        return *this;
    }

    bool item_list::operator==(const item_list&z)const
    {
        if(z.size() != size())
            return false;
        if(this == &z)
            return true;
        for(int i = 0;i < size();i++)
            if(!z.find(seq[i]))
                return false;
        return true;
    }

    void item_list::add(const item&z)
    {
        seq.push_back(z);
    }

    bool item_list::find(const item&b)const
    {
        list::const_iterator iter = seq.begin();
        while(iter != seq.end()){
            if(*iter == b)
                return true;
            iter++;
        }
        return false;
    }

    item& item_list::at(const int idx)
    {
        return seq.at(idx);
    }

    int item_list::size()const
    {
        return seq.size();
    }

    void item_list::reserve(const int sz)
    {
        seq.reserve(sz);
    }

    void item_list::clear()
    {
        seq.clear();
    }

    /*struct item_set*/
    int item_set::add(const item_list&i)
    {
        seq.push_back(i);
        return seq.size()-1;
    }

    item_list& item_set::at(const int idx)
    {
        return seq.at(idx);
    }

    int item_set::size()const
    {
        return seq.size();
    }

    int item_set::find(const item_list& z)const
    {
        int j;
        for(j = size() -1;j >= 0 ;j--)
            if(z == seq[j])
                break;
        return j;
    }
};//namespace cctype
