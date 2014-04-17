#include"parser.h"

namespace ccparser{
   /*class parser*/
   
   parser::parser(lexer* lex)
   :res(NULL),def_table(NULL),type_table(NULL),start_symbols(NULL),
   look(NULL),productions(NULL){}
   
   parser::~parser()
   {
        /*clear the things created by itself*/
        if(res != NULL)
            delete res;
   }

   void parser::read()
   {
        lex -> scan();
   }
   
   bool parser::match(const tag_t tag)
   {
        if(look -> tag == tag){
            this -> read();
            return true;
        } else {
            this -> error("Failed to match ");/*TODO*/
        }
   }
   
   void parser::error(const string&info)
   {
        /*TODO throw exception*/
        throw info;
   }
   
   void parser::do_def(const string& name ,const TYPE t)
   {
        if (def_table -> find(name) != def_table -> end())
			this -> error(name + " redefined");
		else{
			def_table -> insert(  symbol_table::value_type(name, 
			                    type_table -> size())   );
            type_table -> push_back(t);
        }
   }
   
   TYPE parser::sym_type(const string& name)const
   {
   		symbol_table::const_iterator iter = def_table -> find(name);
		if (iter == def_table -> end())
			return UNKNOWN;
		else
			return (type_table -> at(iter -> second));
   }
   
   result*  parser::stmts()
   {
        /*delete the result if it had before*/
        if(res != NULL)
            delete res;
        /*do the dirty work!*/
        res = new result;
        def_table       = &(res -> def_table);
        type_table      = &(res -> type_table);
        start_symbols   = &(res -> start_symbols);
        productions     = &(res -> productions);
        /*empty symbol,using 0*/
        this -> do_def("%",TERM);
        /*The end symbol of the input stream using 1*/
        this -> do_def("$",TERM);
        this -> read();
        while(look -> tag == '#')
            this -> def();
        while(look -> tag == '!')
            this -> prods();
        while(look -> tag == '*')
            this -> begin();
        return res;
   }
   
   void parser::begin()
   {
        string tmp;
        iterator iter;
        match('*');
loop:
        tmp = this -> toString(); 
        switch(this -> sym_type(tmp)){
        case VAR:
            break;
        case TERM:
            this -> error("The symbol `" + tmp + "' is terminator,but variable required here");
            break;
        case UNKNOWN:
        default:
            this -> error("Undefined symbols `" + tmp + "' found here");
        }
        iter = def_table -> find(tmp);
        this -> read();
        if(look -> tag == ','){
            this -> read();
            goto loop;
        }
        match(';');
   }
   
   void parser::def()
   {
        TYPE t;
        match('#');
        if(look -> tag == ::lexer::tag::VAR){
            t = VAR;
        } else if ( look -> tag == ::lexer::tag::TERM ){
            t = TERM;
        } else {
            this -> error( "Unknown operation");
        }
        do{
            this -> read();
            this -> do_def(this -> toString(),t);
            this -> read();
        } while(look -> tag == ',');
        
        match(';');
   }

   void parser::prods()
   {
        right* tmp;
        string leftname ;
        match('!');
        leftname = toString();

        switch(this -> sym_type(leftname)){
        case VAR:
            break;
        case TERM:
            this -> error("The symbol `" + leftname + "' is terminator,but variable required here");
            break;
        case UNKNOWN:
        default:
            this -> error("Undefined symbols `" + leftname + "' found here");
        }
        this -> read();
        match('=');
loop:
        tmp = prod();
        productions -> add(def_table -> find(leftname) -> second,tmp);
        if( look -> tag == '|' ){
            this -> read();
            goto loop;       
        }
        match(';');
   }

   parser::right* parser::prod()
   {
        string tmp;
        right* res = new right;
        try{
		    while (look -> tag ==::lexer::tag::ID || look -> tag == '%') {
		        tmp = this -> toString();
			    if (sym_type(tmp) == UNKNOWN)	
			        //not defined
				   this -> error("Undefined symbols `" + tmp + "' found here");
			    else
				    res -> add(def_table -> find(tmp) -> second);
			    this -> read();
		    }
		}catch(...){
		    /*Of course do the clear work in case!*/
		    delete res;
		    throw;
		}
		return res;
   }

    parser::string parser::toString()
    {
        if(look -> tag == '%' || look -> tag == '$')
            return std::string("")+char(look -> tag);
        else if(look -> tag == ::lexer::tag::ID)
		    return (static_cast < ::lexer::word* >(look))->lexme;
		else
		    this -> error("Unknown error");
    }

};//namespace parser
