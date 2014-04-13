OBJS1 = lexer.o reserved.o kb_io.o file_io.o token_storage.o
OBJS2 = $(OBJS1) cclr4.o
OBJS = $(OBJS1) lexertest.o

CPPC = g++ -c

LD   = g++ -g

CPPFLAGS =  -g

TAR = test 
TAR2 = test2

all:$(OBJS)
	$(LD) $(OBJS) -o $(TAR)
ptest:$(OBJS2)
	$(LD) $(OBJS2) -o $(TAR2)

cclr4.o:cclr4.cpp reserved.o kb_io.o file_io.o token_storage.o
	$(CPPC) $(CPPFLAGS) cclr4.cpp

lexer.o:lexer.cpp reserved.o token_storage.o lexer.h real.h num.h\
        tag.h type.h\
        word.h lexer.h tag.h type.h str.h token.h iol.h
	$(CPPC) $(CPPFLAGS) lexer.cpp

reserved.o:reserved.cpp type.h word.h
	$(CPPC) $(CPPFLAGS) reserved.cpp

kb_io.o:kb_io.cpp kb_io.h iol.h
	$(CPPC) $(CPPFLAGS) kb_io.cpp

file_io.o:file_io.cpp iol.h
	$(CPPC) $(CPPFLAGS) file_io.cpp

lexertest.o:lexertest.cpp lexer.o reserved.o kb_io.o
	$(CPPC) $(CPPFLAGS) lexertest.cpp

token_storage.o:token_storage.cpp token_storage.h token.h
	$(CPPC) $(CPPFLAGS) token_storage.cpp

clean:
	-rm $(OBJS) $(TAR) $(TAR2)
