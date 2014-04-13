#include"lexer.h"
#include"kb_io.h"
#include"file_io.h"

#include<string>
#include<list>
#include<map>
#include<set>
#include<iostream>
#include<vector>
#include<sstream>

enum TYPE { UNKNOWN = -1, VAR, SYM };

struct right {
	typedef std::vector < int > list;
	list var_seq;
	void add(const int sym_id)
	{
		var_seq.push_back(sym_id);
    }

    std::string toString()const
    {
        std::string tmp;
        for(list::const_iterator iter = var_seq.begin();
            iter != var_seq.end();
            iter ++)
        {
             tmp += " " + ::toString(*iter);
        }
        return tmp;
    }

    bool operator==(const right&b)const
    {

        if(size() != b.size())
            return false;
        list::const_iterator  iter1 = var_seq.begin(),
                        iter2 = b.var_seq.begin();
        for(;iter1 != var_seq.end();iter1++,iter2++){
            if(*iter1 != *iter2)
                return false;
        }
        return true;
    }

    int at(const int idx)const
    {
        return var_seq.at(idx);
    }

    int size()const
    {
        return var_seq.size();
    }
};

struct rightlist {
	typedef std::vector < right * >list;
	list pro_seq;
	void add(right * prod)
	{
		pro_seq.push_back(prod);
    }
    std::string toString()
    {
        std::string tmp;
        for(list::iterator iter = pro_seq.begin();
            iter != pro_seq.end();
            iter++){
            tmp += (*iter) -> toString() + "\n\t\t";
        }
        return tmp;
    }

    int size()const
    {
        return pro_seq.size();
    }

    right*at(const int idx)
    {
        return pro_seq[idx];
    }

    bool find(const right*z)const
    {
        list::const_iterator iter = pro_seq.begin();
        while(iter != pro_seq.end()){
            if(*(*iter) == *z)
                return true;
            iter++;
        }
        return false;
    }

    int get_id(right*r)const
    {
        for(int i = 0; i < pro_seq.size();i++)
            if(r == pro_seq[i])
                return i;
        return -1;
    }
};

struct prods {
	typedef std::map < int, rightlist * >map;
	typedef std::map < int, int >   id_map;
	map seq;
	id_map idseq;
	int lastid;

	prods(){lastid = 1;}

	void add(const int left, right * x)
	{
		map::iterator iter = seq.find(left);
		if (iter != seq.end())
			iter->second->add(x);
		else {
			rightlist *ss = new rightlist;
			 ss -> add(x);
			 idseq.insert(id_map::value_type(left, lastid));
			 seq.insert(map::value_type(left, ss));
        }
        lastid++;
	}
	rightlist *get(const int left)
	{
		map::iterator iter = seq.find(left);
		return (iter != seq.end())? iter->second : NULL;
	}

	int get_id(const int left,right*r)
	{
	    int id = idseq[left];
        id += seq[left] -> get_id(r);
        return id;
	}
};

class parser {
 public:
	typedef lexer::token token;
	typedef lexer::lexer lexer;
	typedef std::string string;
	typedef std::map < string, int > syms_vars;//name and its'index
	typedef std::set < int > list;
    typedef std::vector<TYPE> syms_var_table;//get its type by its'index
    typedef syms_vars::iterator iterator;
 public:
    syms_var_table ratable;
 private:
    syms_vars def_table;
	lexer *lex;
	token *look;
	prods *stmts;
 public:
	list begin;
 public:
	~parser()
	{
		if (stmts != NULL)
			delete stmts;	//wrong foreach
		//def_table.clear
	}
	parser(lexer * lex)
	:lex(lex), stmts(NULL)
	{

	}

	void read()
	{
		look = lex->scan();
	}

	prods *getstmts()
	{
		if (stmts != NULL)
			delete stmts;
		stmts = new prods;
        this -> def("%",SYM);
        this->read();
		while (look->tag == '#')	//define the symbols and variable
			getdef();
		while (look->tag == '!')    //get the products
			getprods();
		while (look->tag == '*')    //get the begining state
            getbegin();
		return stmts;
	}

	void getbegin()
	{
        string tmp;
        match('*');
 loop:
        tmp = toString();
        if(getdef(tmp) != VAR)
            throw 78910;//undefined var
        iterator iter = def_table.find(tmp);
        if(begin.count(iter -> second) == 0)
            begin.insert(def_table.find(tmp)->second);
        this -> read();
        if(look -> tag == ','){
            this -> read();
            goto loop;
        }
        match(';');
    }

	void def(const string & name, const TYPE t)
	{

		if (def_table.find(name) != def_table.end())
			throw 777;	//REDFINED ERROR
		else{
			def_table.insert(syms_vars::value_type(name, ratable.size()));//ratable begins at 0
            ratable.push_back(t);
        }
	}

	TYPE getdef(const string & name)
	{
		syms_vars::iterator iter = def_table.find(name);
		if (iter == def_table.end())
			return UNKNOWN;
		else
			return ratable[iter->second];
	}

	void getdef()
	{
		match('#');
        string tmp = toString();
		if (tmp == "var") {
 loop1:
			this->read();
			this->def(toString(), VAR);
			this->read();
			if (look->tag == ',')
				goto loop1;
		} else if (tmp == "sym") {
 loop2:
			this->read();
			this->def(toString(), SYM);
			this->read();
			if (look->tag == ',')
				goto loop2;
		} else {
			throw 888;	//UNKNOWN OPERATION
		}
		match(';');
	}

	void getprods()
	{
		right *tmp;
		match('!');
		string leftname = toString();
		if (getdef(leftname) != VAR) {
            std::cerr<<"In line "<<lex -> lineno
                     <<"."<<leftname<<"not found here."<<std::endl;
			throw 999;	//NOT FOUND OR IS SYMBOLS
		}
		this->read();
		match('=');
 loop:
		tmp = getprod();
		stmts->add(def_table.find(leftname)->second, tmp);
		if (look->tag == '|') {
			this->read();
			goto loop;
		}
		match(';');
	}

	right *getprod()
	{
		right *res = new right;
		while (look->tag ==::lexer::tag::ID || look -> tag == '%') {
			if (getdef(toString()) == UNKNOWN)	//not defined
				throw 1111;
			else
				res->add(def_table.find(toString())->second);
			this->read();
		}
		return res;
	}

	std::string toString()
	{
        if(look -> tag == '%')
            return "%";
		return (static_cast < ::lexer::word * >(look))->lexme;
	}

	bool match(int tag)
	{
		if (look->tag == tag) {
			this->read();
			return true;
		} else {
			throw 1110;
		}
	}

    int number_of_symbols()const
    {
        return def_table.size();
    }

};//class parser

struct item{
    int now;//begin at 0
    int l;//the left id
    right*r;//the right list
    int a;//lookahead symbol

    item(const int now,const int l,right*r,const int a = 0)
    :now(now),l(l),r(r),a(a){}

    void set(const int now,const int l,right*r,const int a = 0)
    {
        this -> now = now;
        this -> l = l;
        this -> r = r;
        this -> a = a;
    }

    bool operator==(const item&z)const
    {
        return (now == z.now)&&(l==z.l)&&(*r == (*z.r))&&(this -> a == z.a);
    }

    std::string toString()const
    {
        return ::toString(l) + "->" + r-> toString()
                + "(" + ::toString(now)+ ") [" + ::toString(this -> a) + "]";
    }

    int at(const int idx)const
    {
        return r -> at(idx);
    }

    int size()const
    {
        return r -> size();
    }
};

enum action_do{
    REDUCE,SEND,JUSTGO,ERROR
};

struct action_node{
    action_do t;
    int where;
    action_node(){t= ERROR;where =0;}
    action_node(action_do t,int where)
    :t(t),where(where){}
};

struct item_list{
    typedef std::vector<item> container;
//    typedef std::vector< vector<int> > zzz;
    container seq;

    item_list(){}
    item_list(const item_list&z)
    :seq(z.seq)
    {
    }

    item_list&operator=(const item_list&z)
    {
        seq = z.seq;
    }

    bool operator==(const item_list&z)const
    {
        if(z.size()!=size())
            return false;
        for(int i = 0;i < size();i++)
        {
            if(!z.find(seq[i]))
                return false;
        }
        return true;
    }

    void push(const item&z)
    {
        seq.push_back(z);
    }

    bool find(const item&b)const
    {
        container::const_iterator iter = seq.begin();
        while(iter != seq.end()){
            if(*iter == b)
                return true;
            iter++;
        }
        return false;
    }

    item&at(const int id)
    {
        return seq[id];
    }

    int size()const
    {
        return seq.size();
    }

    void print()const
    {
        for(int i = 0;i<size();i++){
            std::cout<<seq[i].toString()<<std::endl;
        }
    }
    void clear()
    {
        seq.clear();
    }
};



/*
 * return if the set of first symbols contains the empty symbol
 */

bool first_sym(const int sym,std::set<int>&fset,
                std::set<int>&has_visited,
                std::set<int>&has_empty,
                prods*ss,parser::syms_var_table*svt)
{
#define sym_type(x) (svt->at(x))
#define EMPTY       0
    bool res = false;
    int var;
    rightlist* z;
    if(sym_type(sym) == SYM){
        fset.insert(sym);
        return (sym == EMPTY);
    }
    /*
     *  but in this case this return may be not right
     *  a => b a c | E;
     *  b => E ;
     *  first(a) = first(b) U first(c) actually
     */
    if(has_visited.count(sym) > 0){//this var has been visited
        return (has_empty.count(sym) > 0);
    }
    has_visited.insert(sym);//put the sym into visited
    z = ss -> get(sym);
    if(ss == NULL)
        throw 7891011;
    right* tmp;
    int size = -1;
    bool test = false;
    while(size < (int)fset.size()){
        size = fset.size();
        for(int i = 0; i < z -> size();i++){
            tmp = (z -> at(i));
            test = false;
            for(int j = 0;j < tmp -> size();j++){
                /*
                 * A funny bug: Code With Bugs Before is following
                 * res = res || first_sym(...)
                 */
                test =  first_sym(  tmp -> at(j),fset,
                                   has_visited,has_empty,
                                   ss,svt) || test;
                if(!test){
                        res = true;
                        break;
                }else{
                        has_empty.insert(sym);
                }
            }
        }
    }
    return res;
#undef EMPTY
#undef sym_type//define sym_type(x)
}

void closure(item_list&il)
{

}

int main()
{
    typedef std::set<int> flag_set;
    flag_set first_set,has_visited,has_empty;
    parser::syms_var_table* svt ;
    system::iol* io = new system::kb_io;
    lexer::lexer* l = new lexer::lexer(io);
    parser* p = new parser(l);
    prods* ss = p -> getstmts();

    svt = &(p -> ratable);
    for(prods::map::iterator iter = ss -> seq.begin();
        iter != ss -> seq.end();
        iter++){
        std::cout<<"\t";
        if(p -> begin.count(iter -> first)>0)
            std::cout<<">>";
        std::cout<<iter -> first<<" -> \t";
        std::cout<<iter->second->toString();
        std::cout<<"\n";
    }

    int sym;
    while(std::cin>>sym){
        if(sym < 0)
            goto end;

        first_sym(  sym,first_set,
                    has_visited,has_empty,
                    ss,svt );
        for(flag_set::iterator iter = first_set.begin();
            iter != first_set.end();
            iter ++){
                std::cout<<*iter<<" ";
            }
        std::cout<<std::endl;
        first_set.clear();
        has_visited.clear();
        has_empty.clear();
    }
end:
    delete p;
    delete l;
    delete io;
    return 0;
}
