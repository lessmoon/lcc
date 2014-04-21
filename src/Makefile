OBJS = lexer.o reserved.o parser.test.o kb_io.o file_io.o token_storage.o\
        parser.o  

CPPC = g++ -c

LD   = g++ -O3

CPPFLAGS = -O3

TAR = test

all:$(OBJS)
	$(LD) $(OBJS) -o $(TAR)

parser.test.o:parser.test.cpp parser.h
	$(CPPC) $(CPPFLAGS) parser.test.cpp

parser.o:parser.cpp parser.h
	$(CPPC) $(CPPFLAGS) parser.cpp

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
	-rm $(OBJS) $(TAR)
