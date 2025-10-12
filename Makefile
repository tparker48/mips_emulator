# Compiler
CC = gcc
CFLAGS = -Wall -O2 -Iinclude

# Source files
SRCS = src/mips_alu.c \
       src/mips_memory.c \
       src/mips_registers.c \
       src/mips_pipeline.c \
       src/mips_instructions.c \
       src/mips_main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = mips_sim

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
