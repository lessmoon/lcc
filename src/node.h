#ifndef _NODE_H
#define _NODE_H

#include"lexer.h"

#include<string>

namespace inter{
    class node {
    public:
        typedef std::string string;
    private:
        int lexline;
        static int labels;
    public:
        node()
        :lexline(lexer::lexer::lineno)
        {
            
        }
    public:
        virtual int newlabel();
        virtual void emitlabel(int i);
        virtual void emit(const string&s);
    };//class node
};//namespace symbols

#endif//_NODE_H
