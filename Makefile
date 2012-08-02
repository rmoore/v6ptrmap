CC=gcc
CFLAGS=-O1 -ggdb3 -Wall -Wextra -Werror -D_FORTIFY_SOURCE=2
LDLIBS=-lpthread -lresolv
OBJ=v6ptrmap.o ts_queue.o list.o

all: v6ptrmap

v6ptrmap: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -f *.o v6ptrmap
