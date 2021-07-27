#include "header.hpp"

class IGUI {
    public:
        void createRect(RectangleShape *rect, Color color, float width, float height, float x, float y);
        void addText(Font* font, Text *text, String message, Color color, float x, float y);
        void createMessageRect(Font** font, RectangleShape** output_rect, RectangleShape* output_text_rect, Text* recv_text, const char *str, int pos);
        void texture_loader(Texture* texture, string path);
        void sprite_loader(Sprite* sprite, Texture* texture, string path, float x, float y);
        void createCircleAngleRect(RectangleShape *rect, Color color, int width, int height, int x, int y);
        void addButton(Sprite* sprite, Texture* texture, Color color, Text* text, Font* font, string path, String str, int sprite_pos);
        void addMenuButton(RectangleShape* rect, Texture* texture, Sprite* sprite, Color color, Text* text, Font* font, string path, String str, int text_pos);
        int getCenter(Sprite img, Text text);
        int getCenter_y(RectangleShape img, Text text);
};