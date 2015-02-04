CC = gcc            # default is CC = cc
CFLAGS = -g -Wall   # default is CFLAGS = [blank]
CPPFLAGS =          # default is CPPFLAGS = [blank]


# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

all: gtthread

gtthread: structures.o gtthread.o
	$(CC) -o $@ $< $(CFLAGS) 
# default linking command: $(CC) $(LDFLAGS) <foo>.o -o <foo>

structures.o: structures.c structures.h
	$(CC) -o $@ $< $(CFLAGS) 

gtthread.o: gtthread.c
	$(CC) -o $@ $< $(CFLAGS) 

clean:
	$(RM) -f *.o gtthread
