CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

parser: $(OBJS)
		$(CC) -o parser $(OBJS) $(LDFLAGS)

$(OBJS): clesson.h

clean: 
	rm -f *.o parser