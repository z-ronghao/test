CC = gcc
TARGET = client
OBJECTS = $(TARGET).o
#OBJECTS := $(wildcard *.o)
SOURCE = $(TARGET).c
#SOURCE := $(wildcard *.c)
#obj = $(patsubst ./src/%.c, ./obj/%.o, $(src))
RMRF = rm 


$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJECTS): $(SOURCE)
	$(CC) -c $(SOURCE) -o $(OBJECTS)

.PHONY : clean
clean:
	$(RMRF) $(TARGET) $(OBJECTS)