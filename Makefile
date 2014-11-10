CC      = clang
SA      = scan-build
TARGET  = mcc

CFLAGS  = -Wall -Isrc -IcNBT --std=gnu99
LDFLAGS = -lpthread -lm -Llib -lnbt

OBJECTS := $(patsubst %.c,%.o,$(wildcard src/*.c))
TEST    := $(patsubst %.c,%.o,$(wildcard test/*.c))
SAMPLE  := $(patsubst %.c,%.o,$(wildcard sample/*.c))

all: $(TARGET)

$(TARGET): libs bin $(OBJECTS) $(SAMPLE)
	$(CC) $(LDFLAGS) $(OBJECTS) $(SAMPLE) -o $@

.PHONY: test
test: bin $(OBJECTS) $(TEST)
	$(CC) $(LDFLAGS) $(OBJECTS) $(TEST) -o bin/$@
	bin/$@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TEST) $(SAMPLE)
	$(RM) $(TARGET)

libs: lib lib/libnbt.a
lib:
	mkdir -p $@
bin:
	mkdir -p $@

scan:
	$(SA) make all

format:
	astyle -n -r --style=linux *.c *.h

lib/libnbt.a: lib
	make -C cNBT libnbt.a
	mv cNBT/libnbt.a lib/
