CC = gcc 
SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
CFLAGS=-lpthread

PROGRAM=fileserver

$(PROGRAM) : $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) $(CFLAGS)
$(OBJECTS):$(SOURCES)

.PHONY : clean
clean : 
	rm -rf $(OBJECTS) $(PROGRAM)



