CC      = clang
SA      = scan-build

CFLAGS  = -Wall -Isrc --std=gnu99
LDFLAGS = -lpthread -lm

SRC		= src
LIB		= lib
BIN		= bin
TARGET  = $(BIN)/mcc
TEST    := $(patsubst %.c,%.o,$(wildcard test/*.c))
SRCFILES	:= $(wildcard $(SRC)/*.c)
OBJECTS 	:= $(patsubst $(SRC)/%.c,$(LIB)/%.o, $(SRCFILES))
SAMPLE		:= $(patsubst %.c,%.o,$(wildcard sample/*.c))

all: $(TARGET)

$(TARGET): $(OBJECTS) $(SAMPLE) | $(BIN)
	$(CC) $(LDFLAGS) $(OBJECTS) $(SAMPLE) -o $@

$(LIB)/%.o: $(SRC)/%.c | $(LIB)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: tests
tests: $(OBJECTS) $(TEST) | $(BIN)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST) -o bin/$@
	bin/$@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST) $(SAMPLE)
	$(RM) $(BIN)/*

$(LIB):
	mkdir -p $@

$(BIN):
	mkdir -p $@

scan:
	$(SA) make all

format:
	astyle -n -r --style=linux *.c *.h
