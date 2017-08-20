CFLAGS = -Wall -g -std=c99

# Should be equivalent to your list of C files, if you don't build selectively
SRC=$(wildcard *.c)

topview: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

