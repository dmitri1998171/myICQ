#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <time.h>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <rapidjson/filereadstream.h>
#include "rapidjson/document.h"

#define WIDTH 1000
#define HEIGHT 650

#define PORT 53000

using namespace std;
using namespace sf;
using namespace rapidjson;

struct Settings {
    char username[64];
    char status[8];
    int id;
};

struct networkStruct {
    String message;
    struct Settings settings_struct;
};

void die_With_Error(const char *device, const char *error_message);
void json_parser_create(struct Settings* settings_struct);
void network_func(TcpSocket* socket);
