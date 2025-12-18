CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread -Iinclude

SRC := src/handleton.cpp \
       src/scheduler.cpp

TEST := test/scheduler_test.cpp

TARGET := scheduler_test

all: $(TARGET)

$(TARGET): $(SRC) $(TEST)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
