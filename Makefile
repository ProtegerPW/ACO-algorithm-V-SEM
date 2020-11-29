CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -Ofast



BIN		:= bin
SRC		:= src
INCLUDE	:= include

EXECUTABLE	:= ACO.o


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@

clean:
	-rm $(BIN)/*
