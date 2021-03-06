#include "gui.hpp"

void IGUI::addBackground(RectangleShape* background, RectangleShape* output_rect, RectangleShape* input_rect, RectangleShape* side_rect, RectangleShape* line, RectangleShape* search) {
    createRect(background, reg.output_color, WIDTH, HEIGHT, 0, 0);
    createRect(output_rect, reg.output_color, (WIDTH - sidebar_width) - 20, reg.output_rect_pos - 10, sidebar_width + 10, 10);
    createCircleAngleRect(input_rect, reg.sidebar_color, (WIDTH - sidebar_width) - 20, reg.input_rect_pos, sidebar_width + 10, reg.output_rect_pos + 10);
    createRect(side_rect, reg.sidebar_color, sidebar_width, HEIGHT, 0, 0);
    
    createRect(line, reg.line_color, sidebar_width, 1, 0, HEIGHT - 65);
    createRect(search, reg.sidebar_color, sidebar_width - 30, reg.input_rect_pos, 15, 50);
    search->setOutlineThickness(0.5);
    search->setOutlineColor(reg.line_color);
}

void IGUI::addUI(RectangleShape* line, RectangleShape* search, RectangleShape* input_rect) {
    
    }

void IGUI::addMenu(Text* add_contact_text, Text* add_group_text, Text* add_channel_text, Text* read_all_text) {
    createRect(&menu, reg.sidebar_color, menu_w, menu_h, menu_x, menu_y);
    createRect(&menu_line, reg.line_color, menu_w, 1, menu_x, menu_button_h * 3);
    menu.setOutlineThickness(0.5);
    menu.setOutlineColor(reg.line_color);
}

void IGUI::changeMenuPosition(int button_y, Text* add_contact_text, Text* add_group_text, Text* add_channel_text, Text* read_all_text) {
    menu_add_contact.setPosition(15, button_y);
    menu_add_group.setPosition(15, button_y + menu_button_h);
    menu_add_channel.setPosition(15, button_y + (menu_button_h * 2));
    menu_read_all.setPosition(15, button_y + (menu_button_h * 3));

    add_contact_text->setPosition(60, button_y);
    add_group_text->setPosition(60, button_y + menu_button_h);
    add_channel_text->setPosition(60, button_y + (menu_button_h * 2));
    read_all_text->setPosition(60, button_y+ (menu_button_h * 3));

    menu_add_contact_sprite.setPosition(15, button_y);
    menu_add_group_sprite.setPosition(15, button_y + menu_button_h);
    menu_add_channel_sprite.setPosition(15, button_y + (menu_button_h * 2));
    menu_read_all_sprite.setPosition(15, button_y+ (menu_button_h * 3));
}

void IGUI::changeTexture(Texture* settings, Texture* chats, Texture* contacts) {
    settings_button.setTexture(*settings);
    chats_button.setTexture(*chats);
    contacts_button.setTexture(*contacts);
}

int IGUI::checkToClickSprite(RenderWindow* window, Sprite* button) {
    Vector2i mouse_pos = Mouse::getPosition(*window);
    // IntRect rect = button->getTextureRect();

    if(button->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
    // if(rect.contains(mouse_pos)) 
        return 1;
    else 
        return 0;
}

int IGUI::checkToClickRect(RenderWindow* window, RectangleShape* rect) {
    Vector2i mouse_pos = Mouse::getPosition(*window);

    if(rect->getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) 
        return 1;
    else 
        return 0;
}

void IGUI::checkToClickMenuButtons(RenderWindow* window) {
    if(checkToClickRect(window, &menu_add_contact)) {
        logl("add contact");
    }
    if(checkToClickRect(window, &menu_add_group)) {
        logl("add group");
    }
    if(checkToClickRect(window, &menu_add_channel)) {
        logl("add channel");
    }
    if(checkToClickRect(window, &menu_read_all)) {
        logl("read all");
    }
}

void IGUI::drawBackground(RenderWindow* window, RectangleShape* background, RectangleShape* output_rect, RectangleShape* input_rect, RectangleShape* side_rect, RectangleShape* line) {
    window->draw(*background);
    window->draw(*output_rect);
    window->draw(*side_rect);
    window->draw(*line);
}

void IGUI::drawGrandButtons(RenderWindow* window, Text* settings_text, Text* chats_text, Text* contacts_text) {
    window->draw(settings_button);
    window->draw(chats_button);
    window->draw(contacts_button);
    window->draw(add_button);
    window->draw(menu_button);

    window->draw(*settings_text);
    window->draw(*chats_text);
    window->draw(*contacts_text);
}

void IGUI::drawMenuRects(RenderWindow* window) {
    window->draw(menu);
    window->draw(menu_add_contact);
    window->draw(menu_add_group);
    window->draw(menu_add_channel);
    window->draw(menu_read_all);
    window->draw(menu_line);
}

void IGUI::drawMenu(RenderWindow* window, Text* add_contact_text, Text* add_group_text, Text* add_channel_text, Text* read_all_text) {
    window->draw(*add_contact_text);
    window->draw(*add_group_text);
    window->draw(*add_channel_text);
    window->draw(*read_all_text);

    window->draw(menu_add_contact_sprite);
    window->draw(menu_add_group_sprite);
    window->draw(menu_add_channel_sprite);
    window->draw(menu_read_all_sprite);
}
