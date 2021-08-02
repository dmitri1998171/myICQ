#include "header.hpp"
#include "draw.hpp"

#define DEVICE "CLIENT"

void IDrawUI::createRect(RectangleShape *rect, Color color, float width, float height, float x, float y) {
    rect->setSize(Vector2f(width, height));
    rect->setPosition(Vector2f(x, y));
    rect->setFillColor(Color(color));
}

void IDrawUI::addText(Font* font, Text *text, String message, Color color, float x, float y) {
    text->setFont(*font);
    text->setCharacterSize(font_size);
    text->setString(message);
    text->setFillColor(Color(color));
    text->setPosition(Vector2f(x, y));
}

void IDrawUI::createMessageRect(Font** font, RectangleShape** output_rect, RectangleShape* output_text_rect, Text* recv_text, const char *str, int pos) {
    int padding_x = 7, margin_x = 0, margin_y = 5; 
    float output_rect_x, output_rect_y, output_rect_w;
    RectangleShape* pre_output_rect = *output_rect;

    output_rect_y = pre_output_rect->getGlobalBounds().top;
    output_rect_w = (strlen(str) * font_size) * 0.5;

    if(pos) 
        output_rect_x = (pre_output_rect->getSize().x + sidebar_width) - output_rect_w - margin_x;
    else 
        output_rect_x = pre_output_rect->getGlobalBounds().left + margin_x;
    
    createRect(&output_text_rect[dialog_count], Color(34, 52, 79), output_rect_w, font_size + margin_y, output_rect_x, output_rect_y + sep);
    addText(*font, &recv_text[dialog_count], str, Color::White, output_rect_x + padding_x, output_rect_y + sep);
    
    sep += 30;
    dialog_count++;
}

void IDrawUI::texture_loader(Texture* texture, string path) {
    if (!texture->loadFromFile(path)) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    texture->setSmooth(true);
}

void IDrawUI::sprite_loader(Sprite* sprite, Texture* texture, string path, float x, float y) {
    texture_loader(texture, path);
    sprite->setTexture(*texture);
    sprite->setPosition(x, y);
}

void IDrawUI::createCircleAngleRect(RectangleShape *rect, Color color, int width, int height, int x, int y) {
    int radius = height / 2;

    createRect(rect, color, width, height, x, y);
    
    CircleShape left_circle(radius), right_circle(radius);
    left_circle.setPosition(x, y + radius);
    right_circle.setPosition(x + width, y + radius);
}

void IDrawUI::addButton(Sprite* sprite, Texture* texture, Color color, Text* text, Font* font, string path, String str, int sprite_pos) {
    sprite_loader(sprite, texture, path,  ((sidebar_width / 4) * sprite_pos) - (sprite->getGlobalBounds().width / 2), HEIGHT - 55 );
    addText(font, text, str, Color::White, getCenter_x(*sprite, *text), sprite->getGlobalBounds().top + sprite->getGlobalBounds().height);
}

void IDrawUI::addMenuButton(RectangleShape* rect, Texture* texture, Sprite* sprite, Color color, Text* text, Font* font, string path, String str, int text_pos) {
    createRect(rect, color, menu_w, menu_button_h, menu_x, menu_y + (menu_button_h * text_pos));
    sprite_loader(sprite, texture, path, menu_x + 15, (rect->getPosition().y + 10));
    addText(font, text, str, Color::White, menu_button_h + 64, getCenter_y(*rect, *text) + (menu_button_h * text_pos));
}

int IDrawUI::getCenter_x(Sprite img, Text text) {
    return img.getGlobalBounds().left - (img.getGlobalBounds().width - text.getGlobalBounds().width) / 2;
    // return ((text.getGlobalBounds().width - img.getGlobalBounds().width) / 2);// - (text.getString().getSize() / 2);
}

int IDrawUI::getCenter_y(RectangleShape img, Text text) {
    return ((img.getGlobalBounds().height - text.getGlobalBounds().height) / 2) - (font_size / 2);
}

