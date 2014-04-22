#ifndef _INTER_ARITH_H
#define _INTER_ARITH_H

#include"node.h"
#include"expr.h"
#include"id.h"
#include"token.h"
#include"type.h"

namespace inter{
    struct OP:public expr{
    OP()
    :expr(NULL,symbols::int_){}
    };//struct op

    struct arith:public OP{
        expr* e1;
        expr* e2;

        void gen()const
        {
            e1 -> gen();
            this -> emitpush("%eax");
            e2 -> gen();
            switch(expr::op -> tag){
            case '+':{
                this -> emit("addl (%esp),%eax\n");
                this -> emitstack(4);
                break;
            }
            case '-':{
                this -> emitpop("%ebx");
                this -> emit("subl %ebx,%eax\n");
                break;
            }
            case '*':{
                this -> emit("mull (%esp)\n");
                this -> emitstack(4);
                break;
            }
            case '/':{
                this -> emit("movl $0,%edx\n");
                this -> emit("movl $eax,$ebx\n");
                this -> emit("movl (%esp),$eax\n");
                this -> emit("divl %ebx\n");
                this -> emitstack(4);
                break;
            }
            case '%':{
                this -> emit("movl $0,%edx\n");
                this -> emit("movl $eax,$ebx\n");
                this -> emit("movl (%esp),$eax\n");
                this -> emit("divl %ebx\n");
                this -> emit("movl %edx,%eax\n");
                this -> emitstack(4);
                break;
            }
            case lexer::tag::EQ:{
                this -> emit("cmpl %eax,(%esp)\n");
                this -> emit("sete %al\n");
                this -> emitstack(4);
                break;
            }
            case lexer::tag::NE:{
                this -> emit("cmpl %eax,(%esp)\n");
                this -> emit("setne %al\n");
                this -> emit("movzbl %al,%eax\n");
                this -> emitstack(4);
                break;
            }
            case '>':{
                this -> emit("cmpl %eax,(%esp)\n");
                this -> emit("setg %al\n");
                this -> emit("movzbl %al,%eax\n");
                this -> emitstack(4);
                break;
            }
            case '<':{
                this -> emit("cmpl %eax,(%esp)\n");
                this -> emit("setl %al\n");
                this -> emit("movzbl %al,%eax\n");
                this -> emitstack(4);
                break;
            }
            case lexer::tag::LE:{
                this -> emit("cmpl %eax,(%esp)\n");
                this -> emit("setle %al\n");
                this -> emit("movzbl %al,%eax\n");
                this -> emitstack(4);
                break;
            }
            case lexer::tag::GE:
                this -> emit("cmpl %eax,(%esp)\n");
                this -> emit("setge %al\n");
                this -> emit("movzbl %al,%eax\n");
                this -> emitstack(4);
                break;
            }
        }
    };//struct arith

    struct unary:public OP{
        expr* e;

        void gen()const
        {
            e -> gen();
            switch(expr::op -> tag){
            case '-':{
                this -> emit("negl %eax\n");
                break;
            }
            case '+':{
                break;
            }
            case lexer::tag::INC:{
                this -> emit("movl (%ebx),%eax\n");
                this -> emit("incl %eax\n");
                this -> emit("movl %eax,(%ebx)\n");
                break;
            }
            case lexer::tag::DEC:{
                this -> emit("movl (%ebx),%eax\n");
                this -> emit("decl %eax\n");
                this -> emit("movl %eax,(%ebx)\n");
                break;
            }
            }
        }

    };//struct unary

    struct constant:public expr{
        constant()
        :expr(NULL,symbols::int_){}
        void gen()const
        {
           int value = static_cast<lexer::num*>(expr::op) -> value;
           std::cout<<"movl $"<<value<<",%eax\n";
        }

    };//struct constant

    struct logical:public OP{
        expr* e1;
        expr* e2;
    };//struct logical

    struct and_:public logical{
        void gen()const
        {
            int l1 = this -> newlabel();
            int l2 = this -> newlabel();
            e1 -> gen();
            this -> emitlabel(l1);
            this -> emit("cmpl %eax,$0\n");
            this -> emitjmp(l2,JZ);
            e2 -> gen();
            this -> emitlabel(l2);
        }
    };//struct and_

    struct or_:public logical{

        void gen()const
        {
            int l1 = this -> newlabel();
            int l2 = this -> newlabel();
            e1 -> gen();
            this -> emitlabel(l1);
            this -> emit("cmpl %eax,$0\n");
            this -> emitjmp(l2,JNZ);
            e2 -> gen();
            this -> emitlabel(l2);
        }
    };//struct or_

    struct not_:public logical{
        void gen()const
        {
            this -> emit("notl %eax\n");
        }
    };//struct not_

    struct postfix:public expr{
        expr* e;
        postfix()
        :expr(NULL,symbols::int_){}
        void gen()const
        {
            e -> gen();
            switch(expr::op -> tag){
            case lexer::tag::INC:{
                this -> emit("movl %eax,%ecx\n");
                this -> emit("incl %ecx\n");
                this -> emit("movl %ecx,(%ebx)\n");
                break;
            }
            case lexer::tag::DEC:{
                this -> emit("movl %eax,%ecx\n");
                this -> emit("incl %ecx\n");
                this -> emit("movl %ecx,(%ebx)\n");
                break;
            }
            }
        }
    };//postfix
};

#endif //_INTER_ARITH_H
