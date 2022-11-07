CC = gcc
OBJECTS = main.o
TARGET = main
LIB_INCLUDE = ./include
LIB_PATH = ./include/lib
LIBS = -lturbojpeg -lyuv


$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)  -L $(LIB_PATH) $(LIBS) -Wl,-rpath,$(LIB_PATH)
$(OBJECTS) : main.c
	$(CC) -I  $(LIB_INCLUDE) -c main.c

$(info "生成可执行文件mian")

.PHONY : clean
clean:
	rm -rf $(TARGET) $(OBJECTS)
