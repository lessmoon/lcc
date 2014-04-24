#ifndef _TAG_H
#define _TAG_H
    namespace lexer{
        typedef  int  tag_t;
        namespace tag{
            const tag_t     AND   = 256,BASIC = 257,BREAK = 258,
                            DO    = 259,ELSE  = 260,EQ    = 261,
                            FALSE = 262,GE    = 263,ID    = 264,
                            IF    = 265,INDEX = 266,LE    = 267,
                            MINUS = 268,NE    = 269,NUM   = 270,
                            OR    = 271,REAL  = 272,TEMP  = 273,
                            TRUE  = 274,WHILE = 275,INC   = 276,
                            DEC   = 277,FOR   = 278,STR   = 279,
                            PRINTF_FUNC = 280 , MAIN_ID = 281,RETURN = 282;
        };
    };
#endif // _TAG_H
