#include "GameRender.hpp"

GameRender::GameRender(GameModel *game)
{
	m_game = game;
	Init();
}

GameRender::~GameRender() {}

void GameRender::Init()
{
	m_window.create(sf::VideoMode(WIDTH, HEIGHT), "myICQ");
	// m_window.setFramerateLimit(60);
}

void GameRender::Render()
{
	m_window.clear();
	ui.drawBackground(&m_window, &m_game->background, &m_game->output_rect, &m_game->input_rect, &m_game->side_rect, &m_game->line);
	ui.drawGrandButtons(&m_window, &m_game->settings_text, &m_game->chats_text, &m_game->contacts_text);

	if(m_game->reg.menu_flag) {
		ui.drawMenuRects(&m_window);
		ui.drawMenu(&m_window, &m_game->add_contact_text, &m_game->add_group_text, &m_game->add_channel_text, &m_game->read_all_text);
	}

	if(m_game->reg.add_chat_flag) {
		int button_y = 120;
		ui.changeMenuPosition(button_y, &m_game->add_contact_text, &m_game->add_group_text, &m_game->add_channel_text, &m_game->read_all_text);
		ui.drawMenu(&m_window, &m_game->add_contact_text, &m_game->add_group_text, &m_game->add_channel_text, &m_game->read_all_text);
		
		m_window.draw(m_game->search);
		m_window.draw(m_game->search_text);
	}

	switch(reg.state) {
		case CHAT_STATE:
			m_window.draw(m_game->input_rect);
			m_window.draw(m_game->text);

			// for (int i = 0; i < dialog_count; i++) {
			// 	m_window.draw(m_game->thread_struct.output_text_rect[i]);
			// 	m_window.draw(m_game->thread_struct.recv_text[i]);
			// }
			break;

		case SETTINGS_STATE:
			break;
		
		case CONTACTS_STATE:
			break;
	}
	m_window.display();
}

void GameRender::initUI() {
// Background 
    ui.addBackground(&m_game->background, &m_game->output_rect, &m_game->input_rect, &m_game->side_rect, &m_game->line, &m_game->search);

    drawUI.addText(&reg.font, &m_game->search_text, "m_game->search", reg.line_color, 35, reg.input_rect_pos + drawUI.getCenter_y(m_game->search, m_game->search_text));
    drawUI.addText(&reg.font, &m_game->text, reg.message, reg.line_color, m_game->input_rect.getGlobalBounds().left + 10, m_game->input_rect.getGlobalBounds().top + (m_game->input_rect.getGlobalBounds().height / 4));

// Buttons
    drawUI.addButton(&ui.contacts_button, &reg.contacts_texture, reg.line_color, &m_game->settings_text, &reg.font, "./media/icons/contacts.png", "contacts", 1);
    drawUI.addButton(&ui.chats_button, &reg.chats_active_texture, reg.line_color, &m_game->chats_text, &reg.font, "./media/icons/active/chat_active.png", "chats", 2);
    drawUI.addButton(&ui.settings_button, &reg.settings_texture, reg.line_color, &m_game->contacts_text, &reg.font, "./media/icons/settings.png", "settings", 3);

// Menu
    ui.addMenu(&m_game->add_contact_text, &m_game->add_group_text, &m_game->add_channel_text, &m_game->read_all_text);
    
    drawUI.addMenuButton(&ui.menu_add_contact, &reg.add_contact_texture, &ui.menu_add_contact_sprite, reg.sidebar_color, &m_game->add_contact_text, &reg.font, "./media/icons/menu/menu_add_contact.png", "Add contact", 0);
    drawUI.addMenuButton(&ui.menu_add_group, &reg.menu_add_group_texture, &ui.menu_add_group_sprite, reg.sidebar_color, &m_game->add_group_text, &reg.font, "./media/icons/menu/menu_add_group.png", "Add group", 1);
    drawUI.addMenuButton(&ui.menu_add_channel, &reg.menu_add_channel_texture, &ui.menu_add_channel_sprite, reg.sidebar_color, &m_game->add_channel_text, &reg.font, "./media/icons/menu/menu_add_channel.png", "Add channel", 2);
    drawUI.addMenuButton(&ui.menu_read_all, &reg.menu_read_all_texture, &ui.menu_read_all_sprite, reg.sidebar_color, &m_game->read_all_text, &reg.font, "./media/icons/menu/menu_read_all.png", "Read all", 3);

}
