CFLAGS = -Wall -g -std=c99
DBGFLAGS = -DDEBUG

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

genops: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

debug: $(SRC) 
	gcc -o genops_debug $^ $(CFLAGS) $(LIBS) $(DBGFLAGS)


clean:
	rm genops genops_debug
