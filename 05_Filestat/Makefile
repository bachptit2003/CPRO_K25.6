# Makefile for filestat program
# Trình biên dịch và build automation

# Compiler và flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

# Tên chương trình đầu ra
TARGET = filestat

# Các file nguồn
SRCS = filestat.c filestat_utils.c

# Các file object tương ứng
OBJS = $(SRCS:.c=.o)

# Các file header
HEADERS = filestat.h

# ======================== TARGETS ========================

# Target mặc định: build chương trình
all: $(TARGET)

# Link các object files thành executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile các file .c thành .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Xóa các file được tạo ra
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild từ đầu
rebuild: clean all

# Chạy chương trình với file test
test: $(TARGET)
	@echo "=== Testing with current directory ==="
	./$(TARGET) .
	@echo ""
	@echo "=== Testing with this Makefile ==="
	./$(TARGET) Makefile
	@echo ""
	@echo "=== Testing without arguments ==="
	-./$(TARGET)

# Phony targets (không phải file thật)
.PHONY: all clean rebuild test
