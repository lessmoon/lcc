#ifndef _PARSER_H
#define _PARSER_H
#include"lexer.h"
#include"tag.h"
#include"token.h"

#include<stack>
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
#endif				//symbols define
class parser {
	static const int cctab[545][62][2];
	lexer::lexer*   lex;
    lexer::token*          look;
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
		default:return 0;
		}
#endif				//symbols map
	} //getc()
    int do_parse()
    {
		std::stack < int >status;
		std::stack < int >products;
		int sym, top;
		status.push(0);	//push the start status
		products.push(1);	//push the end symbols
		top = status.top();
		sym = getc();
		while (true) {
			switch (cctab[top][sym][0]) {
			case _LSCC_SHIFT:
				{
					products.push(sym);
					status.push(cctab[top][sym][1]);
					sym = getc();
					top = status.top();
					break;
				}
			case _LSCC_REDUCE:
				{
					int l;
					switch (cctab[top][sym][1]) {
					case 0:
						{	//accept
							break;
						}
					case 1:
						{	//program=>block 
							products.pop();
							status.pop();
							l = 2;
							break;
						}
					case 2:
						{	//block=>LCB decls stmts RCB 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 3;
							break;
						}
					case 3:
						{	//decls=>decls decl 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 4;
							break;
						}
					case 4:
						{	//decls=>% 
							products.pop();
							status.pop();
							l = 4;
							break;
						}
					case 5:
						{	//decl=>BASIC idlist SEM 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 5;
							break;
						}
					case 6:
						{	//init_list=>init_list COMMA expr 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 13;
							break;
						}
					case 7:
						{	//init_list=>expr 
							products.pop();
							status.pop();
							l = 13;
							break;
						}
					case 8:
						{	//idlist=>idlist COMMA declarator 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 7;
							break;
						}
					case 9:
						{	//idlist=>declarator 
							products.pop();
							status.pop();
							l = 7;
							break;
						}
					case 10:
						{	//declarator=>ID 
							products.pop();
							status.pop();
							l = 8;
							break;
						}
					case 11:
						{	//declarator=>ID ASS expr 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 8;
							break;
						}
					case 12:
						{	//declarator=>array_decl 
							products.pop();
							status.pop();
							l = 8;
							break;
						}
					case 13:
						{	//array_decl=>ID LSB NUM RSB 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 9;
							break;
						}
					case 14:
						{	//array_decl=>ID LSB NUM RSB ASS LCB init_list RCB 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 9;
							break;
						}
					case 15:
						{	//stmts=>stmts stmt 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 14;
							break;
						}
					case 16:
						{	//stmts=>% 
							products.pop();
							status.pop();
							l = 14;
							break;
						}
					case 17:
						{	//stmt=>selection 
							products.pop();
							status.pop();
							l = 15;
							break;
						}
					case 18:
						{	//stmt=>iteration 
							products.pop();
							status.pop();
							l = 15;
							break;
						}
					case 19:
						{	//stmt=>BREAK SEM 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 15;
							break;
						}
					case 20:
						{	//stmt=>block 
							products.pop();
							status.pop();
							l = 15;
							break;
						}
					case 21:
						{	//stmt=>expr SEM 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 15;
							break;
						}
					case 22:
						{	//iteration=>WHILE LB expr RB stmt 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 12;
							break;
						}
					case 23:
						{	//iteration=>DO stmt WHILE LB expr RB SEM 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 12;
							break;
						}
					case 24:
						{	//iteration=>FOR LB expr SEM expr SEM expr RB stmt 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 12;
							break;
						}
					case 25:
						{	//selection=>IF LB expr RB stmt 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 11;
							break;
						}
					case 26:
						{	//selection=>IF LB expr RB stmt ELSE stmt 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 11;
							break;
						}
					case 27:
						{	//factor=>ID 
							products.pop();
							status.pop();
							l = 25;
							break;
						}
					case 28:
						{	//factor=>NUM 
							products.pop();
							status.pop();
							l = 25;
							break;
						}
					case 29:
						{	//factor=>LB expr RB 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 25;
							break;
						}
					case 30:
						{	//postfix=>factor 
							products.pop();
							status.pop();
							l = 24;
							break;
						}
					case 31:
						{	//postfix=>factor LSB expr RSB 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 24;
							break;
						}
					case 32:
						{	//postfix=>factor INC 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 24;
							break;
						}
					case 33:
						{	//postfix=>factor DEC 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 24;
							break;
						}
					case 34:
						{	//unary=>postfix 
							products.pop();
							status.pop();
							l = 23;
							break;
						}
					case 35:
						{	//unary=>NOT unary 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 23;
							break;
						}
					case 36:
						{	//unary=>MIN unary 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 23;
							break;
						}
					case 37:
						{	//unary=>INC unary 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 23;
							break;
						}
					case 38:
						{	//unary=>DEC unary 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 23;
							break;
						}
					case 39:
						{	//term_=>term_ MULT unary 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 22;
							break;
						}
					case 40:
						{	//term_=>term_ DIV unary 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 22;
							break;
						}
					case 41:
						{	//term_=>unary 
							products.pop();
							status.pop();
							l = 22;
							break;
						}
					case 42:
						{	//add=>add PLUS term_ 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 21;
							break;
						}
					case 43:
						{	//add=>add MIN term_ 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 21;
							break;
						}
					case 44:
						{	//add=>term_ 
							products.pop();
							status.pop();
							l = 21;
							break;
						}
					case 45:
						{	//rel=>rel GT add 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 20;
							break;
						}
					case 46:
						{	//rel=>rel LS add 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 20;
							break;
						}
					case 47:
						{	//rel=>rel GT add 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 20;
							break;
						}
					case 48:
						{	//rel=>rel LE add 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 20;
							break;
						}
					case 49:
						{	//rel=>add 
							products.pop();
							status.pop();
							l = 20;
							break;
						}
					case 50:
						{	//equality=>equality EQ rel 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 19;
							break;
						}
					case 51:
						{	//equality=>equality NE rel 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 19;
							break;
						}
					case 52:
						{	//equality=>rel 
							products.pop();
							status.pop();
							l = 19;
							break;
						}
					case 53:
						{	//join=>join AND equality 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 18;
							break;
						}
					case 54:
						{	//join=>equality 
							products.pop();
							status.pop();
							l = 18;
							break;
						}
					case 55:
						{	//bool_=>bool_ OR join 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 17;
							break;
						}
					case 56:
						{	//bool_=>join 
							products.pop();
							status.pop();
							l = 17;
							break;
						}
					case 57:
						{	//expr=>unary ASS expr 
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							products.pop();
							status.pop();
							l = 16;
							break;
						}
					case 58:
						{	//expr=>bool_ 
							products.pop();
							status.pop();
							l = 16;
							break;
						}
					}
					top = status.top();
					status.push(cctab[top][l][1]);
					products.push(l);
					top = status.top();
				}
			default:
				{
					error();
				}
			}
		}
	}			//do_parse

	void error()
    {

	}

};//class parser
#endif//_PARSER_H
