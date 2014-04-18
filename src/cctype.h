#ifndef _COMPILER_COMPILER_TYPE_H
#define _COMPILER_COMPILER_TYPE_H

#include<vector>
#include<map>
#include<string>

namespace cctype{
    typedef int     sym_t;
    typedef std::string string;

    /*the right part of a product*/
    struct right{
    public:
        typedef std::vector <sym_t> list;
        typedef right self;
    public:
        list seq;
    public:
        /*add a symbol to the end of the list*/
        void add(const sym_t id);
        /*random access of a symbol in the list*/
        sym_t at(const int idx)const;
        /*get the number of symbols in the list*/
        int size()const;
        /*judge if it is *EQUAL* with b*/
        bool operator==(const self&b)const;
        /*TODO:get the string of the product*/

#if 0
        string toString()const;
#endif
    };//struct right

    /*the right part seq of a left symbol (of products)*/
    struct rightlist{
    public:
        typedef right*                  right_ptr;
        typedef std::vector<right_ptr>  list;
        typedef rightlist               self;
    public:
        list seq;
    public:
        /*add a right part of product */
        void add(right_ptr r);
        /*get the number of the products*/
        int size()const;
        /*random access of a product in the container*/
        right_ptr at(const int idx);
        const right_ptr at(const int idx)const;
        /*judge if there is an *EQUAL* right part*/
        bool find(const right_ptr r)const;
        /*get the index of the right,if not found return -1*/
        int get_id(const right_ptr r)const;
    };//struct rightlist

    /*container of products*/
    struct prods{
    public:
        typedef right*                  right_ptr;
        typedef rightlist*              rl_ptr;
        typedef std::map<sym_t,rl_ptr>  map;
        typedef std::map<sym_t,int>     id_map;
        typedef struct{sym_t l;right_ptr r;}prod;
        typedef std::vector<prod>       prod_seq;
    public:
        map         seq;
        prod_seq    pseq;

    public:
        prods();
        /*add a product(l => r)*/
        void    add(const sym_t l,right_ptr r);
        /*get right part of a left symbol,if not found return NULL*/
        rl_ptr  at(const sym_t left);
        /*get right part by its index*/
        const prod&get_prod(const int idx)const;
        /*get index of a production*/
        int get_id(const sym_t left,right_ptr r)const;
        /*get number of all products*/
        int size()const;
    };//struct prods

    /*like a => .bdc , a => b.dc*/
    struct item{
    public:
        typedef right*  right_ptr;
        typedef item    self;
    public:
        int now;//begin at 0
        sym_t l;//the left id
        right_ptr r;
        sym_t a;
    public:
        /*constructor*/
        item(const int now,const sym_t l,right_ptr r,const sym_t a );
        /*set every value*/
        void set(const int now,const sym_t l,right_ptr r,const sym_t a );
        /*equal test*/
        bool operator==(const self&b)const;

        /*random access of the symbol in the right part*/
        sym_t at(const int idx)const;

        /*get the number of the symbols in the right part*/
        int size()const;
    };//struct item

    /*item set*/
    struct item_list{
    public:
        typedef std::vector<item> list;
        typedef item_list self;
    public:
        list seq;
    public:
        /*constructor*/
        item_list();
        /*copy constructor*/
        item_list(const self&z);
        self&operator=(const self&z);

    public:
        /*add an item(by copying)*/
        void add(const item&z);
        /*equal test*/
        bool operator==(const item_list&z)const;
        /*test if there is an item in the item_list*/
        bool find(const item&b)const;
        /*random access of item in the container*/
        item&at(const int idx);
        /*get number of items in the container*/
        int size()const;

        /*reserve some size to container more*/
        void reserve(const int sz);
        /*clear all*/
        void clear();
    };//struct item_list

    /*item sets*/
    struct item_set{
    public:
        typedef std::vector<item_list> list;
    public:
        list seq;
        /**/
    public:
        /*add an item_list ,return the index of the new item_list*/
        int add(const item_list&i);
        /*random access of the item_list*/
        item_list&at(const int idx);
        /*get the number of item_list*/
        int size()const;
        /*
         *find an item_list in the container,
         *if not found return -1,or return its idx
         */
        int find(const item_list&z)const;
    };//struct item_set
};//namespace cctype

#endif//_COMPILER_COMPILER_TYPE_H
