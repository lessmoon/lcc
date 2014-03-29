#include"lexer.h"
#include"kb_io.h"
#include<iostream>

using namespace std;
using namespace lexer;
using namespace symbols;
using namespace system;

int main()
{
    iol* con_io = new kb_io;
    lexer::lexer lex(con_io);
    tag_t tag;
    token*p;

    while(p = lex.scan()){
            if(p->tag == -1)
                break;
            cout<<p->tag<<":~:"<<p->toString()<<endl;
    }
    
    delete con_io;
    return 0;
}
