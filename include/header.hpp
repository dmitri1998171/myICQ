#ifndef ADD_H
#define ADD_H

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
    short font_size;
};

extern int sep, dialog_count;
extern int sidebar_width;
extern float font_size;

void die_With_Error(const char *device, const char *error_message);
void json_parser_create(struct Settings* settings_struct);
void network_func(TcpSocket* socket);
int getCenter(Sprite img, Text text);
void history_dialog(FILE** history, Font* font, RectangleShape* output_rect, RectangleShape* output_text_rect, Text* recv_text, struct Settings *settings_struct);

void create_rect(RectangleShape *rect, Color color, float width, float height, float x, float y);
void text_params_func(Font* font, Text *text, String message, Color color, float x, float y);
void draw_message_rect(Font** font, RectangleShape** output_rect, RectangleShape** output_text_rect, Text** recv_text, const char *str, int pos);
void texture_loader(Texture* texture, string path);
Sprite UI_shedule(Texture* texture, string path, float x, float y);

#endif