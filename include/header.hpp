#pragma once
#ifndef ADD_H
#define ADD_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <map>
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

enum States {
    CHAT_STATE,
    SETTINGS_STATE,
    CONTACTS_STATE
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

class Registry {
    public:
        int winX = 0, winY = 0;
        int state = CHAT_STATE;
        int write_flag = 0, menu_flag = 0, add_chat_flag = 0, contact_add_flag = 0;
        int min_x = 470, min_y = 600, sub_min_x = 830;
        float output_rect_pos = HEIGHT * 0.9;
        float input_rect_pos = HEIGHT - output_rect_pos  - 20;
        Color line_color = Color(128, 128, 128);
        Color output_color = Color(27, 28, 37);
        Color sidebar_color = Color(39, 40, 49);
        Font font;
        Text text;
        Text search_text;
        String message = "Enter a message...";

        map<string, Sprite> buttons;    // хеш-таблица
        Sprite settings_button, chats_button, contacts_button,
        add_button, menu_button;
        
        Registry() {
            buttons["settings"] = settings_button;
            buttons["chats"] = chats_button;
            buttons["contacts"] = contacts_button;
            buttons["menu"] = menu_button;
            buttons["add"] = add_button;
        }
};

#endif