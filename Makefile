CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Ivendor/raylib/src
LDLIBS = vendor/raylib/src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

snake: main.o vendor/raylib/src/libraylib.a
	$(CC) -o $@ main.o $(LDLIBS)

main.o: src/main.c
	$(CC) -c $< $(CFLAGS)

vendor/raylib/src/libraylib.a:
	$(MAKE) -C vendor/raylib/src

.PHONY: clean

clean:
	rm -f main.o snake
	$(MAKE) -C vendor/raylib/src clean
