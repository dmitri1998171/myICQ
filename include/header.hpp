// #ifndef 
// #define
#include <iostream>
#include <cstdio>
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

void dieWithError(const char *error_message);
void network_func();
void SFML_rendering();

// #endif