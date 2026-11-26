PROJNAME = terraLife
VERSION = 0.1

CC = gcc
CFLAGS = -DGRAPHICS_API_OPENGL_43 -Wall -Wextra -DVERSION='"$(VERSION)"' -O3 -g -O0 -DDEBUG
LDFLAGS = -O3 -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer
LDLIBS = -L$(RAYLIB_DIR) -lraylib -lX11 -lGL -ldl -lrt -lm -lpthread

RAYLIB_DIR = raylib/src
RAYLIB_LIB = $(RAYLIB_DIR)/libraylib.a

SRC_DIRS = src
INCLUDE_DIRS = include raylib/src

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

$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP GRAPHICS=GRAPHICS_API_OPENGL_43 RAYLIB_LIBTYPE=STATIC

$(EXE): $(OBJS) $(RAYLIB_LIB) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIRS)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

-include $(DEPS)

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)
	$(MAKE) -C $(RAYLIB_DIR) clean

