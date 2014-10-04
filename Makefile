CC = clang
SA = scan-build
OUT = test_bot
_FILES = marshal.c protocol.c bot.c
FDIR = src
FILES= $(patsubst %,$(FDIR)/%,$(_FILES))
CFLAGS=-Wall --std=gnu99 -Wfatal-errors
build:
	$(CC) -o $(OUT) $(FILES) $(CFLAGS)
check:
	$(SA) make build
clean:
	rm -f *.o core
rebuild: clean build
