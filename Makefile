BIN := main
SERVER := server
SOURCE_MAIN := ./src/main.cpp ./src/network.cpp ./src/graphics.cpp
SOURCE_EXT := extention.cpp 
SOURCE_SERVER := server_tcp.cpp
CC := g++
CPPFLAGS := -I./include -L./lib
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

$(BIN):
	cls && $(CC) $(SOURCE_MAIN) ./src/$(SOURCE_EXT) $(CPPFLAGS) $(LDFLAGS) -o ./bin/$(BIN).exe
$(SERVER):
	cls && $(CC) ./src/$(SOURCE_SERVER) ./src/$(SOURCE_EXT) $(CPPFLAGS) $(LDFLAGS) -o ./bin/$(SERVER).exe

clean:
	rm -rf /bin