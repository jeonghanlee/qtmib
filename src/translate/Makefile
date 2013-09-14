all: ../../bin/qtmib-translate
H_FILE_LIST       = $(wildcard *.[h])
CPP_FILE_LIST       = $(wildcard *.cpp)
OBJS = $(CPP_FILE_LIST:.cpp=.o)
BINOBJS =  $(foreach file, $(OBJS), $file)
CPPFLAGS += -ggdb

%.o : %.cpp $(H_FILE_LIST)
	g++ $(CPPFLAGS) $(INCLUDE) -c $< -o $@

../../bin/parser: $(OBJS)
	g++  $(LDFLAGS) -o $@ $(OBJS)

clean:; rm -f *.o ../../bin/qtmib-translate
