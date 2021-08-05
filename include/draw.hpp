#include "assets.hpp"

class IDrawUI {
    public:
        int sidebar_width = 400;
        int menu_x = 45, menu_y = 5, menu_w = 160, menu_h = 200;
        int menu_button_h = (menu_h / 4);

        Registry reg;
        Assets* assets = Assets::getInstance();

        void createRect(RectangleShape *rect, Color color, float width, float height, float x, float y);
        void addText(Font* font, Text *text, String message, Color color, float x, float y);
        void createMessageRect(Font** font, RectangleShape** output_rect, RectangleShape* output_text_rect, Text* recv_text, const char *str, int pos);
        void createCircleAngleRect(RectangleShape *rect, Color color, int width, int height, int x, int y);
        void addButton(Sprite* sprite, Texture* texture, Color color, Text* text, Font* font, string path, String str, int sprite_pos);
        void addMenuButton(RectangleShape* rect, Texture* texture, Sprite* sprite, Color color, Text* text, Font* font, string path, String str, int text_pos);
        int getCenter_x(Sprite img, Text text);
        int getCenter_y(RectangleShape img, Text text);
};