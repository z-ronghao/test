CC = gcc
TARGET = conversion
SOURCE = $(wildcard ./*.c)
OBJECTS = $(patsubst ./%.c, ./%.o, $(SOURCE))
INCLUDE = -I./include/
LIB_PATH = ./include/lib
LIBS = -lturbojpeg -lpng


$(TARGET):$(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -L $(LIB_PATH) $(LIBS) -Wl,-rpath,$(LIB_PATH)
	
$(OBJECTS):./%.o:./%.c
	$(CC) $(INCLUDE) -c $^ -o $@

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS)