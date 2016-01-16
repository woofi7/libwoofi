CC=egcc
CPPFLAGS=-I ./include/
CFLAGS=-Wall -Wextra -Werror -g -std=c99 -fdiagnostics-color=auto #-pg
LDFLAGS=-pg -lefence
TARGET=libwoofi.a
TEST_TARGET=run_test

SRC=arraylist.c circularqueue.c stack.c
SRCS=$(addprefix src/,$(SRC))
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

test: LDFLAGS+= -lpthread -lcriterion -lc -lutil
test: CPPFLAGS+= -DWITH_TEST
test: $(TEST_TARGET)
	./$(TEST_TARGET)


$(TEST_TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) $(TEST_TARGET)

.PHONY: clean
