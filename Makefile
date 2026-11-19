PROJNAME = terraLife
VERSION = 0.1

CC = gcc
CFLAGS = -Wall -Wextra -DVERSION='"$(VERSION)"' -O3 -g -O0 -DDEBUG
CPPFLAGS =
LDFLAGS = -O3 -g -O0 #-fsanitize=address,undefined -fno-omit-frame-pointer
LDLIBS = -lm -lpthread -lraylib

SRC_DIRS = src
INCLUDE_DIRS = include

BUILD_DIR = .build

EXE = $(BUILD_DIR)/$(PROJNAME)

SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS = $(patsubst $(SRC_DIRS)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

.PHONY: all clean

all: $(EXE)

run: $(EXE)
	cp -r shader $(BUILD_DIR)
	./$(EXE)

$(EXE): $(OBJS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

-include $(DEPS)

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)

