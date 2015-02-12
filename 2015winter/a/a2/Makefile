PROJECT := app

#set the compiler option
CFLAGS := -Wall \
				  -pedantic \
				  -Wextra \
				  -Wno-missing-field-initializers \
				  -g
CC := gcc 

#header include
INCLUDES := -I.#\ 
#library link
LD_LIBS := -lpthread #\ 

OBJECT_DIR := obj/
SOURCE_DIR := src/

SOURCES :=$(wildcard $(SOURCE_DIR)*.c) 
OBJECTS :=$(SOURCES:$(SOURCE_DIR)%.c=$(OBJECT_DIR)%.o)

all: $(PROJECT) $(OBJECTS)

$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROJECT) $(OBJECTS) $(LD_LIBS)

$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
		rm -rf $(OBJECT_DIR)*.o $(PROJECT)
