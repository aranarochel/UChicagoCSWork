OBJS = main.o list.o functions.o handlers.o dispatch-table.o channels.o
DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -DMUTEX
BIN = ../ps2
LDLIBS = -pthread

all: $(BIN)
	
$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $(BIN)
	
%.d: %.c

clean:
	-rm -f $(OBJS) $(BIN) *.d
