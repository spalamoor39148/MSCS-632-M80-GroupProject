# ==== Expense Tracker CLI (macOS) ====

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include -I.
LDFLAGS =
CATCH2 = /opt/homebrew/include/catch2

SRC = main.cpp Utils.cpp Expense.cpp FileManager.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = expense_tracker

TEST_SRC = test_expense_tracker.cpp
TEST_OBJ = $(TEST_SRC:.cpp=.o)
TEST_TARGET = test_expense_tracker

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) Expense.o FileManager.o Utils.o
	$(CXX) $(CXXFLAGS) -I$(CATCH2) -o $@ $^ -L/opt/homebrew/lib

clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)
