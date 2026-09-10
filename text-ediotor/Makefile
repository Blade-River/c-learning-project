CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = myprogram

# Automatically find all .c files in the current folder
SRCS = $(wildcard *.c)

# Convert the list of .c files into .o files
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

# Link the executable ($@ matches target name, $^ matches all dependencies)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile any .c file into a .o file ($< matches the first dependency)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

