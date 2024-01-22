CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SRC = $(wildcard *.cpp)
OBJ1 = $(filter-out test.o, $(SRC:.cpp=.o))
OBJ = $(filter-out main.o, $(SRC:.cpp=.o))
EXECUTABLE = my_program
TEST_EXECUTABLE = tests

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(OBJ) test.o
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: test.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJ) $(EXECUTABLE) $(TEST_EXECUTABLE) test.o main.o

