#ifndef _PARSER_H
#define _PARSER_H
#include"lexer.h"
#include"tag.h"
#include"token.h"
#include"env.h"
#include"stmts.h"
#include"stmt.h"
#include"inter.arith.h"
#include"inter.nodes.h"
#include"expr.h"
#include"decls.h"
#include"arrexp.h"
#include"strexp.h"

#include<stack>
#include<string>
#define _LSCC_SHIFT 1
#define _LSCC_ERROR 3
#define _LSCC_REDUCE 0
#if 1				//symbols define
#define _LSCC_program 2
#define _LSCC_block 3
#define _LSCC_decls 4
#define _LSCC_decl 5
#define _LSCC_init_decl 6
#define _LSCC_idlist 7
#define _LSCC_declarator 8
#define _LSCC_array_decl 9
#define _LSCC_compare 10
#define _LSCC_selection 11
#define _LSCC_iteration 12
#define _LSCC_init_list 13
#define _LSCC_stmts 14
#define _LSCC_stmt 15
#define _LSCC_expr 16
#define _LSCC_bool_ 17
#define _LSCC_join 18
#define _LSCC_equality 19
#define _LSCC_rel 20
#define _LSCC_add 21
#define _LSCC_term_ 22
#define _LSCC_unary 23
#define _LSCC_postfix 24
#define _LSCC_factor 25
#define _LSCC_LCB 26
#define _LSCC_RCB 27
#define _LSCC_LSB 28
#define _LSCC_RSB 29
#define _LSCC_COMMA 30
#define _LSCC_SEM 31
#define _LSCC_ID 32
#define _LSCC_BREAK 33
#define _LSCC_BASIC 34
#define _LSCC_NUM 35
#define _LSCC_IF 36
#define _LSCC_ELSE 37
#define _LSCC_FOR 38
#define _LSCC_DO 39
#define _LSCC_WHILE 40
#define _LSCC_REAL 41
#define _LSCC_TRUE 42
#define _LSCC_FALSE 43
#define _LSCC_LB 44
#define _LSCC_RB 45
#define _LSCC_INC 46
#define _LSCC_DEC 47
#define _LSCC_PLUS 48
#define _LSCC_MIN 49
#define _LSCC_DIV 50
#define _LSCC_MULT 51
#define _LSCC_ASS 52
#define _LSCC_OR 53
#define _LSCC_AND 54
#define _LSCC_GT 55
#define _LSCC_GE 56
#define _LSCC_NE 57
#define _LSCC_EQ 58
#define _LSCC_LS 59
#define _LSCC_LE 60
#define _LSCC_NOT 61
#define _LSCC_PRINTF_FUNC 62
#define _LSCC_STR 63
#define _LSCC_RETURN 64
#endif				//symbols define

#define assxtoy(x,y) {\
                            union{typeof(x) i;typeof(y) i2;}tmp;\
                            tmp.i = x;\
                            y = tmp.i2;\
                            }

class parser {
    private:
    static const int cctab[668][65][2];
    lexer::lexer*   lex;
    lexer::token*   look;
    symbols::env*   env;

    private:
    int getc()
    {
#if 1//symbols map
        look = lex -> scan();
        lexer::tag_t t = look->tag;
        switch (t) {
        case -1:return 1;//end symbol
        case '{':return _LSCC_LCB;
        case '}':return _LSCC_RCB;
        case '[':return _LSCC_LSB;
        case ']':return _LSCC_RSB;
        case ',':return _LSCC_COMMA;
        case ';':return _LSCC_SEM;
        case lexer::tag::ID:return _LSCC_ID;
        case lexer::tag::BREAK:return _LSCC_BREAK;
        case lexer::tag::BASIC:return _LSCC_BASIC;
        case lexer::tag::NUM:return _LSCC_NUM;
        case lexer::tag::IF:return _LSCC_IF;
        case lexer::tag::ELSE:return _LSCC_ELSE;
        case lexer::tag::FOR:return _LSCC_FOR;
        case lexer::tag::DO:return _LSCC_DO;
        case lexer::tag::WHILE:return _LSCC_WHILE;
        case lexer::tag::REAL:return _LSCC_REAL;
        case lexer::tag::TRUE:return _LSCC_TRUE;
        case lexer::tag::FALSE:return _LSCC_FALSE;
        case '(':return _LSCC_LB;
        case ')':return _LSCC_RB;
        case lexer::tag::INC:return _LSCC_INC;
        case lexer::tag::DEC:return _LSCC_DEC;
        case '+':return _LSCC_PLUS;
        case '-':return _LSCC_MIN;
        case '/':return _LSCC_DIV;
        case '*':return _LSCC_MULT;
        case '=':return _LSCC_ASS;
        case lexer::tag::OR:return _LSCC_OR;
        case lexer::tag::AND:return _LSCC_AND;
        case '>':return _LSCC_GT;
        case lexer::tag::GE:return _LSCC_GE;
        case lexer::tag::NE:return _LSCC_NE;
        case lexer::tag::EQ:return _LSCC_EQ;
        case '<':return _LSCC_LS;
        case lexer::tag::LE:return _LSCC_LE;
        case '!':return _LSCC_NOT;
        case lexer::tag::PRINTF_FUNC:return _LSCC_PRINTF_FUNC;
        case lexer::tag::STR:return _LSCC_STR;
        case lexer::tag::RETURN:return _LSCC_RETURN;
        default:return 0;
        }
#endif                //symbols map
    } //getc()

    public:
    parser(lexer::lexer*lex)
    :lex(lex),look(NULL),env(new symbols::env(NULL))
    {}

    ~parser()
    {
        delete env;
    }

    int match(const lexer::tag_t what)
    {
        if(look -> tag != what)
            this -> error("Syntax error");
        else
            this -> getc();
    }
    public:

    int do_parse()
    {
        std::stack < int >status;
        std::stack < int >products;
        int sym, top;
        int left;
        status.push(0);    //push the start status
        products.push(1);    //push the end symbols
        top = status.top();
        /*skip nonsense code!*/
        getc();
        while(look -> tag != lexer::tag::BASIC)
            getc();
        getc();
        match(lexer::tag::MAIN_ID);
        match('(');
        while(look -> tag != ')'){
            getc();
        }
        sym = getc();
        while (true) {
            switch (cctab[top][sym][0]) {
            case _LSCC_SHIFT:{
                    union{void* vtok;int vint;}tmp;
                    tmp.vtok = look;
                    products.push(tmp.vint);
                    status.push(cctab[top][sym][1]);
                    sym = getc();
                    top = status.top();
                    break;
                }
            case _LSCC_REDUCE:{
                    int l;

					switch (cctab[top][sym][1]) {
					case 0:{//accept
							return 0;
						}
					case 1:{//program=>block
                            inter::block* blk;
                            assxtoy(products.top(),blk);
							products.pop();status.pop();
							blk -> emit(".globl main\n");
							blk -> emit("main:\n");
							blk -> gen();
							l = 2;
							break;
						}
					case 2:{//block=>LCB decls stmts RCB
                            inter::block* blk = new inter::block;
                            products.pop();status.pop();//RCB
                            assxtoy(products.top(),blk -> ss);
                            products.pop();status.pop();//decls
                            assxtoy(products.top(),blk -> ds);
                            products.pop();status.pop();//stmts
                            products.pop();status.pop();//LCB
                            assxtoy(blk,left);
                            //blk -> gen();
							l = 3;
							break;
						}
					case 3:{//decls=>decls decl
                            inter::decls* ds = new inter::decls;
                            assxtoy(products.top(),ds -> d2);
                            products.pop();status.pop();
                            assxtoy(products.top(),ds -> d1);
                            products.pop();status.pop();
                            assxtoy(ds,left);
							l = 4;
							break;
						}
					case 4:{//decls=>%
						    inter::decls* ds = new inter::decls;
                            ds -> d1 = NULL;
                            ds -> d2 = NULL;
                            assxtoy(ds,left);
							l = 4;
							break;
						}
					case 5:{//decl=>BASIC idlist SEM
                             /*
                              *TODO:for each id in id list
                              *     input the id into the env.
                              */
                            inter::decl* dc = new inter::decl;

                            products.pop();status.pop();
                            assxtoy(products.top(),dc -> idl);
                            products.pop();status.pop();
                            assxtoy(products.top(),dc -> t);
                            products.pop();status.pop();
                            //dc -> gen();
                            for(int i = 0;i < dc -> idl -> list.size();i++){
                                inter::declarator* dclt = dc -> idl -> list.at(i);
                                inter::id* t = dclt -> produce(dc -> t);
                                if(env -> get(dclt -> name))
                                    this -> error(std::string("Variable ") + dclt -> name -> lexme + " redefined");/*TODO:lineno*/
                                env -> put(dclt -> name,t);
                            }
                            //dc -> idl -> gen(dc -> t);
                            assxtoy(dc,left);
							l = 5;
							break;
						}
					case 6:{//init_list=>init_list COMMA expr
                            std::vector<inter::expr*>* initlist ;
                            inter::expr* e;
                            assxtoy(products.top(),e);
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),initlist);
                            initlist -> push_back(e);
                            products.pop();status.pop();
                            assxtoy(initlist,left);
							l = 13;
							break;
						}
					case 7:{//init_list=>expr
                            std::vector<inter::expr*>* initlist = new std::vector<inter::expr*>;
                            inter::expr* e;
                            assxtoy(products.top(),e);
                            initlist -> push_back(e);
                            products.pop();status.pop();
                            assxtoy(initlist,left);
							l = 13;
							break;
						}
					case 8:{//idlist=>idlist COMMA declarator
                            inter::single_id* nid;
                            inter::idlist* idl;
                            assxtoy(products.top(),nid);
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),idl);
                            idl-> list.push_back(nid);
                            products.pop();status.pop();
                            assxtoy(idl,left);
							l = 7;
							break;
						}
					case 9:{//idlist=>declarator
                            inter::single_id* nid ;
                            inter::idlist* idl = new inter::idlist;
                            assxtoy(products.top(),nid);
                            idl-> list.push_back(nid);
                            products.pop();status.pop();
                            assxtoy(idl,left);
							l = 7;
							break;
						}
					case 10:{//declarator=>ID
                            inter::single_id* nid = new inter::single_id;
                            nid -> init = NULL;
                            assxtoy(products.top(),nid -> name);
                            products.pop();status.pop();
                            assxtoy(nid,left);
							l = 8;
							break;
						}
					case 11:{//declarator=>ID ASS expr
                            inter::single_id* nid = new inter::single_id;
                            assxtoy(products.top(),nid -> init);
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),nid -> name);
                            products.pop();status.pop();
                            assxtoy(nid,left);
                            //nid -> gen();
							l = 8;
							break;
						}
					case 12:{//declarator=>array_decl
                            left = products.top();
                            products.pop();status.pop();
							l = 8;
							break;
						}
					case 13:{//array_decl=>ID LSB NUM RSB
                            inter::arr_decl* arr = new inter::arr_decl;
                            products.pop();status.pop();//RSB
                            assxtoy(products.top(),arr -> size);
                            products.pop();status.pop();//NUM
                            products.pop();status.pop();//LSB
                            assxtoy(products.top(),arr -> name);
                            products.pop();status.pop();//ID
                            assxtoy(arr,left);
							l = 9;
							break;
						}
					case 14:{//array_decl=>ID LSB NUM RSB ASS LCB init_list RCB
                            inter::arr_decl*arr = new inter::arr_decl;
                            products.pop();status.pop();//RCB
                            assxtoy(products.top(),arr -> initlist);
                            products.pop();status.pop();//init_list
                            products.pop();status.pop();//LCB
                            products.pop();status.pop();//ASS
                            products.pop();status.pop();//RSB
                            assxtoy(products.top(),arr -> size);
                            products.pop();status.pop();//NUM
                            products.pop();status.pop();//LSB
                            assxtoy(products.top(),arr -> name);
                            products.pop();status.pop();//ID
                            assxtoy(arr,left);
							l = 9;
							break;
						}
					case 15:{//stmts=>stmts stmt
                            inter::stmts*ss = new inter::stmts(NULL,NULL);
                            assxtoy(products.top(),ss -> s2);
                            products.pop();status.pop();//stmt
                            assxtoy(products.top(),ss -> s1);
                            products.pop();status.pop();//stmts
                            assxtoy(ss,left);
							l = 14;
							break;
						}
					case 16:{//stmts=>%
						    inter::stmts* s = new inter::stmts(NULL,NULL);
                            assxtoy(s, left);
							l = 14;
							break;
						}
					case 17:{//stmt=>selection
                            left = products.top();
                            products.pop();status.pop();
							l = 15;
							break;
						}
					case 18:{//stmt=>iteration
                            left = products.top();
                            products.pop();status.pop();
							l = 15;
							break;
						}
					case 19:{//stmt=>block
                            left = products.top();
                            products.pop();status.pop();
							l = 15;
							break;
						}
					case 20:{//stmt=>expr SEM
                            inter::sexpr*s = new inter::sexpr;
                            products.pop();status.pop();
                            assxtoy(products.top(),s->e);
                            products.pop();status.pop();
                            assxtoy(s,left);
							l = 15;
							break;
						}
					case 21:{//stmt=>PRINTF_FUNC LB init_list RB SEM
                            inter::func_printf* fp = new inter::func_printf;
							products.pop();status.pop();//SEM
							products.pop();status.pop();//RB
							assxtoy(products.top(),fp -> paralist);
							products.pop();status.pop();//init_list
							products.pop();status.pop();//LB
							products.pop();status.pop();//PRINTF_FUNC
							assxtoy(fp,left);
							l = 15;
							break;
						}
					case 22:{//iteration=>WHILE LB expr RB stmt
                            inter::while_* w = new inter::while_;
                            assxtoy(products.top(),w -> s);
                            products.pop();status.pop();//stmt
                            products.pop();status.pop();
                            assxtoy(products.top(),w -> c);
                            products.pop();status.pop();//expr
                            products.pop();status.pop();
                            products.pop();status.pop();//WHILE
                            assxtoy(w,left);
							l = 12;
							break;
						}
					case 23:{//iteration=>DO stmt WHILE LB expr RB SEM
                            inter::dowhile* dw = new inter::dowhile;
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),dw -> c);
                            products.pop();status.pop();//expr
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),dw -> s);
                            products.pop();status.pop();//stmt
                            products.pop();status.pop();
                            assxtoy(dw,left);
							l = 12;
							break;
						}
					case 24:{//iteration=>FOR LB expr SEM expr SEM expr RB stmt
                            inter::for_*  f = new inter::for_;
                            assxtoy(products.top(),f -> s);
                            products.pop();status.pop();//stmt
                            products.pop();status.pop();
                            assxtoy(products.top(),f -> e2);
                            products.pop();status.pop();//e2
                            products.pop();status.pop();
                            assxtoy(products.top(),f -> c);
                            products.pop();status.pop();//c
                            products.pop();status.pop();
                            assxtoy(products.top(),f -> e1);
                            products.pop();status.pop();//e1
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(f,left);
							l = 12;
							break;
						}
					case 25:{//selection=>IF LB expr RB stmt
                            inter::if_* i_ = new inter::if_;
                            assxtoy(products.top(),i_ -> s);
                            products.pop();status.pop();//stmt
                            products.pop();status.pop();
                            assxtoy(products.top(),i_ -> c);
                            products.pop();status.pop();//expr
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(i_,left);
							l = 11;
							break;
						}
					case 26:{//selection=>IF LB expr RB stmt ELSE stmt
                            inter::ifelse* ie = new inter::ifelse;
                            assxtoy(products.top(),ie -> sfalse);
                            products.pop();status.pop();//stmt
                            products.pop();status.pop();
                            assxtoy(products.top(),ie -> strue);
                            products.pop();status.pop();//stmt
                            products.pop();status.pop();
                            assxtoy(products.top(),ie -> c);
                            products.pop();status.pop();//expr
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(ie,left);
							l = 11;
							break;
						}
					case 27:{//factor=>ID
                            lexer::word*w;
                            inter::id* t;
                            assxtoy(products.top(),w);
                            t = env -> get(w);
                            if(!t)
                                this -> error("Variable "+w -> lexme + " not defined previously");
                            products.pop();status.pop();
                            assxtoy(t,left);
							l = 25;
							break;
						}
					case 28:{//factor=>NUM
                            inter::constant* _const = new inter::constant;
                            assxtoy(products.top(), _const -> op);
                            products.pop();status.pop();
                            assxtoy(_const, left);
							l = 25;
							break;
						}
					case 29:{//factor=>LB expr RB
                            products.pop();status.pop();
                            left = products.top();
                            products.pop();status.pop();
                            products.pop();status.pop();
							l = 25;
							break;
						}
					case 30:{//factor=>STR
                            inter::strexp* str = new inter::strexp(NULL);
                            assxtoy(products.top(),str -> str_tok);
							products.pop();status.pop();
							assxtoy(str,left);
							l = 25;
							break;
						}
					case 31:{//postfix=>factor
                            left = products.top();
                            products.pop();status.pop();
							l = 24;
							break;
						}
					case 32:{//postfix=>factor LSB expr RSB
                            inter::id* nid;
                            inter::expr*idx;
                            products.pop();status.pop();//RSB
                            assxtoy(products.top(),idx);
                            products.pop();status.pop();//expr
                            products.pop();status.pop();//LSB
                            assxtoy(products.top(),nid);
                            products.pop();status.pop();//factor
                            assxtoy(new inter::arrexp(nid,idx),left);
							l = 24;
							break;
						}
					case 33://postfix=>factor INC
					case 34:{//postfix=>factor DEC
                            inter::postfix* p = new inter::postfix;
                            assxtoy(products.top(), p -> op);
                            products.pop();status.pop();
                            assxtoy(products.top(), p -> e);
                            products.pop();status.pop();
                            assxtoy(p, left);
							l = 24;
							break;
					    }
					case 35:{	//unary=>postfix
                            left = products.top();
                            products.pop();status.pop();
							l = 23;
							break;
					}
					case 36://unary=>NOT unary
					case 37://unary=>MIN unary
					case 38://unary=>INC unary
					case 39:{//unary=>DEC unary
                            inter::unary* u = new inter::unary;
                            /*TODO:check type!*/
                            assxtoy(products.top(), u -> e);
                            products.pop();status.pop();
                            assxtoy(products.top(), u -> op);
                            products.pop();status.pop();
                            assxtoy(u, left);
							l = 23;
							break;
					    }
					case 40://term_=>term_ MULT unary
					case 41:{//term_=>term_ DIV unary
                            inter::arith*_arith = new inter::arith;
                            assxtoy(products.top(), _arith -> e2);
                            products.pop();status.pop();
                            assxtoy(products.top(), _arith -> op);
                            products.pop();status.pop();
                            assxtoy(products.top(), _arith -> e1);
                            products.pop();status.pop();
                            assxtoy(_arith,left);
							l = 22;
							break;
					    }
					case 42:{//term_=>unary
                            left = products.top();
                            products.pop();status.pop();
							l = 22;
							break;
					    }
					case 43://add=>add PLUS term_
					case 44:{//add=>add MIN term_
                            inter::arith*_arith = new inter::arith;
                            assxtoy(products.top(), _arith -> e2);
                            products.pop();status.pop();
                            assxtoy(products.top(), _arith -> op);
                            products.pop();status.pop();
                            assxtoy(products.top(), _arith -> e1);
                            products.pop();status.pop();
                            assxtoy(_arith,left);
							l = 21;
							break;
					    }
					case 45:{//add=>term_
                            left = products.top();
                            products.pop();status.pop();
							l = 21;
							break;
					    }
					case 46://rel=>rel GE add
					case 47://rel=>rel LS add
					case 48://rel=>rel GT add
					case 49:{//rel=>rel LE add
                            inter::arith*_arith = new inter::arith;
                            assxtoy(products.top(),_arith -> e2);
                            products.pop();status.pop();
                            assxtoy(products.top(),_arith -> op);
                            products.pop();status.pop();
                            assxtoy(products.top(),_arith -> e1);
                            products.pop();status.pop();
                            assxtoy(_arith,left);
							l = 20;
							break;
					    }
					case 50:{//rel=>add
                            left = products.top();
                            products.pop();status.pop();
							l = 20;
							break;
					    }
					case 51://equality=>equality EQ rel
					case 52:{//equality=>equality NE rel
                            inter::arith*_arith = new inter::arith;
                            assxtoy(products.top(),_arith -> e2);
                            products.pop();status.pop();
                            assxtoy(products.top(),_arith -> op);
                            products.pop();status.pop();
                            assxtoy(products.top(),_arith -> e1);
                            products.pop();status.pop();
                            assxtoy(_arith,left);
                            l = 19;
							break;
					    }
					case 53:{	//equality=>rel
                            left = products.top();
                            products.pop();status.pop();
							l = 19;
							break;
					    }
					case 54:{//join=>join AND equality
                            inter::and_* _and = new inter::and_;
                            assxtoy(products.top(),_and -> e2);
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),_and -> e1);
                            products.pop();status.pop();
                            assxtoy(_and,left);
                            l = 18;
							break;
					    }
					case 55:{//join=>equality
                            left = products.top();
                            products.pop();status.pop();
							l = 18;
							break;
					    }
					case 56:{//bool_=>bool_ OR join
                            inter::or_* _or = new inter::or_;
                            assxtoy(products.top(),_or -> e2);
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(products.top(),_or -> e1);
                            products.pop();status.pop();
                            assxtoy(_or,left);
							l = 17;
							break;
					    }
					case 57:{	//bool_=>join
                            left = products.top();
                            products.pop();status.pop();
							l = 17;
							break;
					    }
					case 58:{//expr=>unary ASS expr
                            inter::arith*ass = new inter::arith;
                            assxtoy(products.top(),ass -> e2);
                            products.pop();status.pop();
                            assxtoy(products.top(),ass -> op);
                            products.pop();status.pop();
                            assxtoy(products.top(),ass -> e1);
                            products.pop();status.pop();
                            assxtoy(ass,left);
							l = 16;
							break;
					    }
					case 59:{	//expr=>bool_
                            left = products.top();
                            products.pop();status.pop();

							l = 16;
							break;
					    }
                     case 60:{//stmt=>RETURN expr SEM
                            /*ignore!*/
                            inter::stmts* s = new inter::stmts(NULL,NULL);
                            products.pop();status.pop();
                            products.pop();status.pop();
                            products.pop();status.pop();
                            assxtoy(s,left);
                            l = 15;
                            break;
                        }
					}
                    top = status.top();
                    status.push(cctab[top][l][1]);
                    products.push(left);
                    top = status.top();
                    break;
                }
            default:
                {
                    std::cerr<<"At line "<<lexer::lexer::lineno<<":";
                    error("Syntax error");
                }
            }
        }
        return 1;
    }            //do_parse


    private:
    void error(const std::string errinfo)
    {
        throw errinfo;
    }

};//class parser
#endif//_PARSER_H
