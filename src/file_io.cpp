#include"file_io.h"

namespace system{
    file_io::~file_io()
    {
        this -> close();
    }

    file_io::file_io()
    {

    }

    system::string file_io::readline()
    {
        string str;
        std::getline(file,str,'\n');
        return str;
    }

    file_io::file_io(const string&filename)
    {
        this -> open(filename);
    }

    bool file_io::open(const string&filename)
    {
        if(this -> is_open())
            this -> close();
        file.open(filename.data());
        return this -> is_open();
    }

    bool file_io::is_open()const
    {
        return file.is_open();
    }

    elem_t file_io::readch()
    {
        return file.get();
    }

    void file_io::close()
    {
        file.close();
    }

    bool file_io::is_end()const
    {
        return file.eof();
    }

    void file_io::unread(const elem_t e)
    {
        //file.seekg();
    }

    void file_io::put_error(const string&err)
    {
        //file.write();
    }

    void file_io::put_str(const string&str)
    {
        //file.write();
    }
};//namespace system
