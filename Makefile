CC=gcc
CFLAGS=-ggdb3 -Wall -Wextra -Werror
LDLIBS=-lpthread -lresolv
OBJ=v6ptrmap.o ts_queue.o list.o

all: v6ptrmap

v6ptrmap: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -f *.o v6ptrmap
