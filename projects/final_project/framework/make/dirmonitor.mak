CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread -Iinclude
LDFLAGS := -ldl

SRC := src/dirmonitor.cpp \
       src/dllloader.cpp \
       src/logger.cpp \
       src/scheduler.cpp \
       src/handleton.cpp

TEST := test/dirmonitor_test.cpp

TARGET := dirmonitor_test

all: $(TARGET)

$(TARGET): $(SRC) $(TEST)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
