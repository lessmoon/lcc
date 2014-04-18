#include <iostream>
#include "parser.h"
#include "kb_io.h"
#include "lexer.h"
#include "tabgen.h"

using namespace ccparser;
using namespace system;
using namespace cctabgen;

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
    lexer::lexer* z = new lexer::lexer(io);
    parser p(z);
    try{
        ccparser::result*res = p.stmts();
        ccparser::result::prods*zp = &(res->productions);
        ccparser::result::prods::map::iterator iter;
        for(iter = zp -> seq.begin();iter != zp -> seq.end(); iter++){
            cout<<iter -> first<<"=>";
            print_rl(iter -> second);
        }
        tg.set_para(res);
    }catch(std::string&e){
        std::cerr<<"At line "<<z -> lineno<<":"<<e<<std::endl;
    }
    try{
        tab = tg.calculate();
        for(int i = 0 ; i < tab -> size();i++){
            for(int j = 0 ; j < tab -> at(i).size();j++)
                std::cout<< (*tab)[i][j]<<"\t";
            std::cout<<std::endl;
        }
    }catch(std::string&e){
        std::cerr<<e<<std::endl;
    }
    delete z;
    delete io;
    return 0;
}
