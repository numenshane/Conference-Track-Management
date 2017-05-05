CC = g++ -std=c++11 -g -I.
LDFLAGS = -lm

ifdef DEBUG
CPPFLAGS = -Wall -Wextra -ggdb -pg -DDEBUG -O0
else
CPPFLAGS = -Wall -O2
endif

OBJS = Talk.o Parser.o Extractor.o main.o

all : test

test : $(OBJS)
	$(CC) -lm -o $@ $^
	rm -fr *.o

debug :
	make all DEBUG=1

clean :
	rm -fr $(OBJS) test 

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
