CC = gcc
SA = scan-build
OUT = test_protocol
_FILES = marshal.c protocol.c bot.c test_protocol.c
FDIR = src
FILES= $(patsubst %,$(FDIR)/%,$(_FILES))
CFLAGS=-Wall --std=gnu99
build:
	$(CC) -o $(OUT) $(FILES) $(CFLAGS)
check:
	$(SA) make build
clean:
	rm -f *.o core
rebuild: clean build
