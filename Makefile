#### GTThread Library Makefile

CFLAGS  = -Wall -pedantic
LFLAGS  =
CC      = gcc
RM      = /bin/rm -rf
AR      = ar rc
RANLIB  = ranlib

LIBRARY = gtthread.a

LIB_SRC = structures.c gtthread.c gtthread_sched.c gtthread_mutex.c

LIB_OBJ = $(patsubst %.c,%.o,$(LIB_SRC))

# pattern rule for object files
dining_philosophers: dining_philosophers.c gtthread.a
	$(CC) $(CFLAGS) -I. -o dining_philosophers dining_philosophers.c $(LIBRARY)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: $(LIBRARY)

$(LIBRARY): $(LIB_OBJ)
	$(AR) $(LIBRARY) $(LIB_OBJ)
	$(RANLIB) $(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(LIB_OBJ) dining_philosophers

.PHONY: depend
depend:
	$(CFLAGS) -- $(LIB_SRC)  2>/dev/null
