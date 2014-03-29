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
        int newlable()
        {
            return ++labels;
        }

        void emitlabel(int i)
        {
            //TODO generate i
        }

        void emit(const string&s)
        {
            //TODO generate this code
        }
    };//class node
    int node::labels = 0;//TODO Move to .cpp
};//namespace symbols

#endif//_NODE_H
