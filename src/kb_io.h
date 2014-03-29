#ifndef _KEYBOARD_IO_H
#define _KEYBOARD_IO_H

#include"iol.h"

#include<iostream>

namespace system{
    class kb_io:public iol{
    public:
        typedef kb_io self;
        typedef self*   self_ptr;
        typedef std::string string;
    public:
        ~kb_io();
        kb_io();
    public:
        elem_t readch();
        string readline();
        void   close();
        bool   is_end()const;
        void   unread(const elem_t);
        void   put_error(const string&err);
        void   put_str(const string&str);
        /*and others*/
    private:
        elem_t c;
        string line;
    };//class kb_io
};//namespace system
#endif//_KEYBOARD_IO_H

