CC = cc
CFLAGS = -Wall -std=c99 -pedantic
CXX = g++
CXXFLAGS = -Wall -ansi -pedantic
CPPFLAGS =
LDFLAGS =

debuglevel := 1

ifeq ($(debuglevel),0)
	CXXFLAGS += -O2 -DNDEBUG
else
	CXXFLAGS += -g -Wextra
	CFLAGS += -g -Wextra
endif

.PHONY: all clean

all: network.o growingnet.o

%.o: %.cc %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DDEBUG_LEVEL=$(debuglevel) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o

%.test: %.c network.o growingnet.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DDEBUG_LEVEL=$(debuglevel) $(LDFLAGS) $< network.o growingnet.o -o $@
