CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

parser: $(OBJS)
		$(CC) -o stack $(OBJS) $(LDFLAGS)

$(OBJS): stack.h

clean: 
	rm -f *.o stack