CFLAGS = -Wall -g -std=c99
DBGFLAGS = -DDEBUG

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

nthbiggest: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

debug: $(SRC)
	gcc -o nthbiggest_debug $^ $(CFLAGS) $(LIBS) $(DBGFLAGS)


clean:
	rm nthbiggest_debug nthbiggest
