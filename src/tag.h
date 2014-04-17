#ifndef _TAG_H
#define _TAG_H
    namespace lexer{
        typedef  int  tag_t;
        namespace tag{
            const tag_t     ID = 256,TERM = 257,VAR = 258;
        };
    };
#endif // _TAG_H
