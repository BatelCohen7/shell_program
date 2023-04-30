CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
RM = rm -f
SHAREDLIB = -shared
DLIBFLAGS = -ldl

.PHONY: all clean
all: cmp copy encode decode stshell

# Part A #
cmp: cmp.o
	$(CC) $(CFLAGS) -o $@ $^

copy: copy.o
	$(CC) $(CFLAGS) -o $@ $^

# Part B #
encode: encode.o libcodecA.so libcodecB.so
	$(CC) $(CFLAGS) -o $@ encode.o $(DLIBFLAGS)

decode: decode.o libcodecA.so libcodecB.so
	$(CC) $(CFLAGS) -o $@ decode.o $(DLIBFLAGS)
libcodecA.so: codecA.o
	$(CC) $(CFLAGS) $(SHAREDLIB) -o $@ $^

libcodecB.so: codecB.o
	$(CC) $(CFLAGS) $(SHAREDLIB) -o $@ $^

codecA.o: codecA.c codec.h
	$(CC) $(CFLAGS) -fPIC -c $^

codecB.o: codecB.c codec.h
	$(CC) $(CFLAGS) -fPIC -c $^

# Part C #
stshell: stshell.o
	$(CC) $(CFLAGS) -o $@ $^

stshell.o: stshell.c stshell.h
	$(CC) $(CFLAGS) -c $^

%.o: %.c
	$(CC) $(CFLAGS) -c $^
	

clean:
	$(RM) *.gch *.o *.a *.so *.dll *.dylib cmp copy encode decode stshell