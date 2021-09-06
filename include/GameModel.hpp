#pragma once
#include "gui.hpp"

class GameModel {
	public:
		struct Settings settings_struct;
		struct GraphicsThread {
			Font* font;
			TcpSocket* socket;
			RenderWindow* window;
			RectangleShape* output_rect;
			RectangleShape output_text_rect[STR_SIZE];
			Text recv_text[STR_SIZE];
			float output_rect_pos;
		}thread_struct;
	
		Text text, search_text;
		Text settings_text, chats_text, contacts_text;
		Text add_contact_text, add_group_text, add_channel_text, read_all_text;

		RectangleShape background, output_rect, input_rect, side_rect, line, search;

		Assets* assets = Assets::getInstance();
		Registry reg;
		IDrawUI drawUI;
		IGUI ui;

		GameModel();
		void loadAssets();
		void Init();
		void sendMessage();
		bool Check();
		void onClick(RenderWindow *);
		void textInput(Event *event);
		void KeyPressedEnter();
		void KeyPressedBackspace();
};
