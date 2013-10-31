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

all: network.o growingnet.o network_test1.test network_test2.test network_test3.test network_test4.test net_test.test

%.o: %.cc %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DDEBUG_LEVEL=$(debuglevel) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o

%.test: %.c network.o growingnet.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DDEBUG_LEVEL=$(debuglevel) $(LDFLAGS) $< network.o growingnet.o -o $@
