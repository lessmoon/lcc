#ifndef _NODE_H
#define _NODE_H

#include"lexer.h"

#include<string>

namespace inter{

    const int   JMP = 0,JZ = 1,JNZ = 2,JE = 3,JNE = 4,
                JL  = 5,JNL = 6,JLE = 7,JG = 8,JGE = 9,
                JNG = 10;
    const int   JMPO = 11,JZO = 12,JNZO = 13,JEO = 14,JNEO = 15,
                JLO  = 16,JNLO = 17,JLEO = 18,JGO = 19,JGEO = 20,
                JNGO = 21;

    class node {
    public:
        typedef std::string string;
    private:
        int lexline;
        static int labels;
    public:
        static int esp_offset;
    public:
        node()
        :lexline(lexer::lexer::lineno)
        {

        }
    public:
        virtual void gen() const = 0;
        virtual int newlabel()const;
        virtual void emitlabel(int i)const;
        virtual void emitpush(const string&d)const;
        virtual void emitpop(const string&d)const;
        virtual void emitstack(const int size)const;
        virtual void emit(const string&s)const;
        virtual void emitjmp(const int l,const int type)const;
    };//class node
};//namespace symbols

#endif//_NODE_H
