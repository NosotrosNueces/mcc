CC = clang
SA = scan-build
_LIB_FILES = marshal.c protocol.c bot.c client.c handlers.c api.c pipe.c json.c
_TEST_FILES = packet_test.c protocol_test.c test_runner.c
_SAMPLE_FILES = bot_runner.c sample_defender.c sample_slave.c
LIB_DIR = src
TEST_DIR = test
SAMPLE_DIR = sample
LIB_FILES= $(patsubst %,$(LIB_DIR)/%,$(_LIB_FILES))
TEST_FILES= $(patsubst %,$(TEST_DIR)/%,$(_TEST_FILES))
SAMPLE_FILES= $(patsubst %,$(SAMPLE_DIR)/%,$(_SAMPLE_FILES))
CFLAGS=-Wall --std=gnu99 -Wfatal-errors -g
CLIBS=-lpthread -lm 
tests: bin
	$(CC) -o bin/tests $(LIB_FILES) $(TEST_FILES) -I $(LIB_DIR) $(CFLAGS) $(CLIBS)
	./bin/tests
build: bin
	$(CC) -o bin/sample_bot $(SAMPLE_FILES) $(LIB_FILES) -I $(LIB_DIR) $(CFLAGS) $(CLIBS)
bin:
	mkdir bin
check:
	$(SA) make build
clean:
	rm -f bin/*
rebuild: clean build
format:
	astyle -n -r --style=linux *.c *.h
