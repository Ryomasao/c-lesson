CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

parser: $(OBJS)
		$(CC) -o eval $(OBJS) $(LDFLAGS)

$(OBJS): stack.h

clean: 
	rm -f *.o eval