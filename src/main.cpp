#include"kb_io.h"
#include"parser.h"

#include<string>

int main()
{
    system::iol* io = new system::kb_io();
    lexer::lexer*lex = new lexer::lexer(io);
    parser p(lex);
    try{
        p.do_parse();
    }catch(const std::string&str){
        std::cerr<<str<<std::endl;
    }
    delete lex;
    delete io;
    return 0;
}
