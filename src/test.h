#ifndef zzzz
#define zzzz
#include<sstream>
#include<string>

template<typename T>
std::string toString(T&x)
{
    std::string s;
    std::stringstream ss;
    ss<<x;
    ss>>s;
    return s;
}

#endif
