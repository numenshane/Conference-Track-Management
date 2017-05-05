CC = g++ -std=c++11 -g -I. -I..
LDFLAGS = -lm -lcppunit

ifdef DEBUG
CPPFLAGS = -Wall -Wextra -ggdb -pg -DDEBUG -O0
else
CPPFLAGS = -Wall -O2
endif

OBJS = ../Talk.o ../Parser.o ../Extractor.o ParserTest.o ExtractorTest.o main.o

all : ut

ut : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	rm -fr $(OBJS)

debug : $(OBJS)
	make all DEBUG=1
clean :
	rm -fr $(OBJS) ut 

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
