VERSION = 0.0.1
PROTOCOL_VERSION = 315

CC      = clang
SA      = scan-build

DIR 	:= $(pwd)


CFLAGS  = -Wall -Werror -Wswitch -Wswitch-enum -Iinclude -std=c11 -D_GNU_SOURCE -DPROTOCOL_VERSION=$(PROTOCOL_VERSION)
LDFLAGS = -lpthread -lm -luv -lz -lssl -lcrypto

SRC		= src
LIB		= lib
SHAREDLIB	= $(LIB)/libmcc.so.$(VERSION)
OBJ		= obj
BIN		= bin
TARGET  = $(BIN)/mcc
SRCFILES	= 	src/protocol.c \
				src/hex.c \
				src/logger.c \
				src/bot.c \
				src/nbt.c \
				src/nbt_print.c \
				src/base64.c \
				src/serial.c \
				src/capture.c \
				src/break.c

OBJECTS 	= $(patsubst $(SRC)/%.c,$(OBJ)/%.o, $(SRCFILES))

all: CFLAGS += -O3
all: $(TARGET)

$(TARGET): $(SHAREDLIB) | $(BIN)
	$(CC) $(CFLAGS) test.c $(OBJECTS) -o $@ $(LDFLAGS)

debug: CFLAGS += -g
debug: $(TARGET)


# Rule for making all object files
$(OBJ)/%.o: $(SRC)/%.c break | $(OBJ)
	$(CC) -c -fpic $(CFLAGS) $< -o $@

# Rule for making shared object file
$(SHAREDLIB): $(OBJECTS) | $(LIB)
	$(CC) -shared -o $@ $(OBJECTS) $(LDFLAGS)

.PHONY: clean
clean: clean-gen
	$(RM) $(OBJECTS) $(BIN)/* $(SHAREDLIB)

.PHONY: %.lint
%.lint: %
	$(CC) -fsyntax-only $(CFLAGS) $^

# Generate source files
.PHONY: break
break: break.py break.json
	./break.py break.json

.PHONY: clean_gen
clean-gen:
	rm src/break.c include/break.h

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
