# Makefile

# Compiler and linker options
CC = gcc


#CFLAGS = -Wall -Werror -Wextra -pedantic
CFLAGS = -Wall -Wextra -pedantic

#LDFLAGS = -lm

# Output file
TARGET = btree

# Source files and object files
SRCS = main.c btree.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)
# $(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean