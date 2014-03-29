OBJS = lexer.o reserved.o lexertest.o kb_io.o

CPPC = g++ -c

LD   = g++

CPPFLAGS = -O3

TAR = test

all:$(OBJS)
	$(LD) $(OBJS) -o $(TAR)

lexer.o:lexer.cpp reserved.o lexer.h real.h num.h tag.h type.h\
         word.h lexer.h tag.h type.h str.h token.h iol.h
	$(CPPC) $(CPPFLAGS) lexer.cpp

reserved.o:reserved.cpp type.h word.h
	$(CPPC) $(CPPFLAGS) reserved.cpp

kb_io.o:kb_io.cpp kb_io.h iol.h
	$(CPPC) $(CPPFLAGS) kb_io.cpp

lexertest.o:lexertest.cpp lexer.o reserved.o kb_io.o
	$(CPPC) $(CPPFLAGS) lexertest.cpp

clean:
	-rm $(OBJS) $(TAR)
