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

#define STR_SIZE 256
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

extern int sep;
extern int dialog_count;
extern float font_size;

void die_With_Error(const char *device, const char *error_message);
void json_parser_create(struct Settings* settings_struct);
void network_func(TcpSocket* socket);
void history_dialog(FILE** history, Font* font, RectangleShape* output_rect, RectangleShape* output_text_rect, Text* recv_text, struct Settings *settings_struct);
void splitString(string str, string tokens[]);
void setDate(string *systemTime, string date_time[6], string *date);

class Registry{
    public:
        int winX = 0, winY = 0;
        int state = CHAT_STATE;
        int write_flag = 0, menu_flag = 0, add_chat_flag = 0, contact_add_flag = 0;
        int stringsCount = 0, stringsLexemes = 0;
        int min_x = 470, min_y = 600, sub_min_x = 830;
        time_t t;//=time(NULL);
        float output_rect_pos = HEIGHT * 0.9;
        float input_rect_pos = HEIGHT - output_rect_pos  - 20;
        
        string date;
        string time;
        string systemTime = ctime(&t);
        string date_time[6];

        Color line_color = Color(128, 128, 128);
        Color output_color = Color(27, 28, 37);
        Color sidebar_color = Color(39, 40, 49);
        String message = "Enter a message...";
        Font font;
	    TcpSocket socket;
		Packet sendPacket;

        
            /* Textures */ 
        Texture settings_texture, chats_texture, contacts_texture,
        add_chat_texture, add_contact_texture, menu_texture;

        Texture settings_active_texture, chats_active_texture, contacts_active_texture,
        add_chat_active_texture, add_contact_active_texture, menu_active_texture;

        Texture menu_add_contact_texture, menu_add_group_texture,
        menu_add_channel_texture, menu_read_all_texture;
};

#endif