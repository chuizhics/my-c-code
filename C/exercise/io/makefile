# Makefile

# Compiler and linker options
CC = gcc

#The line "CFLAGS = -Wall -Werror -Wextra -pedantic" is a common example of compiler flags used in C programming.
#“CFLAGS = -Wall -Werror -Wextra -pedantic”这一行是 C 编程中使用的编译器标志的常见示例。
#The "-Wall" flag enables all warning messages that the compiler can generate, which can help identify potential issues in the code.
#“-Wall”标志启用编译器可以生成的所有警告消息，这有助于识别代码中的潜在问题。
#The "-Werror" flag treats all warnings as errors, causing the compiler to halt if any warnings are generated. This can help catch potential bugs before they become serious issues.
#“-Werror”标志将所有警告视为错误，如果生成任何警告，则会导致编译器停止。这有助于在潜在错误成为严重问题之前发现它们。
#The "-Wextra" flag enables additional warning messages beyond those included in "-Wall".
#“-Wextra”标志启用除“-Wall”中包含的警告消息之外的其他警告消息。
#The "-pedantic" flag enforces strict adherence to the C standard, which can help ensure portability and compatibility across different platforms and compilers.
#“-pedantic”标志强制严格遵守 C 标准，这有助于确保跨不同平台和编译器的可移植性和兼容性。
#Overall, these flags are used to promote best practices in C programming and to help catch potential issues before they cause problems in the code.
#总的来说，这些标志用于促进 C 编程中的最佳实践，并帮助在代码中引起问题之前捕获潜在问题。


#CFLAGS = -Wall -Werror -Wextra -pedantic
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -lm

# Output file
TARGET = excercise

# Source files and object files
SRCS = excercise.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
