CC      = clang
TARGET  = mcc

CFLAGS  = -Wall -Isrc
LDFLAGS = -lpthread -lm

OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))
TEST    := $(patsubst %.c,%.o,$(wildcard test/*.c))
SAMPLE  := $(patsubst %.c,%.o,$(wildcard sample/*.c))

all: $(TARGET)

$(TARGET): bin $(OBJECTS) $(SAMPLE)
	$(CC) $(LDFLAGS) $(OBJECTS) $(SAMPLE) -o $@

.PHONY: test
test: bin $(OBJECTS) $(TEST)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST) -o bin/$@
	bin/$@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST) $(SAMPLE)
	$(RM) $(TARGET)

bin:
	mkdir -p $@

format:
	astyle -n -r --style=linux *.c *.h
