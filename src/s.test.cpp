#include<stack>
#define _LSCC_SHIFT 1
#define _LSCC_ERROR 3
#define _LSCC_REDUCE 0
#if 1//symbols define
#define _LSCC_f 2
#define _LSCC_x 3
#endif//symbols define
class parser{
static const int cctab[3][4][2];
int getc(){
#if 1//symbols map
tag_t t = gettok();
switch(t){
case xxx: return 1;//end symbol
case xxx: return _LSCC_x;
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
case 1:{//f=>x 
products.pop();status.pop();
l = 2 ;
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
};//class parser
#if 1//action table
const int parser::cctab[3][4][2]={
{{3,0},{3,0},{2,1},{1,2}},
{{3,0},{0,0},{3,0},{3,0}},
{{3,0},{0,1},{3,0},{3,0}}
};
#endif //action table
