#include"node.h"
#include<iostream>

namespace inter{
    int node::newlabel()const
    {
        return ++labels;
    }

    void node::emitlabel(int i)const
    {
        std::cout<<"_LCC_"<<i<<":"<<std::endl;
    }

    void node::emit(const string&s)const
    {
        std::cout<<s<<"\n";
    }
    
    void node::emitjmp(const int l,const int t)const
    {
        switch(t){
        case JMP:
            std::cout<<"jmp _LCC_"<<l<<"\n";
            break;
        case JZ:
            std::cout<<"jz _LCC_"<<l<<"\n";
            break;
        case JNZ:
            std::cout<<"jnz _LCC_"<<l<<"\n";
            break;
        case JE:
            std::cout<<"je _LCC_"<<l<<"\n";
            break;
        case JNE:
            std::cout<<"jne _LCC_"<<l<<"\n";
            break;
        case JL:
            std::cout<<"jl _LCC_"<<l<<"\n";
            break;
        case JNL:
            std::cout<<"jnl _LCC_"<<l<<"\n";
            break;
        case JLE:
            std::cout<<"jle _LCC_"<<l<<"\n";
            break;
        case JG:
            std::cout<<"jg _LCC_"<<l<<"\n";
            break;
        case JGE:
            std::cout<<"jge _LCC_"<<l<<"\n";
            break;
        case JNG:
            std::cout<<"jng _LCC_"<<l<<"\n";
            break;
        case JMPO:
            std::cout<<"jmp $"<<l<<"\n";
            break;
        case JZO:
            std::cout<<"jz $"<<l<<"\n";
            break;
        case JNZO:
            std::cout<<"jnz $"<<l<<"\n";
            break;
        case JEO:
            std::cout<<"je $"<<l<<"\n";
            break;
        case JNEO:
            std::cout<<"jne $"<<l<<"\n";
            break;
        case JLO:
            std::cout<<"jl $"<<l<<"\n";
            break;
        case JNLO:
            std::cout<<"jnl $"<<l<<"\n";
            break;
        case JLEO:
            std::cout<<"jle $"<<l<<"\n";
            break;
        case JGO:
            std::cout<<"jg $"<<l<<"\n";
            break;
        case JGEO:
            std::cout<<"jge $"<<l<<"\n";
            break;
        case JNGO:
            std::cout<<"jng $"<<l<<"\n";
            break;
        }
    }
   
    void node::emitpush(const string&str)const
    {
        std::cout<<"pushl "<<str<<"\n";
        esp_offset += 4;
    }
    
    void node::emitpop(const string&str)const
    {
        std::cout<<"popl "<<str<<"\n";
        esp_offset -= 4;
    }

    void node::emitstack(const int sz)const
    {
        std::cout<<"addl "<<sz<<",%esp\n";
        esp_offset += sz;
    }

    int node::esp_offset = 0;
    int node::labels = 0;
};//namespace inter
