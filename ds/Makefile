CC ?= gcc
CFLAGS ?= -ansi -std=c99 -pedantic-errors -Wall -Wextra
TARGET = fooer.out
OBJS = src/foo.o test/main.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


######## creat files .o ########
src/foo.o: src/foo.c include/foo.h
	$(CC) $(CFLAGS) -c $< -o $@

test/main.o: test/main.c include/foo.h
	$(CC) $(CFLAGS) -c $< -o $@

foo.o: src/foo.o
	@ln -sf $< $@
################################

clean:
	rm -f *.o src/*.o test/*.o $(TARGET)

debug: CFLAGS += -g -DDEBUG -O3
debug: clean all

release: CFLAGS = -O2
release: clean all

run: $(TARGET)
	@./$(TARGET)

.PHONY: all clean debug release run


