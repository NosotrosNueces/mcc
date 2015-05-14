CC      = clang
SA      = scan-build
TARGET  = mcc

CFLAGS  = -Wall -Isrc --std=gnu99
LDFLAGS = -lpthread -lm

OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))
TEST    := $(patsubst %.c,%.o,$(wildcard test/*.c))
SAMPLE  := $(patsubst %.c,%.o,$(wildcard sample/*.c))

all: $(TARGET)

$(TARGET): bin $(OBJECTS) $(SAMPLE)
	$(CC) $(LDFLAGS) $(OBJECTS) $(SAMPLE) -o $@

debug: CFLAGS += -g
debug: clean
debug: all

.PHONY: test
test: CFLAGS += -g
test: bin $(OBJECTS) $(TEST)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST) -o bin/$@
	bin/$@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST) $(SAMPLE)
	$(RM) $(TARGET)

bin:
	mkdir -p $@

scan:
	$(SA) make all

format:
	astyle -n -r --style=linux *.c *.h
