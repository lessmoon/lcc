OBJS = main.o parser.o cctype.o tabgen.o lexer.o\
        reserved.o kb_io.o file_io.o \
        token_storage.o 
		

CPPC = g++ -c

LD   = g++ -g

CPPFLAGS =  -g

TAR = mycc

all:$(OBJS)
	$(LD) $(OBJS) -o $(TAR)

main.o:main.cpp parser.h kb_io.h lexer.h tabgen.h
	$(CPPC) $(CPPFLAGS) main.cpp

parser.o:parser.cpp parser.h lexer.h cctype.h tag.h word.h
	$(CPPC) $(CPPFLAGS) parser.cpp

cctype.o:cctype.cpp cctype.h 
	$(CPPC) $(CPPFLAGS) cctype.cpp

tabgen.o:tabgen.cpp tabgen.h parser.h
	$(CPPC) $(CPPFLAGS) tabgen.cpp

lexer.o:lexer.cpp  lexer.h \
        tag.h type.h\
        word.h lexer.h tag.h type.h token.h iol.h
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
