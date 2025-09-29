# ------------------------------
# Watchdog Project Makefile
# ------------------------------

# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -pthread -g
INC     = -I../watch_dog/inc -I../sched/inc -I../../ds/inc

# Output directory
BIN_DIR = ../bin/debug

# Source directories
WD_TEST_DIR   = ../watch_dog/test
WD_SRC_DIR    = ../watch_dog/src
SCHED_SRC_DIR = ../sched/src
DS_SRC_DIR    = ../../ds/src

# Sources
USER_SRC      = $(WD_TEST_DIR)/user.c
WATCHDOG_SRC  = $(WD_TEST_DIR)/watch_dog.c
WD_SRC        = $(WD_SRC_DIR)/wd.c $(WD_SRC_DIR)/wd_internal.c
SCHED_SRC     = $(SCHED_SRC_DIR)/sched.c $(SCHED_SRC_DIR)/task.c
PQ_SRC        = $(DS_SRC_DIR)/pq.c $(DS_SRC_DIR)/sortedl.c $(DS_SRC_DIR)/dll.c
UID_SRC       = $(DS_SRC_DIR)/uid.c

# Targets
USER_BIN      = $(BIN_DIR)/user_prog
WATCHDOG_BIN  = $(BIN_DIR)/watchdog

# Default target
all: $(USER_BIN) $(WATCHDOG_BIN)

# Build user program
$(USER_BIN): $(USER_SRC) $(WD_SRC) $(SCHED_SRC) $(PQ_SRC) $(UID_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INC) -o $@ $(USER_SRC) $(WD_SRC) $(SCHED_SRC) $(PQ_SRC) $(UID_SRC)

# Build watchdog
$(WATCHDOG_BIN): $(WATCHDOG_SRC) $(WD_SRC) $(SCHED_SRC) $(PQ_SRC) $(UID_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(INC) -o $@ $(WATCHDOG_SRC) $(WD_SRC) $(SCHED_SRC) $(PQ_SRC) $(UID_SRC)

# Clean binaries
clean:
	rm -f $(USER_BIN) $(WATCHDOG_BIN)

.PHONY: all clean
