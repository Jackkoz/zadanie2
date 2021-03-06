CPP = g++
OPT_CPPFLAGS = -Wall -ansi -pedantic -c -DDEBUG_LEVEL=$(debuglevel) -O2
DEBUG_CPPFLAGS = -Wall -Wextra -ansi -pedantic -c -g -DDEBUG_LEVEL=$(debuglevel)
LDFLAGS = 

ifeq ($(debuglevel),)
	debuglevel = 0
endif

ifeq ($(debuglevel),0)
	CPPFLAGS = $(OPT_CPPFLAGS)
else
	CPPFLAGS = $(DEBUG_CPPFLAGS)
endif

all: network.o growingnet.o

%.o: %.cc %.h
	$(CPP) $(CPPFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f *.o
