#ifndef _PARSER_H
#define _PARSER_H
#include"lexer.h"

#include<stack>

#define SHIFT 1
#define ERROR 3
#define REDUCE 0
#if 1//symbols define
#define program 2
#define block 3
#define decls 4
#define decl 5
#define init_decl 6
#define idlist 7
#define declarator 8
#define array_decl 9
#define compare 10
#define selection 11
#define iteration 12
#define init_list 13
#define stmts 14
#define stmt 15
#define expr 16
#define bool_ 17
#define join 18
#define equality 19
#define rel 20
#define add 21
#define term_ 22
#define unary 23
#define postfix 24
#define factor 25
#define LCB 26
#define RCB 27
#define LSB 28
#define RSB 29
#define COMMA 30
#define SEM 31
#define ID 32
#define BREAK 33
#define BASIC 34
#define NUM 35
#define IF 36
#define ELSE 37
#define FOR 38
#define DO 39
#define WHILE 40
#define REAL 41
#define TRUE 42
#define FALSE 43
#define LB 44
#define RB 45
#define INC 46
#define DEC 47
#define PLUS 48
#define MIN 49
#define DIV 50
#define MULT 51
#define ASS 52
#define OR 53
#define AND 54
#define GT 55
#define GE 56
#define NE 57
#define EQ 58
#define LS 59
#define LE 60
#define NOT 61
#endif//symbols define
class parser{
static const int cctab[545][62][2];
int getc(){
#if 1//symbols map
lexer::tag_t t = gettok();
switch(t){
case -1: return 1;//end symbol
case '{': return LCB;
case '}': return RCB;
case '[': return LSB;
case ']': return RSB;
case ',': return COMMA;
case ';': return SEM;
case lexer::tag::ID: return ID;
case lexer::tag::BREAK: return BREAK;
case lexer::tag::BASIC: return BASIC;
case lexer::tag::NUM: return NUM;
case lexer::tag::IF: return IF;
case lexer::tag::ELSE: return ELSE;
case lexer::tag::FOR: return FOR;
case lexer::tag::DO: return DO;
case lexer::tag::WHILE: return WHILE;
case lexer::tag::REAL: return REAL;
case lexer::tag::TRUE: return TRUE;
case lexer::tag::FALSE: return FALSE;
case '(': return LB;
case ')': return RB;
case lexer::tag::INC: return INC;
case lexer::tag::DEC: return DEC;
case '+': return PLUS;
case '-': return MIN;
case '/': return DIV;
case '*': return MULT;
case '=': return ASS;
case lexer::tag::OR: return OR;
case lexer::tag::AND: return AND;
case '>': return GT;
case lexer::tag::GE: return GE;
case lexer::tag::NE: return NE;
case lexer::tag::EQ: return EQ;
case '<': return LS;
case lexer::tag::LE: return LE;
case '!': return NOT;
default: return 0;
}
#endif//symbols map
}//getc()
int do_parse(){
std::stack<int> status;
std::stack<int> products;
int sym,top;
status.push(0);//push the start status
products.push(1);//push the end symbols
top = status.top();
sym = getc();
while(true){
switch(cctab[top][sym][0]){
case SHIFT:{
products.push(sym);
status.push(cctab[top][sym][1]);
sym = getc();
top = status.top();
break;
}
case REDUCE:{
int l;
switch(cctab[top][sym][1]){
case 0:{//accept
break;
}
case 1:{//program=>block 
products.pop();status.pop();
l = 2 ;
break;
}
case 2:{//block=>LCB decls stmts RCB 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 3 ;
break;
}
case 3:{//decls=>decls decl 
products.pop();status.pop();
products.pop();status.pop();
l = 4 ;
break;
}
case 4:{//decls=>% 
products.pop();status.pop();
l = 4 ;
break;
}
case 5:{//decl=>BASIC idlist SEM 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 5 ;
break;
}
case 6:{//init_list=>init_list COMMA expr 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 13 ;
break;
}
case 7:{//init_list=>expr 
products.pop();status.pop();
l = 13 ;
break;
}
case 8:{//idlist=>idlist COMMA declarator 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 7 ;
break;
}
case 9:{//idlist=>declarator 
products.pop();status.pop();
l = 7 ;
break;
}
case 10:{//declarator=>ID 
products.pop();status.pop();
l = 8 ;
break;
}
case 11:{//declarator=>ID ASS expr 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 8 ;
break;
}
case 12:{//declarator=>array_decl 
products.pop();status.pop();
l = 8 ;
break;
}
case 13:{//array_decl=>ID LSB NUM RSB 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 9 ;
break;
}
case 14:{//array_decl=>ID LSB NUM RSB ASS LCB init_list RCB 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 9 ;
break;
}
case 15:{//stmts=>stmts stmt 
products.pop();status.pop();
products.pop();status.pop();
l = 14 ;
break;
}
case 16:{//stmts=>% 
products.pop();status.pop();
l = 14 ;
break;
}
case 17:{//stmt=>selection 
products.pop();status.pop();
l = 15 ;
break;
}
case 18:{//stmt=>iteration 
products.pop();status.pop();
l = 15 ;
break;
}
case 19:{//stmt=>BREAK SEM 
products.pop();status.pop();
products.pop();status.pop();
l = 15 ;
break;
}
case 20:{//stmt=>block 
products.pop();status.pop();
l = 15 ;
break;
}
case 21:{//stmt=>expr SEM 
products.pop();status.pop();
products.pop();status.pop();
l = 15 ;
break;
}
case 22:{//iteration=>WHILE LB expr RB stmt 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 12 ;
break;
}
case 23:{//iteration=>DO stmt WHILE LB expr RB SEM 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 12 ;
break;
}
case 24:{//iteration=>FOR LB expr SEM expr SEM expr RB stmt 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 12 ;
break;
}
case 25:{//selection=>IF LB expr RB stmt 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 11 ;
break;
}
case 26:{//selection=>IF LB expr RB stmt ELSE stmt 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 11 ;
break;
}
case 27:{//factor=>ID 
products.pop();status.pop();
l = 25 ;
break;
}
case 28:{//factor=>NUM 
products.pop();status.pop();
l = 25 ;
break;
}
case 29:{//factor=>LB expr RB 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 25 ;
break;
}
case 30:{//postfix=>factor 
products.pop();status.pop();
l = 24 ;
break;
}
case 31:{//postfix=>factor LSB expr RSB 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 24 ;
break;
}
case 32:{//postfix=>factor INC 
products.pop();status.pop();
products.pop();status.pop();
l = 24 ;
break;
}
case 33:{//postfix=>factor DEC 
products.pop();status.pop();
products.pop();status.pop();
l = 24 ;
break;
}
case 34:{//unary=>postfix 
products.pop();status.pop();
l = 23 ;
break;
}
case 35:{//unary=>NOT unary 
products.pop();status.pop();
products.pop();status.pop();
l = 23 ;
break;
}
case 36:{//unary=>MIN unary 
products.pop();status.pop();
products.pop();status.pop();
l = 23 ;
break;
}
case 37:{//unary=>INC unary 
products.pop();status.pop();
products.pop();status.pop();
l = 23 ;
break;
}
case 38:{//unary=>DEC unary 
products.pop();status.pop();
products.pop();status.pop();
l = 23 ;
break;
}
case 39:{//term_=>term_ MULT unary 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 22 ;
break;
}
case 40:{//term_=>term_ DIV unary 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 22 ;
break;
}
case 41:{//term_=>unary 
products.pop();status.pop();
l = 22 ;
break;
}
case 42:{//add=>add PLUS term_ 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 21 ;
break;
}
case 43:{//add=>add MIN term_ 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 21 ;
break;
}
case 44:{//add=>term_ 
products.pop();status.pop();
l = 21 ;
break;
}
case 45:{//rel=>rel GT add 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 20 ;
break;
}
case 46:{//rel=>rel LS add 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 20 ;
break;
}
case 47:{//rel=>rel GT add 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 20 ;
break;
}
case 48:{//rel=>rel LE add 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 20 ;
break;
}
case 49:{//rel=>add 
products.pop();status.pop();
l = 20 ;
break;
}
case 50:{//equality=>equality EQ rel 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 19 ;
break;
}
case 51:{//equality=>equality NE rel 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 19 ;
break;
}
case 52:{//equality=>rel 
products.pop();status.pop();
l = 19 ;
break;
}
case 53:{//join=>join AND equality 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 18 ;
break;
}
case 54:{//join=>equality 
products.pop();status.pop();
l = 18 ;
break;
}
case 55:{//bool_=>bool_ OR join 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 17 ;
break;
}
case 56:{//bool_=>join 
products.pop();status.pop();
l = 17 ;
break;
}
case 57:{//expr=>unary ASS expr 
products.pop();status.pop();
products.pop();status.pop();
products.pop();status.pop();
l = 16 ;
break;
}
case 58:{//expr=>bool_ 
products.pop();status.pop();
l = 16 ;
break;
}
}
top = status.top();
status.push(cctab[top][l][1]);
products.push(l);
top = status.top();
}
default:{
error();
}
}
}

}//do_parse

void error(){
}
};//class parser
#endif//_PARSER_H
