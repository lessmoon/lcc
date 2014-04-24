OBJS = lexer.o reserved.o main.o kb_io.o file_io.o token_storage.o\
        parser.o  env.o node.o

CPPC = g++ -c

LD   = g++  

CPPFLAGS =  

TAR = lcc

all:$(OBJS)
	$(LD) $(OBJS) -o $(TAR)

main.o:main.cpp parser.h kb_io.h
	$(CPPC) $(CPPFLAGS) main.cpp

parser.o:parser.cpp parser.h
	$(CPPC) $(CPPFLAGS) parser.cpp

env.o: env.cpp env.h
	$(CPPC) $(CPPFLAGS) env.cpp

node.o:node.cpp node.h
	$(CPPC) $(CPPFLAGS) node.cpp

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

token_storage.o:token_storage.cpp token_storage.h token.h
	$(CPPC) $(CPPFLAGS) token_storage.cpp

clean:
	-rm $(OBJS) $(TAR)
