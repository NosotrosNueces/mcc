CC = clang
SA = scan-build
_LIB_FILES = marshal.c protocol.c bot.c
_TEST_FILES = packet_test.c protocol_test.c test_runner.c
LIB_DIR = src
TEST_DIR = test
LIB_FILES= $(patsubst %,$(LIB_DIR)/%,$(_LIB_FILES))
TEST_FILES= $(patsubst %,$(TEST_DIR)/%,$(_TEST_FILES))
CFLAGS=-Wall --std=gnu99 -Wfatal-errors -lpthread
tests: bin
	$(CC) -o bin/tests $(LIB_FILES) $(TEST_FILES) -I $(LIB_DIR) $(CFLAGS)
	./bin/tests
sample: bin
	$(CC) -o bin/sample_bot src/sample_bot.c $(LIB_FILES) $(CFLAGS)
bin:
	mkdir bin
check:
	$(SA) make build
clean:
	rm -f *.o core
rebuild: clean build
