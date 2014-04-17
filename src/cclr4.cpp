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

	void print_def_table()
	{
	    for(syms_vars::iterator iter = def_table.begin();iter != def_table.end();iter++){
            std::cout<<iter -> second<<":"<<iter -> first<<std::endl;
	    }
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
        this -> def("%",SYM);//empty symbol,USING 0
        this -> def("$",SYM);//the end symbol of the input stream,USING 1
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
        if(look -> tag == '%' || look -> tag == '$')
            return std::string("")+char(look -> tag);
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

/*
 * Calclute the first symbol set of a given symbol
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
    if(z == NULL)
        throw 7891011;
    right* tmp;
    int size = -1;
    bool test;
    while(size < (int)fset.size()){
        size = fset.size();
        for(int i = 0; i < z -> size();i++){
            tmp = (z -> at(i));
            for(int j = 0;j < tmp -> size();j++){
                /*
                 * A funny bug: Code With Bugs Before is following
                 * res = res || first_sym(...)
                 */
                test =  first_sym(  tmp -> at(j),fset,
                                   has_visited,has_empty,
                                   ss,svt) ;
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

/*
 * Calculate the first syms of a given symbol list.
 * e.g:
 *     a => a b d|E
 *     b => F d|C
 *     d => H;
 * the first({a b d}) = {F,C};
 */

bool first_sym( std::vector<int>&syms,std::set<int>&fset,
                std::set<int>&has_visited,
                std::set<int>&has_empty,
                prods*ss,parser::syms_var_table*svt)
{
        bool res = false;
        int size = -1;
        while(size < (int)fset.size()){
            size = fset.size();
            for(int i = 0;i < syms.size();i++){
                res = first_sym(syms[i],fset,has_visited,has_empty,ss,svt);
                if(!res){
                    break;
                }else{
                    has_empty.insert(syms[i]);
                }
            }
        }

        return res;
}

bool first_sym(std::vector<int>&syms,std::set<int>&fset,//fset is the result to return
                std::vector< std::set<int> >&ffsm//the fast first symbols map
                )
{
    bool res = false;
    int size = -1;
    size = fset.size();
    for(int i = 0;i < syms.size();i++){
        std::set<int>&tmp = ffsm[syms[i]];
        fset.insert(tmp.begin(),tmp.end());
        if(tmp.count(0) == 0){//has no empty symbol in its first symbols
             break;
        }
    }
    return fset.count(0);
}

void closure(item_list&I,prods*stmts,parser::syms_var_table*svt)
{
loop:
    int size;
    item_list::container::iterator iter_item;
    rightlist::list::iterator iter_right;
    right*r;
    int l;
    int now;
    rightlist*rl;
    item tmp(0,0,0,0);
    std::vector<int> seq;
    std::set<int> fset;
    std::set<int> has_visited;
    std::set<int> has_empty;
    right* tmpr = NULL;
    do{
        size = I.size();
        for(int i = 0;
            i < I.size();
            i++){
            r = I.seq[i].r;
            now = I.seq[i].now;
            l = r -> var_seq[now];
            rl = stmts -> get(l);
            if(rl == NULL)//if it is not a symbol var
                continue;
            seq.clear();
            fset.clear();
            has_visited.clear();
            has_empty.clear();
            for(int j = now + 1;j < r -> var_seq.size();j++){
                seq.push_back(r -> var_seq[j]);
            }
            seq.push_back(I.seq[i].a);
            first_sym(seq,fset,has_visited,has_empty,stmts,svt);//calculate the first_set of the sym seq
            for(int j = 0; j < rl -> size();j++){//for each prod l => ?
                tmpr = rl -> at(j);
                for(std::set<int>::iterator iter = fset.begin();
                    iter != fset.end();
                    iter++){
                    tmp.set(0,l,tmpr,*iter);
                    if(!I.find(tmp))
                        I.push(tmp);
                }
            }
        }
    }while(I.size() > size);
}

void closure(   item_list&I,std::vector< std::set<int> >&ffsm,
                prods*stmts)
{
loop:
    int size;
    item_list::container::iterator iter_item;
    rightlist::list::iterator iter_right;
    right*r;
    int l;
    int now;
    rightlist*rl;
    item tmp(0,0,0,0);
    std::vector<int> seq;
    std::set<int> fset;
    std::set<int> has_visited;
    std::set<int> has_empty;
    right* tmpr = NULL;
    do{
        size = I.size();
        for(int i = 0;
            i < I.size();
            i++){
            r = I.seq[i].r;
            now = I.seq[i].now;
            l = r -> var_seq[now];
            rl = stmts -> get(l);
            if(rl == NULL)//if it is not a symbol var
                continue;
            seq.clear();
            fset.clear();
            has_visited.clear();
            has_empty.clear();
            for(int j = now + 1;j < r -> var_seq.size();j++){
                seq.push_back(r -> var_seq[j]);
            }
            seq.push_back(I.seq[i].a);
            first_sym(seq,fset,ffsm);//calculate the first_set of the sym seq
            for(int j = 0; j < rl -> size();j++){//for each prod l => ?
                tmpr = rl -> at(j);
                for(std::set<int>::iterator iter = fset.begin();
                    iter != fset.end();
                    iter++){
                    tmp.set(0,l,tmpr,*iter);
                    if(!I.find(tmp))
                        I.push(tmp);
                }
            }
        }
    }while(I.size() > size);
}

struct item_set{
    typedef std::vector<item_list> container;
    container seq;
    typedef std::vector<action_node> line;
    typedef std::vector<line> table;
    table atab;
    int num_syms;

    void set_syms(const int num)
    {
        num_syms = num;
    }

    void atab_add_one_line()
    {
        atab.reserve(atab.size() + 1);
        atab.resize(atab.size() + 1);
        atab[atab.size()-1].reserve(num_syms);
        atab[atab.size()-1].resize(num_syms);
    }

    int add(const item_list&i)
    {
        seq.push_back(i);
        atab_add_one_line();
        return seq.size() - 1;
    }

    item_list&at(const int id)
    {
        return seq.at(id);
    }

    int size()const
    {
        return seq.size();
    }

    void print()const
    {
        for(int i = 0;i < size();i++){
            std::cout<<"I"<<i<<std::endl;
            seq[i].print();
        }
        std::cout<<"I"<<"\t";
        for(int i = 0;i < num_syms;i++){
            std::cout<<i<<"\t";
        }
        std::cout<<std::endl;
        for(int i = 0;i < size();i++){
            std::cout<<"I"<<i<<"\t";
            for(int j = 0;j < num_syms;j++)
                std::cout<<(atab[i][j].t == REDUCE?'r'
                            :(atab[i][j].t == ERROR)?'e'
                            :(atab[i][j].t == SEND)?'s'
                            :' ')<<atab[i][j].where<<"\t";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
        /*Emit table*/
        std::cout<<"int cctab["<<size()<<"]["<<num_syms<<"][2]="<<"{\n";
        for(int i = 0;i < size();i++){
            std::cout<<"{";
            for(int j = 0;j < num_syms;j++){
                std::cout<<"{"<<atab[i][j].t<<","<<atab[i][j].where<<"}";
                if(j != num_syms - 1)
                    std::cout<<",";
            }
            std::cout<<"}";
            if(i != size() - 1)
                std::cout<<",";
            std::cout<<"\n";
        }
        std::cout<<"};\n";
        /*TODO:emit cc parser machine code here*/
#define print(x) std::cout<<(x);
        print("parser(){\n");
            print("stack<int> status;\n");
            print("stack<int> products;\n");
            print("int sym,top;\n");
            print("status.push(0);//push the start status\n");
            print("products.push(1);//push the end symbol to the products");
            print("sym = lexer.scan();\n");
            print("top = status.top();\n");
            print("while(true){\n");
                print("\tif(atab[top][sym][0] == SEND){\n");
                print("\tproducts.push(sym);\n");
                print("\tstatus.push(atab[top][sym][1]);\n");
                print("\tsym = lexer.scan();\n");
                print("\t}else if(atab[top][sym][0] == REDUCE){\n");
                print("\t\tif(atab[top][sym][1] == 0){//it is the accept position\n");
                print("\t\t\tbreak;\n");
                print("\t\t}\n");
                print("\t\tswitch(atab[top][sym][1]){\n");
                /*
                 *TODO:generate every production!
                 *     get the length of every product and pop the symbols in the stack
                 */
                #if 0
                for(int i = 0; i < number_of_products;i++){
                    print("case ");print(i);print(":\n{");
                    print("//reduce ");print(prod -> l);print("=>");print(prod -> r);print("\n");
                    for(int j = 0;j < length_of_product;j++)
                        print("products.pop();\n");
                    print("top = status.top();\n");
                    print("products.push(prod -> l);\n");
                    print("status.push(atab[top][prod -> l][1]);\n");
                    print("}\n");
                }
                #endif
                //print("");
                /**/
                print("\t\t}\n");
                print("\t}else{\n");
                print("\t\terror();\n");
                print("\t}\n");
            print("}\n");
        print("}//function parser()\n");
#undef print
    }

    int find(const item_list&z)const
    {
        for(int j = size() -1;j >=0;j--)
            if(z == seq[j])
                return j;
        return -1;
    }
};//struzct item_set

void closure_set(   item_set&s,item_list&I0,
                    prods*stmts,
                    const int num_syms,
                    parser::syms_var_table*svt)
{
    item_list* il;
    s.set_syms(num_syms);
    s.add(I0);
    int size;
    item* it;
    item_list tmp1;
    //for each Item Set in the s
    for(int i = 0; i < s.size();i++){
        //for each context grammer symbols
        //Because 0 is the empty symbol,ignore it
        for(int k = 1;k < num_syms;k++){
            il = &(s.at(i));
            tmp1.seq.reserve(10);
            for(int j = 0;j < il -> size();j++){
                it = &(il -> at(j));
                if(it -> now < it -> size()){
                    if(it -> at(it -> now) == k){
                        if(it -> at(it -> now) == k){
                            item tmp(it -> now +1,it -> l,it -> r,it -> a);
                            if(!tmp1.find(tmp)){
                                tmp1.push(tmp);
                                closure(tmp1,stmts,svt);
                            }
                        }
                    }
                }else{
                    if(it -> l != -1){
                        s.atab[i][k] = action_node(REDUCE,stmts -> get_id(it -> l,it -> r));
                    }else{
                        s.atab[i][k] = action_node(REDUCE,0);
                    }
                }
            }
            if(tmp1.size() > 0){
                int z;
                int f = s.find(tmp1);
                if(f < 0){
                    f = s.add(tmp1);
                }
                s.atab[i][k] = action_node(JUSTGO,f);
            }
            tmp1.clear();
        }
    }
}

void closure_set(   item_set&s,item_list&I0,
                    prods*stmts,std::vector< std::set<int> >&ffsm,
                   const int num_syms,parser::syms_var_table*svt)
{
#define sym_type(x) (svt -> at(x))
    item_list* il;
    s.set_syms(num_syms);
    s.add(I0);
    int size;
    item* it;
    item_list tmp1;
    //for each Item Set in the s
    for(int i = 0; i < s.size();i++){
        tmp1.seq.reserve(10);
        //for each context grammer symbols
        //Because 0 is the empty symbol,ignore it
        for(int k = 1;k < num_syms;k++){
            il = &(s.at(i));
            for(int j = 0;j < il -> size();j++){
                it = &(il -> at(j));
                if(it -> now < it -> size()){
                    if(it -> at(it -> now) == k){
                        if(it -> at(it -> now) == k){
                            item tmp(it -> now +1,it -> l,it -> r,it -> a);
                            if(!tmp1.find(tmp)){
                                tmp1.push(tmp);
                                closure(tmp1,ffsm,stmts);
                            }
                        }
                    }
                }else{
                    if(it -> l != -1){
                        /*it should be reduced now!*/
                        if(s.atab[i][it -> a].t != ERROR){
                            if(!(s.atab[i][it -> a].t == REDUCE && s.atab[i][it ->a].where == stmts -> get_id(it -> l,it -> r)))
                            std::cerr<<"At I"<<i<<" has conflict with "<<(s.atab[i][it -> a].t == SEND?'s'
                                                                        :(s.atab[i][it -> a].t == REDUCE)?'r'
                                                                        :'n')<<s.atab[i][it ->a].where
                                                                        <<" and r"<<stmts -> get_id(it -> l,it -> r)<<"\n";
                            //throw 78;
                        }
                        s.atab[i][it -> a] = action_node(REDUCE,stmts -> get_id(it -> l,it -> r));
                    }else{
                        /*it is the state of accept*/
                        if(k == 1)
                            s.atab[i][k] = action_node(REDUCE,0);
                    }
                }
            }
            if(tmp1.size() > 0){
                int z;
                int f = s.find(tmp1);
                if(f < 0){
                    f = s.add(tmp1);
                }
                if(s.atab[i][k].t != ERROR){
                  if(!(s.atab[i][it -> a].t == REDUCE && s.atab[i][it ->a].where == stmts -> get_id(it -> l,it -> r)))
                     std::cerr<<"At I"<<i<<" has conflict with "<<(s.atab[i][k].t == SEND?'s'
                                                                 :(s.atab[i][k].t == REDUCE)?'r'
                                                                 :'n')<<s.atab[i][k].where
                                                                <<" and s"<<f<<"\n";
                   
                    //throw 889;
                }
                s.atab[i][k] = action_node((sym_type(k)==SYM?SEND
                                            :JUSTGO),f);
            }
            tmp1.clear();
        }

#undef sym_type
    }
}

/*
#var S,L,R;
#sym EQ,STAR,i;
!S = L EQ R;
!S = R;
!L = STAR R;
!L = i;
!R = L;
*S;
 */
int main()
{
    typedef std::set<int> flag_set;
    flag_set first_set,has_visited,has_empty;
    parser::syms_var_table* svt ;
    system::iol* io = new system::kb_io;
    lexer::lexer* l = new lexer::lexer(io);
    parser* p = new parser(l);
    prods* ss ;
    try{
        ss = p -> getstmts();
    }catch(int i){
        std::cout<<i<<std::endl;
    }
    std::vector<std::set<int> > ffsm;//cache the first set of each symbol

    std::cout<<"The map table:\n";
    p -> print_def_table();
    svt = &(p -> ratable);
    ffsm.reserve(p -> number_of_symbols());
    ffsm.resize(p -> number_of_symbols());
    /*calculate each symbol's first set*/
    
    for(int i = 0; i < p -> number_of_symbols();i++){
        try{
            first_sym(  i,ffsm[i],
                    has_visited,has_empty,
                    ss,svt );
        }catch(int i){
            std::cout<<"errno:"<<i<<std::endl;
            continue;
        }
        has_visited.clear();
        has_empty.clear();
    }

    std::cout<<"The first table:\n";
    for(int i = 0;i < p -> number_of_symbols();i++){
        std::cout<<i<<":";
        for(flag_set::iterator iter = ffsm[i].begin();iter != ffsm[i].end();iter++){
            std::cout<<*iter<<" ";
        }
        std::cout<<std::endl;
    }

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
    std::cout<<"Calculate single symbol:\n";
    while(std::cin>>sym){
        if(sym < 0)
            break;

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
    std::cout<<"Calculate symbols list:\n";
    std::vector<int> syml;
    while(std::cin>>sym){
        if(sym >= 0)
            syml.push_back(sym);
        else{

            if(syml.size() == 0)
                break;
            std::cout<<"Orignal:\n";
            first_sym(syml,first_set,
                      has_visited,has_empty,
                      ss,svt);

            for(flag_set::iterator iter = first_set.begin();
                iter != first_set.end();
                iter ++){
                    std::cout<<*iter<<" ";
            }
            std::cout<<std::endl;

            std::cout<<"After Optimazing:\n";
            first_set.clear();
            first_sym(syml,first_set,ffsm);
            
            for(flag_set::iterator iter = first_set.begin();
                iter != first_set.end();
                iter ++){
                    std::cout<<*iter<<" ";
            }
            std::cout<<std::endl;

            syml.clear();
            first_set.clear();
            has_visited.clear();
            has_empty.clear();
        }
    }

    std::cout<<"Calculate the closure of the I0"<<std::endl;
#define END_SYM 1

    item_list I0;
    item tmp(0,-1,NULL,END_SYM);//the lookahead symbols is $
    for(parser::list::iterator iter = p -> begin.begin();
            iter != p -> begin.end();
            iter++){
            right*temp = new right;
            temp -> add(*iter);
            tmp.set(0,-1,temp,END_SYM);
            I0.push(tmp);
    }
    item_list I0_bk = I0;
    std::cout<<"Original:\n";
    closure(I0,ss,svt);
    I0.print();
    I0.clear();
    
    I0 = I0_bk;
    std::cout<<"After optimazing:\n";
    closure(I0,ffsm,ss);
    I0.print();

    /*
     * TODO:Clean the right allocated here
     */

#undef END_SYM

    std::cout<<"Calculate the closure sets:"<<std::endl;
    item_set is;
    
    closure_set(is,I0,ss,ffsm,p -> number_of_symbols(),svt);
    is.print();
    p -> print_def_table(); 
end:

    delete p;
    delete l;
    delete io;
    return 0;
}

