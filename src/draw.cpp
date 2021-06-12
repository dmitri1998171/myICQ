#include "header.hpp"

#define DEVICE "CLIENT"

void create_rect(RectangleShape *rect, Color color, float width, float height, float x, float y) {
    rect->setSize(Vector2f(width, height));
    rect->setPosition(Vector2f(x, y));
    rect->setFillColor(Color(color));
}

void text_params_func(Font* font, Text *text, String message, Color color, float x, float y) {
    text->setFont(*font);
    text->setCharacterSize(font_size);
    text->setString(message);
    text->setFillColor(Color(color));
    text->setPosition(Vector2f(x, y));
}

void draw_message_rect(Font** font, RectangleShape** output_rect, RectangleShape** output_text_rect, Text** recv_text, const char *str, int pos) {
    int padding_x = 7, margin_x = 0, margin_y = 5; 
    float output_rect_x, output_rect_y, output_rect_w;
    RectangleShape* pre_output_rect = *output_rect;
    RectangleShape* pre_output_text_rect = *output_text_rect;
    Text* pre_recv_text = *recv_text;

    output_rect_y = pre_output_rect->getGlobalBounds().top;
    output_rect_w = (strlen(str) * font_size) * 0.5;

    if(pos) 
        output_rect_x = (pre_output_rect->getSize().x + sidebar_width) - output_rect_w - margin_x;
    else 
        output_rect_x = pre_output_rect->getGlobalBounds().left + margin_x;
    
    create_rect(&pre_output_text_rect[dialog_count], Color(34, 52, 79), output_rect_w, font_size + margin_y, output_rect_x, output_rect_y + sep);
    text_params_func(*font, &pre_recv_text[dialog_count], str, Color::White, output_rect_x + padding_x, output_rect_y + sep);
    
    sep += 30;
    dialog_count++;
}

Sprite UI_shedule(string path, int x, int y) {
    Texture texture;
    if (!texture.loadFromFile(path)) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    
    texture.setSmooth(true);

    Sprite sprite(texture);
    sprite.setPosition(x, y);
    return sprite;
}
