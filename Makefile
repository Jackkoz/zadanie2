CXX = g++
CXXFLAGS = -Wall -ansi -pedantic
LDFLAGS = 

debuglevel := 0

ifeq ($(debuglevel),0)
	CXXFLAGS += -O2 -DNDEBUG
else
	CXXFLAGS += -g -Wextra
endif

all: clean network.o growingnet.o

%.o: %.cc %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DDEBUG_LEVEL=$(debuglevel) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o
