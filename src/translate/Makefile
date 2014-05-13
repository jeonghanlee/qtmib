all: ../../build/qtmib-translate
H_FILE_LIST       = $(wildcard *.[h])
CPP_FILE_LIST       = $(wildcard *.cpp)
OBJS = $(CPP_FILE_LIST:.cpp=.o)
BINOBJS =  $(foreach file, $(OBJS), $file)
CXXFLAGS += -O2 -fstack-protector-all -D_FORTIFY_SOURCE=2 -fPIE -pie -Wformat -Wformat-security
LDFLAGS += -Wl,-z,relro -Wl,-z,now

%.o : %.cpp $(H_FILE_LIST)
	g++ $(CXXFLAGS) $(INCLUDE) -c $< -o $@

../../build/qtmib-translate: $(OBJS)
	g++ $(LDFLAGS) $(LIBS) -o $@ $(OBJS)

clean:; rm -f *.o ../../build/qtmib-translate
