#ifndef _FILE_IO_H
#define _FILE_IO_H

#include"iol.h"

#include<fstream>

namespace system{
    class file_io:public iol{
    public:
        typedef file_io self;
        typedef self*   self_ptr;
        typedef std::ifstream file_t;
    public:
        ~file_io();
        file_io();
        file_io(const string&filename);
    public:
        bool open(const string&filename);
        bool is_open()const;
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
        file_t file;
    };
};//namespace system

#endif//_FILE_IO_H
