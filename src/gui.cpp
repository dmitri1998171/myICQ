#include "header.hpp"
#include "draw.hpp"
#include "gui.hpp"

void IGUI::drawBackground(RectangleShape* background, RectangleShape* output_rect, RectangleShape* input_rect, RectangleShape* side_rect) {
    gui.createRect(background, reg.output_color, WIDTH, HEIGHT, 0, 0);
    gui.createRect(output_rect, reg.output_color, (WIDTH - sidebar_width) - 20, reg.output_rect_pos - 10, sidebar_width + 10, 10);
    gui.createCircleAngleRect(input_rect, reg.sidebar_color, (WIDTH - sidebar_width) - 20, reg.input_rect_pos, sidebar_width + 10, reg.output_rect_pos + 10);
    gui.createRect(side_rect, reg.sidebar_color, sidebar_width, HEIGHT, 0, 0);
}

void IGUI::drawUI(RectangleShape* line, RectangleShape* search) {
    gui.createRect(line, reg.line_color, sidebar_width, 1, 0, HEIGHT - 65);
    gui.createRect(search, reg.sidebar_color, sidebar_width - 30, reg.input_rect_pos, 15, 50);
    search->setOutlineThickness(0.5);
    search->setOutlineColor(reg.line_color);
    gui.addText(&reg.font, &reg.search_text, "serach", reg.line_color, 35, reg.input_rect_pos + gui.getCenter_y(*search, reg.search_text));
}

void IGUI::loadFontAndText(RectangleShape* inputRect) {
    reg.font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    gui.addText(&reg.font, &reg.text, reg.message, reg.line_color, inputRect->getGlobalBounds().left + 10, inputRect->getGlobalBounds().top + (inputRect->getGlobalBounds().height / 4));
}