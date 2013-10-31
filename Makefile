CXX = g++
CXXFLAGS = -Wall -ansi -pedantic
CPPFLAGS =
LDFLAGS =

debuglevel := 0

ifeq ($(debuglevel),0)
	CXXFLAGS += -O2
else
	CXXFLAGS += -g -Wextra
endif

.PHONY: all clean

all: network.o growingnet.o

%.o: %.cc %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DDEBUG_LEVEL=$(debuglevel) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o
