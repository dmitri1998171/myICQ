#pragma once
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

using namespace std;
using namespace sf;
using namespace rapidjson;

#define WIDTH 1000
#define HEIGHT 650

#define PORT 2517
#define logl(x) cout << x << endl
#define log(x) cout << x

struct Settings {
    char username[64];
    char status[8];
    int id;
    short font_size;
};

extern int sep, dialog_count;
extern int sidebar_width;
extern float font_size;
extern int menu_x, menu_y, menu_w, menu_h;
extern int menu_button_h;

void die_With_Error(const char *device, const char *error_message);
void json_parser_create(struct Settings* settings_struct);
void network_func(TcpSocket* socket);
int getCenter(Sprite img, Text text);
int getCenter_y(RectangleShape img, Text text);
void history_dialog(FILE** history, Font* font, RectangleShape* output_rect, RectangleShape* output_text_rect, Text* recv_text, struct Settings *settings_struct);

void create_rect(RectangleShape *rect, Color color, float width, float height, float x, float y);
void text_params_func(Font* font, Text *text, String message, Color color, float x, float y);
void draw_message_rect(Font** font, RectangleShape** output_rect, RectangleShape* output_text_rect, Text* recv_text, const char *str, int pos);
void texture_loader(Texture* texture, string path);
void sprite_loader(Sprite* sprite, Texture* texture, string path, float x, float y);
void draw_circle_angle_rect(RectangleShape *rect, Color color, int width, int height, int x, int y);
void add_Button_Func(Sprite* sprite, Texture* texture, Color color, Text* text, Font* font, string path, String str, int sprite_pos);
void add_Menu_Button_Func(RectangleShape* rect, Texture* texture, Sprite* sprite, Color color, Text* text, Font* font, string path, String str, int text_pos);
#endif