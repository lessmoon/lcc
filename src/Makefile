OBJS = lexer.o reserved.o

CPPC = g++ -c

LD   = g++

CPPFLAGS = -O3

all:$(OBJS)
	$(LD) $(OBJS)

lexer.o:lexer.cpp reserved.o lexer.h real.h num.h tag.h type.h\
        word.h lexer.h tag.h type.h str.h token.h
	$(CPPC) $(CPPFLAGS) lexer.cpp
reserved.o:reserved.cpp type.h word.h
	$(CPPC) $(CPPFLAGS) reserved.cpp

clean:
	-rm $(OBJS)
