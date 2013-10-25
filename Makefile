CPP = g++
OPT_CPPFLAGS = -Wall -ansi -pedantic -c -DDEBUG_LEVEL=$(debuglevel) -DNDEBUG -O2
DEBUG_CPPFLAGS = -Wall -Wextra -ansi -pedantic -c -g -DDEBUG_LEVEL=$(debuglevel)
LDFLAGS = 

ifeq ($(debuglevel),)
	debuglevel = 1
endif

ifeq ($(debuglevel),0)
	CPPFLAGS = $(OPT_CPPFLAGS)
else
	CPPFLAGS = $(DEBUG_CPPFLAGS)
endif

all: clean network.o growingnet.o

%.o: %.cc %.h
	$(CPP) $(CPPFLAGS) -c $(LDFLAGS) $< 

clean:
	rm -f *.o

net_test: clean net_test.c network.o growingnet.o
	gcc -c net_test.c
	$(CPP) $(CPPFLAGS) $(LDFLAGS) net_test.o network.o growingnet.o -o $@
