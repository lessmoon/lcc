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


ostream&operator<<(ostream&os,cctabgen::action_node&x)
{
    os<<(x.t == cctabgen::ERROR?'E'
        :(x.t == cctabgen::SHIFT)?'S'
        :(x.t == cctabgen::JUSTGO)?'\0'
        :'R')<<x.where;
}

void prt_right(cctype::right* r)
{
    for(int i = 0;i < r -> size();i++)
        cout<< r -> at(i)<<"\t";
}

void print_rl(cctype::rightlist*rl)
{
    for(int i = 0;i < rl -> size();i++){
        cout<<"\t\t";
        prt_right(rl -> at(i));
        cout<<"\n";
    }
}

int main()
{
    tabgen tg;
    tabgen::table*tab;
    iol* io = new kb_io;
    cccodegen::codegen*cg = new cccodegen::codegen;
    lexer::lexer* z = new lexer::lexer(io);
    parser p(z);
    ccparser::result::prods*zp;
    ccparser::result*res;
    try{
        res = p.stmts();
        zp = &(res->productions);
        tg.set_para(res);
    }catch(std::string&e){
        std::cerr<<"At line "<<z -> lineno<<":"<<e<<std::endl;
    }
    try{
        tab = tg.calculate();
    }catch(std::string&e){
        std::cerr<<e<<std::endl;
    }
    cg -> set_para(res,tab);
    cg -> gen_code();
    delete cg;
    delete z;
    delete io;
    return 0;
}
