VERSION = 0.0.1

CC      = clang
SA      = scan-build

DIR 	:= $(pwd)

CFLAGS  = -c -fpic -Wall -Isrc --std=gnu99
LDFLAGS = -lpthread -lm

SRC		= src
LIB		= lib
SHAREDLIB	= $(LIB)/libmcc.so.$(VERSION)
OBJ		= obj
BIN		= bin
TARGET  = $(BIN)/mcc
TEST    := $(patsubst %.c,%.o,$(wildcard test/*.c))
SRCFILES	:= $(wildcard $(SRC)/*.c)
OBJECTS 	:= $(patsubst $(SRC)/%.c,$(OBJ)/%.o, $(SRCFILES))
SAMPLE		:= $(patsubst %.c,%.o,$(wildcard sample/*.c))

all: $(TARGET)

$(TARGET): $(SHAREDLIB) $(SAMPLE) | $(BIN)
	$(CC) $(LDFLAGS) $(OBJECTS) $(SAMPLE) -o $@

debug: CFLAGS += -g
debug: clean
debug: all


# Rule for making all object files
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) $< -o $@

# Rule for making shared object file
$(SHAREDLIB): $(OBJECTS) | $(LIB)
	$(CC) -shared -o $@ $(OBJECTS)

.PHONY: test
test: CFLAGS += -g
test: bin $(OBJECTS) $(TEST)
test: $(SHAREDLIB) $(TEST) | $(BIN)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST) -o bin/$@
	bin/$@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST) $(SAMPLE) $(BIN)/* $(SHAREDLIB)

$(OBJ):
	mkdir -p $@

$(LIB):
	mkdir -p $@

$(BIN):
	mkdir -p $@

.PHONY: scan
scan:
	$(SA) make all

.PHONY: format
format:
	astyle -n -r --style=linux *.c *.h
