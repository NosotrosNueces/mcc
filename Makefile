VERSION = 0.0.1

CC      = clang
SA      = scan-build

DIR 	:= $(pwd)

CFLAGS  = -Wall -Isrc --std=c11 -D_GNU_SOURCE -g
LDFLAGS = -lpthread -lm -luv -lz -lssl -lcrypto

SRC		= src
LIB		= lib
SHAREDLIB	= $(LIB)/libmcc.so.$(VERSION)
OBJ		= obj
BIN		= bin
TARGET  = $(BIN)/mcc
SRCFILES	:= $(wildcard $(SRC)/*.c)
OBJECTS 	:= $(patsubst $(SRC)/%.c,$(OBJ)/%.o, $(SRCFILES))

all: $(TARGET)

$(TARGET): $(SHAREDLIB) | $(BIN)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

debug: CFLAGS += -g
debug: clean
debug: all


# Rule for making all object files
$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) -c -fpic $(CFLAGS) $< -o $@

# Rule for making shared object file
$(SHAREDLIB): $(OBJECTS) | $(LIB)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)


.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(BIN)/* $(SHAREDLIB)

.PHONY: %.lint
%.lint: %
	$(CC) -fsyntax-only $(CFLAGS) $^

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
