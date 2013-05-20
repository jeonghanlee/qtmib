all: qtmibdb
H_FILE_LIST       = $(wildcard *.[h])
CPP_FILE_LIST       = $(wildcard *.c)
OBJS = $(CPP_FILE_LIST:.cpp=.o)
BINOBJS =  $(foreach file, $(OBJS), $file)
CFLAGS += -ggdb
LIBS=`net-snmp-config --agent-libs`

%.o : %.c $(H_FILE_LIST)
	gcc $(CFLAGS) $(INCLUDE) -c $< -o $@

qtmibdb: $(OBJS)
	g++  $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:; rm -f *.o dhcng
