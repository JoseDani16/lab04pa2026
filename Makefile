CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iheaders
TARGET = programa
SRC = $(wildcard src/*.cpp) \
      $(wildcard ICollection/*.cpp) \
      $(wildcard ICollection/interfaces/*.cpp) \
      $(wildcard ICollection/collections/*.cpp)
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
