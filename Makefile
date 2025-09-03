CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = remover
SOURCES = main.c metadata_remover.c



all: $(TARGET)


$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)


clean:
	rm -f $(TARGET)