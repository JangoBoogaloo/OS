PROJECT :=umguo62_a2
SCRIPT :=$(PROJECT) 10 1216.txt #add arguments

#set the compiler option
CFLAGS :=-Wall \
				 -pedantic \
				 -Wextra \
				 -Wno-missing-field-initializers \
				 -g

CC :=clang

#header include
INCLUDES := -I.#\ 
#library link
LD_LIBS := -lpthread #\ 

OBJECT_DIR :=obj/
SOURCE_DIR :=src/

SOURCES :=$(wildcard $(SOURCE_DIR)*.c) 
OBJECTS :=$(SOURCES:$(SOURCE_DIR)%.c=$(OBJECT_DIR)%.o)

$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROJECT) $(OBJECTS) $(LD_LIBS)

$(OBJECT_DIR)%.o: $(SOURCE_DIR)%.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(PROJECT) $(OBJECTS)

run: all
	./$(SCRIPT)

clean:
		rm -f $(OBJECT_DIR)*.o $(PROJECT)
