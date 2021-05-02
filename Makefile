BIN := main
SERVER := server
SOURCE_MAIN := main.cpp #./src/graphics.cpp #./src/network.cpp
SOURCE_EXT := extention.cpp 
SOURCE_SERVER := server_tcp.cpp
CC := g++
CPPFLAGS := -I./include -L./lib
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

$(BIN):
	cls && $(CC) ./src/$(SOURCE_MAIN) ./src/$(SOURCE_EXT) $(CPPFLAGS) $(LDFLAGS) -o ./bin/$(BIN).exe
$(SERVER):
	cls && $(CC) ./src/$(SOURCE_SERVER) ./src/$(SOURCE_EXT) $(CPPFLAGS) $(LDFLAGS) -o ./bin/$(SERVER).exe

clean:
	rm -rf /bin