// #ifndef 
// #define
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <rapidjson/filereadstream.h>
#include "rapidjson/document.h"

#define WIDTH 640 
#define HEIGHT 480
#define OUTLINE_WIDTH 5

#define PORT 53000

using namespace std;
using namespace sf;
using namespace rapidjson;

struct Settings {
    char username[64];
    char status[8];
    int id;
};

void die_With_Error(const char *device, const char *error_message);
void json_parser_create(struct Settings* settings_struct);
void SFML_GUI(struct Settings settings_struct);
// void network_func(struct Settings settings_struct);

// #endif