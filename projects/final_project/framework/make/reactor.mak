CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread -Iinclude

SRC := src/reactor.cpp \
       src/select_listener.cpp

TEST := test/reactor_test.cpp

TARGET := reactor_test

all: $(TARGET)

$(TARGET): $(SRC) $(TEST)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
