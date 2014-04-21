#ifndef _IO_LEVEL_H
#define _IO_LEVEL_H

#include<string>

/*
 * This is an interface of the io level.
 */
namespace system{
    typedef int elem_t;
    typedef std::string string;
    class iol{
    public:
        typedef iol     self;
        typedef self*   self_ptr;
    public:
        virtual ~iol(){}
        virtual elem_t readch()     = 0;//read a char from the input
        virtual string readline()   = 0;//read a line from the input
        virtual void   close()      = 0;//close the read
        virtual bool   is_end()const= 0;//check if it is the end of the input
        virtual void   unread(const elem_t)=0;//put the elem_t back to the input
        virtual void   put_error(const string&err)=0;//output an error information
        virtual void   put_str(const string&str)=0;//output a string
        /*and others*/
    };//class iol
};
#endif//_IO_LEVEL_H
