#include"node.h"

namespace inter{
    int node::newlabel()
    {
        return ++labels;
    }

    void node::emitlabel(int i)
    {

    }

    void node::emit(const string&s)
    {

    }

    int node::labels = 0;
};//namespace inter
