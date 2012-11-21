CC=g++
CFLAGS=-c -ggdb -std=c++0x
LDFLAGS=-lfftw3 -lm
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=NN

all: $(OBJECTS) $(EXECUTABLE)

rebuild: clean all

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

$(OBJECTS): 
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
