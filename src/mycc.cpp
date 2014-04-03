#include"lexer.h"
#include"kb_io.h"

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
		this->read();
		while (look->tag == '#')	//define the symbols and variable
			getdef();
		while (look->tag == '!')
			getprods();
		while (look->tag == '*')
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
		while (look->tag ==::lexer::tag::ID) {
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
    item(const int now,const int l,right*r)
    :now(now),l(l),r(r){}

    void set(const int now,const int l,right*r)
    {
        this -> now = now;
        this -> l = l;
        this -> r = r;
    }

    bool operator==(const item&z)const
    {
        return (now == z.now)&&(l==z.l)&&(*r == (*z.r));
    }

    std::string toString()const
    {
        return ::toString(l) + "->" + r-> toString()
                + "(" + ::toString(now)+ ")";
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

void calculate_closure(item_list&I,prods*stmts)
{
    int size;
    item_list::container::iterator iter_item;
    rightlist::list::iterator iter_right;
    right*r;
    rightlist*rl;
    item tmp(0,0,0);

    int l;
    int now;
    do{
        size = I.size();
        for(int i = 0;
            i < I.seq.size();
            i++){
           r = I.seq[i].r;
           now = I.seq[i].now;
           if(now < r -> var_seq.size()){
            l = r -> var_seq[now];
            rl = stmts->get(l);
            if(rl != NULL){
            for(iter_right = rl -> pro_seq.begin();
                iter_right != rl -> pro_seq.end();
                iter_right ++){
                    tmp.set(0,l,*iter_right);
                    if(!I.find(tmp)){
                        I.push(tmp);//0 is the begin
                    }
                }
            }
           }
        }
        /*#var a,b;
#sym p,m,c,lb,rb;
!a = b p a|b m a|b;
!b = lb a rb|c;
*a;
1


        */
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
        for(int i = 0;i < size();i++){
            std::cout<<"\t";
        }
        std::cout<<std::endl;
        for(int i = 0;i < size();i++){
            std::cout<<"I"<<i<<"\t";
            for(int j = 0;j < num_syms;j++)
                std::cout<<(atab[i][j].t == REDUCE?'r'
                            :(atab[i][j].t == ERROR)?'e'
                            :'s')<<atab[i][j].where<<"\t";
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

    int find(const item_list&z)const
    {
        for(int j = size() -1;j >=0;j--)
            if(z == seq[j])
                return j;
        return -1;
    }
};//struct item_set

void calculate_item_set(item_set&s,item_list&I0,
                        prods*stmts,const int num_syms)
{
    item_list*il;
    s.set_syms(num_syms);
    s.add(I0);
    int size;
    item*it;
    item_list tmp1;

//loop:
    //size = s.size();
    for(int i = 0;i < s.size();i++){

        //for each context grammer symbols
        for(int k = 0;k < num_syms;k++){
            il = &(s.at(i));
            for(int j = 0;j < il -> size();j++){
                //il = &(s.at(i));//I Should Say :BE CAREFUL!!!
                it = &(il -> at(j));
                //il -> print();
                //std::cout<<il -> at(j).toString()<<std::endl;
                //it -> print();
                //std::cout<<it<<std::endl;
                if(it -> now < it -> size()){//this symbols has not finished
                    if(it -> at(it -> now) == k){/*next symbol is k*/
                        item tmp(it -> now + 1 ,it -> l,it -> r);
                        if(!tmp1.find(tmp)){
                            tmp1.push(tmp);
                            calculate_closure(tmp1,stmts);
                        }
                    }
                }else{
                    if(it -> l != -1){
                        s.atab[i][k] = action_node(REDUCE,stmts -> get_id(it -> l,it -> r));//f is the chanshengshide id
                    }else{
                        s.atab[i][k] = action_node(REDUCE,0);//it is the accept point
                    }
                }
            }
             if(tmp1.size() > 0){
                 int z;
                 int f = s.find(tmp1);
                   if(f < 0){
                       f = s.add(tmp1);//i ----k---- z.IT should be blame
                   }
                  s.atab[i][k] = action_node(JUSTGO,f);
            }
            tmp1.clear();
            //std::cout<<i<<":"<<std::endl;
            //s.print();
            //
            //foreach item in il
            //if now < item.size() - 1 and item.at(now + 1) == k
            //-----k------> closure(item + 1) into tmp
            // It is GOTO(i,k) = s.add();
            // Find if(tmp in s)
            //if not s.add(tmp)
        }
    }

}
/*
#var e,t,f;
#sym id,l,r,p,m;
!e = e p t|t;
!t = t m f|f;
!f = l e r|id;
*e;
*/
int main()
{
	system::iol * con_io = new system::kb_io;
	lexer::lexer * l = new lexer::lexer(con_io);
	parser p(l);
	prods::map::iterator iter;
	right expand;
    item tmp(0,-1,&expand);
    item_list I0;

    prods *ss ;
    try{
        ss  = p.getstmts();
    }catch(int e){
        std::cerr<<e<<std::endl;
    }catch(...){
        throw;
    }

    for(iter = ss -> seq.begin();
        iter != ss -> seq.end();
        iter++){
        std::cout<<"\t";
        if(p.begin.count(iter -> first)>0)
            std::cout<<">>";
        std::cout<<iter -> first<<" -> \t";
        std::cout<<iter->second->toString();
        std::cout<<"\n";
    }

    for(parser::list::iterator it = p.begin.begin();
        it != p.begin.end();
        it++){
        expand.add(*it);
        break;
    }

    I0.push(tmp);

    calculate_closure(I0,ss);

    item_set iset;
    calculate_item_set(iset,I0,ss,p.number_of_symbols());

    std::cout<<"I0:"<<std::endl;
    //I0.print();

    std::cout<<"OK:"<<std::endl;
    iset.print();
	delete l;
	delete con_io;
	return 0;
}
