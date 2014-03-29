#include"kb_io.h"
#include<iostream>

namespace system{
    kb_io::~kb_io()
    {
        //nothing
    }

    kb_io::kb_io()
    :c(' ')
    {
        //nothing
    }
    
    elem_t kb_io::readch()
    {
        c = std::cin.get();
        return c;   
    }
    
    string kb_io::readline()
    {
        std::getline(std::cin,line,'\n');
        return line;
    }
    
    void kb_io::close()
    {   
        ;//nothing
    }
    
    bool kb_io::is_end()const
    {
        return (c=='\0');
    }
    
    void kb_io::unread(const elem_t c)
    {
        ;//TODO ungetchar();
    }
    
    void kb_io::put_error(const string&err)
    {
        std::cerr<<err;//
    }
    
    void kb_io::put_str(const string&str)
    {
        std::cout<<str;
    }
};//namespace system
