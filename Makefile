CPP = g++
OPT_CPPFLAGS = -Wall -ansi -pedantic -DDEBUG_LEVEL=$(debuglevel) -DNDEBUG -O2
DEBUG_CPPFLAGS = -Wall -Wextra -ansi -pedantic -g -DDEBUG_LEVEL=$(debuglevel)
LDFLAGS = 

ifeq ($(debuglevel),)
	debuglevel = 1
endif

ifeq ($(debuglevel),0)
	CPPFLAGS = $(OPT_CPPFLAGS)
else
	CPPFLAGS = $(DEBUG_CPPFLAGS)
endif

all: network.o growingnet.o

%.o: %.cc %.h
	$(CPP) $(CPPFLAGS) -c $(LDFLAGS) $< -o $@

clean:
	rm -f *.o
