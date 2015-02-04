CC = gcc            # default is CC = cc
CFLAGS = -g -Wall   # default is CFLAGS = [blank]
CPPFLAGS =          # default is CPPFLAGS = [blank]


# default compile command: $(CC) $(CFLAGS) $(CPPFLAGS) -c -o <foo>.o <foo>.c

all: gtthread

gtthread: gtthread.h gtthread.c 
	$(CC) -o $@ $< $(CFLAGS) 
# default linking command: $(CC) $(LDFLAGS) <foo>.o -o <foo>

#gtthread.o: gtthread.h gtthread.c 
#	$(CC) -o $@ $< $(CFLAGS) 

structures.o: structures.h structures.c
	$(CC) -o $@ $< $(CFLAGS) 

clean:
	$(RM) -f *.o gtthread
