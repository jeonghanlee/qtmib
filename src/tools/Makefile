all: qtmibdb qtmiblist qtmibadd
H_FILE_LIST       = $(wildcard *.[h])
CPP_FILE_LIST       = $(wildcard *.c)
BINOBJS =  $(foreach file, $(OBJS), $file)
CFLAGS += -ggdb
LIBS=`net-snmp-config --agent-libs`

%.o : %.c $(H_FILE_LIST)
	gcc $(CFLAGS) $(INCLUDE) -c $< -o $@

qtmibdb: qtmibdb.o
	g++  $(LDFLAGS) -o $@ qtmibdb.o $(LIBS)

qtmiblist: qtmiblist.o
	g++  $(LDFLAGS) -o $@ qtmiblist.o

qtmibadd: qtmibadd.o
	g++  $(LDFLAGS) -o $@ qtmibadd.o

clean:; rm -f *.o qtmibdb qtmiblist qtmibadd
