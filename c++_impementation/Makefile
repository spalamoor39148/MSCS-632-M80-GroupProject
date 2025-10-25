# ==== Expense Tracker CLI (macOS) ====

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/opt/homebrew/include -I.
LDFLAGS =

SRC = main.cpp Utils.cpp Expense.cpp FileManager.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = expense_tracker

# Default rule
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
