CC = gcc
TARGET = ls_l
OBJECTS = $(TARGET).o
SOURCE = $(TARGET).c
RMRF = rm


$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJECTS): $(SOURCE)
	$(CC) -c $(SOURCE) -o $(OBJECTS)

.PHONY : clean
clean:
	$(RMRF) $(TARGET) $(OBJECTS)