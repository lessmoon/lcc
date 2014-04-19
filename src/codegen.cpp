#include"codegen.h"

namespace cccodegen{
    codegen::codegen()
    {}

    codegen::~codegen()
    {}

    void codegen::set_para(info*para,table*tab)
    {
        this -> para = para;
        this -> tab = tab;
        /*dirty work*/
        this -> def_tab = &(para -> def_table);
        this -> svt     = &(para -> type_table);
        this -> str_sym = &(para -> start_symbols);
        this -> ps      = &(para -> productions);
    }

    void codegen::gen_code()
    {
#define cout std::cout
        this -> gen_include("stack");
        this -> gen_macro("SHIFT",cctabgen::SHIFT);
        this -> gen_macro("ERROR",cctabgen::ERROR);
        this -> gen_macro("REDUCE",cctabgen::REDUCE);
        this -> gen_sym_def();
        cout<<"class parser{\n";
            cout<<"static const int cctab["
                <<tab -> size()<<"]["<<svt -> size()<<"]"<<"[2];\n" 
                <<"int getc(){\n";
            this -> gen_sym_map();
            cout<<"}//getc()\n";
            cout<<"int do_parse(){\n"
                <<"std::stack<int> status;\n"
                <<"std::stack<int> products;\n"
                <<"int sym,top;\n"
                <<"status.push(0);//push the start status\n"
                <<"products.push(1);//push the end symbols\n"
                <<"top = status.top();\n"
                <<"sym = getc();\n";
            cout<<"while(true){\n"
                <<"switch(cctab[top][sym][0]){\n"
                <<"case SHIFT:{\n";
                    this -> gen_shift();
            cout<<"}\n"
                <<"case REDUCE:{\n";
                    this -> gen_reduce();
            cout<<"}\n"
                <<"default:{\n"
                <<"error();\n"
                <<"}\n"
                <<"}\n"
                <<"}\n"
                <<"}//do_parse\n";
            cout<<"};//class parser\n";
            this -> gen_table();
#undef cout
    }
    
    void codegen::gen_shift()
    {
#define cout    std::cout
        cout<<"products.push(sym);\n"
            <<"status.push(cctab[top][sym][1]);\n"
            <<"sym = getc();\n"
            <<"top = status.top();\n"
            <<"break;\n";
#undef cout
    }

    void codegen::gen_sym_def()
    {
        std::cout<<"#if 1//symbols define\n";
        for(sym_t i = 2 ; i < svt -> size();i++){
           this -> gen_macro(para -> get_name(i),i);
        }
        std::cout<<"#endif//symbols define\n";
    }

    void codegen::gen_sym_map()
    {
#define cout std::cout
        cout<<"#if 1//symbols map\n"
            <<"tag_t t = gettok();\n"
            <<"switch(t){\n"
            <<"case xxx: return 1;//end symbol\n";
        for(sym_t i =  2; i < svt -> size() ; i++){
            if(svt -> at(i) == ccparser::TERM){
                cout<<"case xxx" <<": return "<<para -> get_name(i)<<";\n";
            }
        }
        cout<<"default: return 0;\n"
            <<"}\n"
            <<"#endif//symbols map\n";
#undef cout
    }

    void codegen::gen_reduce()
    {
#define print(x) cout<<(x);
#define cout     std::cout
        cout<<"int l;\n"
            <<"switch(cctab[top][sym][1]){\n" 
            <<"case 0:{//accept\n"
            <<"break;\n"
            <<"}\n";
        for(int i = 0; i < ps -> size();i++){
            const prod& p = para -> get_prod(i);
            cout<<"case "<<i+1<<":{//";gen_prod(p);cout<<"\n";
            for(int j = 0;j < p.r -> size();j++)
                print("products.pop();status.pop();\n");
            cout<<"l = " << p.l << " ;\n"
                <<"break;\n";
            print("}\n");
        }
        cout<<"}\n"
            <<"top = status.top();\n"
            <<"status.push(cctab[top][l][1]);\n"
            <<"products.push(l);\n"
            <<"top = status.top();\n";
        
#undef  cout
#undef print
    }

    void codegen::gen_table()
    {
        std::cout   <<"#if 1//action table\n"
                    <<"const int parser::cctab["<< tab -> size() <<"]["<< svt -> size() <<"][2]={\n";
        for(int i = 0 ; i < tab -> size();i++){
            std::cout<<"{";
            for(int j = 0;j < svt -> size();j++){
                std::cout<<"{"<<(*tab)[i][j].t<<","<<(*tab)[i][j].where<<"}";
                if(j != tab -> at(i).size() - 1)
                    std::cout<<",";
            }
            std::cout<<"}";
            if(i != tab -> size() - 1)
                std::cout<<",";
            std::cout<<"\n";
        }
        std::cout<<"};\n"
                 <<"#endif //action table\n";
    }
    
    void codegen::gen_prod(const prod&p)
    {
#define nm(x) (para -> get_name(x))
        std::cout<<nm(p.l)<<"=>";
        right* r = p.r;
        for(int i = 0;i < r -> size();i++){
            std::cout << nm(r -> at(i))<<" ";
        }
#undef  nm
    }

    void codegen::gen_macro(const string&name,int value)
    {
        std::cout<<"#define " << name << " " << value << "\n";
    }

    void codegen::gen_include(const string&filename)
    {
        std::cout<<"#include<"<<filename<<">\n";
    }

    void codegen::gen_space(const int num)
    {
        for(int i = 0;i < num;i++)
            std::cout<<' ';
    }
};//namespace cccodegen
