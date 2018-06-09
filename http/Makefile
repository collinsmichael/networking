TARGET=server
HEADERS=$(wildcard *.h)
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
CCFLAGS=-Wall
LDFLAGS=
CC=gcc

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) 

%.o: %.c $(HEADERS)
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
