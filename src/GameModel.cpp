#include "GameModel.hpp"

GameModel::GameModel()
{
	Init();
}

void GameModel::loadAssets() {
	reg.font.loadFromFile("./media/fonts/CyrilicOld.TTF");

    assets->texture_loader(&reg.add_contact_texture, "./media/icons/add_contact.png");
    assets->texture_loader(&reg.chats_texture, "./media/icons/chats.png");
    assets->texture_loader(&reg.settings_active_texture, "./media/icons/active/settings_active.png");
    assets->texture_loader(&reg.contacts_active_texture, "./media/icons/active/contacts_active.png");
    
    assets->sprite_loader(&ui.add_button, &reg.add_chat_texture, "./media/icons/add_chat.png", drawUI.sidebar_width - 35, 5);
    assets->sprite_loader(&ui.menu_button, &reg.menu_texture, "./media/icons/menu.png", 0, 5);
}

void GameModel::Init()
{
	// json_parser_create(&settings_struct);
    // font_size = settings_struct.font_size;
    
	loadAssets();
    // network_func(&reg.socket);
}

void GameModel::sendMessage() {
	if (reg.socket.send(reg.sendPacket) == Socket::Done) {
		reg.time = reg.date_time[3]; 						// set time
		setDate(&reg.systemTime, reg.date_time, &reg.date); // set date
		logl(reg.date);

		reg.message.insert(reg.message.getSize(), reg.time);
		// drawUI.createMessageRect(&thread_struct.font, &thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, reg.message.toAnsiString().c_str(), 1);
		reg.sendPacket.clear();
		reg.message.clear();
		text.setString(reg.message);
	}
	else
		die_With_Error(DEVICE, "Failed to send a message to server!");
}

void GameModel::onClick(RenderWindow *window) {
	Vector2i mouse_pos = Mouse::getPosition(*window);

	text.setFillColor(reg.line_color);
	if(reg.message.getSize() == 0) {
		reg.message = "Enter a message...";
		text.setString(reg.message);
	}
	
// input_rect
	if(ui.checkToClickRect(window, &input_rect)) {
		if(reg.message == "Enter a message...") {
			reg.message.clear();
			text.setString("");
		}
		reg.write_flag = 1;
		text.setFillColor(Color::White);
	}
	else
		reg.write_flag = 0;

// Sidebar buttons
	if(ui.checkToClickSprite(window, &ui.settings_button)) {
		logl("SETTINGS_STATE");
		reg.write_flag = SETTINGS_STATE;
		ui.changeTexture(&reg.settings_active_texture, &reg.chats_texture, &reg.contacts_texture);
	}
	if(ui.checkToClickSprite(window, &ui.chats_button)) {
		logl("CHAT_STATE");
		reg.write_flag = CHAT_STATE;
		ui.changeTexture(&reg.contacts_texture, &reg.chats_active_texture, &reg.contacts_texture);
	}
	if(ui.checkToClickSprite(window, &ui.contacts_button)) {
		logl("CONTACTS_STATE");
		reg.write_flag = CONTACTS_STATE;
		ui.changeTexture(&reg.contacts_texture, &reg.chats_texture, &reg.contacts_active_texture);
	}
	
// Menu or Add chat button
	if(reg.menu_flag || reg.add_chat_flag)
		ui.checkToClickMenuButtons(window);

	if(ui.checkToClickSprite(window, &ui.add_button)) {
		logl("add_chat_button");
		reg.add_chat_flag++;
	}
	else
		reg.add_chat_flag = 0;

	if(ui.checkToClickSprite(window, &ui.menu_button)) {
		logl("menu_button");
		reg.menu_flag++;
	}
	else
		reg.menu_flag = 0;
}

void GameModel::textInput(Event *event) {
	if (event->text.unicode >= 32 && event->text.unicode <= 126) {
		// logl("Character typed: " + static_cast<char>(event.text.unicode));

		reg.stringsLexemes = static_cast<int>(reg.message.getSize() * text.getCharacterSize() / 2) % static_cast<int>(input_rect.getGlobalBounds().width);

		if(reg.stringsLexemes == 1 || reg.stringsLexemes == 2 || reg.stringsLexemes == 3) {
			text.setPosition(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top);
			reg.message.insert(reg.message.getSize(), "\n");
			reg.stringsCount++;
		}

		if(reg.stringsCount < 3) {
			reg.message.insert(reg.message.getSize(), event->text.unicode);
			text.setString(reg.message);
		}
	}
}

void GameModel::KeyPressedEnter() {
	if (reg.message.getSize() > 0) {
		// if(history != NULL)
		//     fprintf(history, "%s: %s\n", settings_struct.username, reg.message.getData());
		
		logl("reg.message: " + reg.message.toAnsiString());
		reg.message.insert(reg.message.getSize(), " ");
		reg.sendPacket << reg.message;

		sendMessage();
		// if (socket.send(sendPacket) == Socket::Done) {
  		// 	time = date_time[3]; 					// set time
		// 	setDate(&systemTime, date_time, &date); // set date
		// 	logl(date);

		// 	reg.message.insert(reg.message.getSize(), time);
		// 	drawUI.createMessageRect(&thread_struct.font, &thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, reg.message.toAnsiString().c_str(), 1);
		// 	sendPacket.clear();
		// 	reg.message.clear();
		// 	text.setString(reg.message);
		// }
		// else
		// 	die_With_Error(DEVICE, "Failed to send a reg.message to server!");
	}
}

void GameModel::KeyPressedBackspace() {
	if(reg.message.getSize() > 0) {
		reg.message.erase(reg.message.getSize() - 1);
		text.setString(reg.message);
	}
}

