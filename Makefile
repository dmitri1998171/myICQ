BIN := main
SERVER := server
SOURCE_MAIN := main.cpp 
SOURCE_EXT := draw.cpp gui.cpp extention.cpp
SOURCE_MVC := GameModel.cpp GameRender.cpp GameController.cpp
SOURCE_SERVER := server_tcp.cpp
CC := g++
CPPFLAGS := -I../include -L../lib -g
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

all: $(BIN) $(SERVER)

$(BIN):
	cls && cd ./src && $(CC) $(SOURCE_MAIN) $(SOURCE_EXT) $(SOURCE_MVC) $(CPPFLAGS) $(LDFLAGS) -o ../bin/$(BIN).exe
$(SERVER):
	cls && cd ./src && $(CC) $(SOURCE_SERVER) $(CPPFLAGS) $(LDFLAGS) -o ../bin/$(SERVER).exe

clean:
	rm -rf /bin