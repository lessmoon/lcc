OBJS = lexer.o reserved.o lexertest.o

CPPC = g++ -c

LD   = g++

CPPFLAGS = -O3

TAR = test

all:$(OBJS)
	$(LD) $(OBJS) -o $(TAR)

lexer.o:lexer.cpp reserved.o lexer.h real.h num.h tag.h type.h\
         word.h lexer.h tag.h type.h str.h token.h
	$(CPPC) $(CPPFLAGS) lexer.cpp
reserved.o:reserved.cpp type.h word.h
	$(CPPC) $(CPPFLAGS) reserved.cpp

lexertest.o:lexertest.cpp lexer.o reserved.o
	$(CPPC) $(CPPFLAGS) lexertest.cpp

clean:
	-rm $(OBJS) $(TAR)
