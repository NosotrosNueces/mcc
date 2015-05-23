VERSION = 0.0.1

CC      = clang
SA      = scan-build

DIR 	:= $(shell pwd)

CFLAGS  = -c -fpic -Wall -Isrc --std=gnu99
LDFLAGS = -L$(DIR)/$(LIB) -lpthread -lm -lmcc

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
	$(CC) $(SAMPLE) -o $@ $(LDFLAGS)

# Rule for making all object files
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) $< -o $@

# Rule for making shared object file
$(SHAREDLIB): $(OBJECTS) | $(LIB)
	$(CC) -shared -o $@ $(OBJECTS)
	ln $(SHAREDLIB) $(LIB)/libmcc.so

.PHONY: test
test: export LD_LIBRARY_PATH=$(DIR)/$(LIB)
test: $(SHAREDLIB) $(TEST) | $(BIN)
	$(CC) $(TEST) -o $(BIN)/$@ $(LDFLAGS)
	$(BIN)/$@

# I don't understand this rule, but it works
.PHONY: run
run: export LD_LIBRARY_PATH=$(DIR)/$(LIB)
run: $(SHAREDLIB)
	$(BIN)/$(EXEC)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST) $(SAMPLE) $(BIN)/* $(SHAREDLIB)
	unlink $(LIB)/libmcc.so

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
