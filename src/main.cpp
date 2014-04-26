#include <iostream>
#include "parser.h"
#include "kb_io.h"
#include "lexer.h"
#include "tabgen.h"
#include "codegen.h"

using namespace ccparser;
using namespace system;
using namespace cctabgen;
//using namespace cccodegen;

using namespace std;

int main()
{
    tabgen tg;
    tabgen::table*tab;
    iol* io = new kb_io;
    cccodegen::codegen*cg = new cccodegen::codegen;
    lexer::lexer* z = new lexer::lexer(io);
    parser p(z);
    ccparser::result*res;
    
    do{
        try{
            res = p.stmts();
        }catch(std::string&e){
            std::cerr<<"At line "<<z -> lineno<<":"<<e<<std::endl;
        }
        try{
            tg.set_para(res);
            tab = tg.calculate();
        }catch(std::string&e){
            std::cerr<<e<<std::endl;
            break;
        }
        try{
            cg -> set_para(res,tab);
            cg -> gen_code();
        }catch(...){
            
        }
    }while(false);
    delete cg;
    delete z;
    delete io;
    return 0;
}
