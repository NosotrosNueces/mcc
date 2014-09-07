CC = gcc
FILES = protocol.c
OUT = MC

build: $(FILES)
	$(CC) -o $(OUT) $(FILES)

clean:
	rm -f *.o core

rebuild: clean build
