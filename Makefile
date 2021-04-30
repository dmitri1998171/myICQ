BIN := main
SOURCE := main.cpp
CC := g++
CPPFLAGS := -I./include -L./lib
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

$(BIN):
	cls && $(CC) $(SOURCE) $(CPPFLAGS) $(LDFLAGS) -o ./bin/$(BIN).exe
clean:
	