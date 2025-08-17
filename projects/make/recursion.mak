# Determine build type based on CFLAGS #
ifeq ($(findstring -g,$(CFLAGS)),-g)
    BUILD_DIR = bin/debug
else
    BUILD_DIR = bin/release
endif

.DEFAULT_GOAL := $(BUILD_DIR)/recursion_test.out

INC_DIRS = -I$(RECURSIA_INC_DIR) -I$(DS_INC_DIR) 

# APIs to turn into .so files #
LIB_NAMES = stack sll 
RECURSIA_NAMES = recursion

# Source directories #
DS_INC_DIR = ../ds/inc
DS_SRC_DIR = ../ds/src
RECURSIA_SRC_DIR = recursion/src
RECURSIA_INC_DIR = recursion/inc
TEST_DIR = recursion/test

# .so file names #
DS_SO_FILES = $(addprefix $(DS_SRC_DIR)/lib, $(addsuffix .so, $(LIB_NAMES)))
RECURSIA_SO_FILE = $(addprefix $(RECURSIA_SRC_DIR)/lib, $(addsuffix .so,$(RECURSIA_NAMES)))

TARGET = $(BUILD_DIR)/recursion_test.out
OBJS = $(TEST_DIR)/recursion_test.o

# Create build directory #
$(BUILD_DIR):
	mkdir -p $@

# Build Shared librarys files in ../ds/src/ #
$(DS_SRC_DIR)/lib%.so: $(DS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -shared -o $@ $<

# Build Shared librarys in sched/src/ #
$(RECURSIA_SRC_DIR)/lib%.so: $(RECURSIA_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -shared -o $@ $<

# Build Test object file #
$(TEST_DIR)/recursion_test.o: $(TEST_DIR)/recursion_test.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c -o $@ $<

# build directory before linking (order-only prerequisite) #
$(TARGET): | $(BUILD_DIR)

# Test executable linked with shared library #
$(TARGET): $(OBJS) $(DS_SO_FILES) $(RECURSIA_SO_FILE)
	$(CC) -o $@ $^ -L$(DS_SRC_DIR) -L$(RECURSIA_SRC_DIR) $(addprefix -l, $(LIB_NAMES)) $(addprefix -l, $(RECURSIA_NAMES)) -Wl,-rpath,$(abspath $(DS_SRC_DIR)):$(abspath $(RECURSIA_SRC_DIR))

-include $(DS_SO_FILES:.so=.d) $(RECURSIA_SO_FILE:.so=.d) $(OBJS:.o=.d)






