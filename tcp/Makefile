CC=gcc
CCFLAG=-c -Wall
LDFLAG=
HEADER=network.h
SOURCE=server.c client.c
OBJECT=$(SOURCE:.c=.o)
BINARY=$(OBJECT:.o=)

all: $(BINARY)

clean:
	rm ./*.o $(BINARY)

$(BINARY):%: %.o $(HEADER)
	$(CC) $(LDFLAG) -o $@ $<

.c.o:
	$(CC) $(CCFLAG) -o $@ $<
