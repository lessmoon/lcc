#include"lexer.h"
#include<iostream>

using namespace std;
using namespace lexer;
using namespace symbols;

int main()
{
    lexer::lexer lex;
    tag_t tag;
    token*p;

    while(p = lex.scan()){
        cout<<p->tag<<endl;
    }
}
