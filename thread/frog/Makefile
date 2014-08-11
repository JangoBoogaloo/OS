PROJECT := frog_game
RUN_SCRIPT := frog_game

SRC_DIR := src
OBJ_DIR := obj
SOURCES := console.c wood.c general.c
TEST_SOURCES := 
APP_SOURCES := main.c

# Compile
MYLIBS_CFLAGS := -I. -I/usr/local/include 
MYLIBS_LIBS := -lncurses 
CC := clang

CFLAGS := -Wall -pedantic -O3 -g $(MYLIBS_CFLAGS)
LDFLAGS := -g
LDLIBS := $(MYLIBS_LIBS)

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SOURCES:%.c=%.o))
TEST_OBJECTS := $(addprefix $(OBJ_DIR)/, $(TEST_SOURCES:%.c=%.o))
APP_OBJECTS := $(addprefix $(OBJ_DIR)/, $(APP_SOURCES:%.c=%.o))

$(PROJECT): $(OBJECTS) $(APP_OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(APP_OBJECTS) $(LDLIBS)

all: $(PROJECT)

analyze:
	clang --analyze $(CFLAGS) $(addprefix $(SRC_DIR)/, $(SOURCES)) $(addprefix $(SRC_DIR)/, $(APP_SOURCES))

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o *.plist $(PROJECT)
clear:
	clear

run: all
	./$(RUN_SCRIPT)

again: clear clean all run

.PHONY: clean clear

#test: $(OBJECTS) $(TEST_OBJECTS)
#$(CC) $(LDFLAGS) -o run_tests $(OBJECTS) $(TEST_OBJECTS) $(LDLIBS)
#./run_tests

install: all
	install -m 755 $(PROJECT) $(DESTDIR)/usr/local/bin/$(PROJECT)