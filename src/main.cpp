#include <iostream>
#include "parser.h"
#include "kb_io.h"
#include "lexer.h"

using namespace ccparser;
using namespace system;

using namespace std;

int main()
{
    iol* io = new kb_io;
    lexer::lexer*   z = new lexer::lexer(io);
    parser p(z);
    try{
    p.stmts();
    }catch(std::string&e){
        std::cerr<<"At line "<<z -> lineno<<":"<<e<<std::endl;
    }
    delete z;
    delete io;
    return 0;
}
