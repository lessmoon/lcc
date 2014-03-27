#include"type.h"
#include"word.h"

namespace lexer{
    word::self_ptr  and_  = new word("&&",tag::AND),
                    or_   = new word("||",tag::OR),
                    eq_   = new word("==",tag::EQ),
                    ne_   = new word("!=",tag::NE),
                    le_   = new word("<=",tag::LE),
                    ge_   = new word(">=",tag::GE),
                    inc_  = new word("++",tag::INC),
                    dec_  = new word("--",tag::DEC),
                    true_ = new word("true",tag::TRUE),
                    false_= new word("false",tag::FALSE),
                    temp  = new word("t",tag::TEMP);
};//namespace lexer

namespace symbols{
    type::self_ptr int_   = new type("int",lexer::tag::BASIC,4),
                   float_ = new type("float",lexer::tag::BASIC,8),
                   char_  = new type("char",lexer::tag::BASIC,4),
                   bool_  = new type("bool",lexer::tag::BASIC,4);

};//namespace symbols
