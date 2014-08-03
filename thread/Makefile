# Makefile Template

PROJECT := frog_game
RUN_SCRIPT := frog_game
SOURCES := console.c 
TEST_SOURCES := 
APP_SOURCES := main.c

# Compile
MYLIBS_CFLAGS := -I. -I/usr/local/include 
MYLIBS_LIBS := -lncurses 
CC := clang

CFLAGS := -Wall -pedantic -Wextra -O3 -g $(MYLIBS_CFLAGS)
LDFLAGS := -g
LDLIBS := $(MYLIBS_LIBS)

OBJECTS := $(SOURCES:%.c=%.o)
TEST_OBJECTS := $(TEST_SOURCES:%.c=%.o)
APP_OBJECTS := $(APP_SOURCES:%.c=%.o)

$(PROJECT): $(OBJECTS) $(APP_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(APP_OBJECTS) $(LDLIBS)

all: $(PROJECT)

analyze:
	clang --analyze $(CFLAGS) $(SOURCES) $(APP_SOURCES)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.plist $(PROJECT)
clear:
	clear

run: all
	./$(RUN_SCRIPT)

again: clear clean all run

.PHONY: clean clear

test: $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) -o run_tests $(OBJECTS) $(TEST_OBJECTS) $(LDLIBS)
	./run_tests

install:
	install -m 755 $(PROJECT) $(DESTDIR)/usr/sbin/$(PROJECT)
