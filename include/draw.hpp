#include "assets.hpp"

class IDrawUI {
    public:
        int sidebar_width = 400;
        int menu_x = 45, menu_y = 5, menu_w = 160, menu_h = 200;
        int menu_button_h = (menu_h / 4);

        Registry reg;
        Assets* assets = Assets::getInstance();

        void addText(Font*, Text*, String, Color, float, float);
        void addButton(Sprite*, Texture*, Color, Text*, Font*, string, String, int);
        void addMenuButton(RectangleShape*, Texture*, Sprite*, Color, Text*, Font*, string, String, int);
        void createRect(RectangleShape*, Color, float, float, float, float);
        void createMessageRect(Font**, RectangleShape**, RectangleShape*, Text*, const char*, int);
        void createCircleAngleRect(RectangleShape*, Color, int, int, int, int);
        int getCenter_x(Sprite, Text);
        int getCenter_y(RectangleShape, Text);
};