P=tests
OBJECTS=../historing.o
CFLAGS=-Wall -Werror -std=c99 `pkg-config --cflags glib-2.0` -Idummy_includes
LDLIBS=`pkg-config --libs glib-2.0`
CC=gcc

$(P): $(OBJECTS)

run: $(P)
	./$(P)

clean:
	rm $(OBJECTS)
	rm $(P)
