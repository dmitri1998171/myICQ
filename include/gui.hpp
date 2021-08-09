#include "draw.hpp"

class IGUI : IDrawUI {
    public:
        // IDrawUI gui;
        Registry reg;
        
        RectangleShape menu, menu_add_contact, menu_add_group, 
        menu_add_channel, menu_read_all, menu_line;

        Sprite settings_button, chats_button, contacts_button,
        add_button, menu_button;

        Sprite menu_add_contact_sprite, menu_add_group_sprite,
        menu_add_channel_sprite, menu_read_all_sprite;

        void addBackground(RectangleShape*, RectangleShape*, RectangleShape*, RectangleShape*);
        void addMenu(Text*, Text*, Text*, Text*);
        void addUI(RectangleShape*, RectangleShape*, RectangleShape*);
        void changeMenuPosition(int, Text*, Text*, Text*, Text*);
        void drawBackground(RenderWindow*, RectangleShape*, RectangleShape*, RectangleShape*, RectangleShape*, RectangleShape*);
        void drawGrandButtons(RenderWindow*, Text*, Text*, Text*);
        void drawMenuRects(RenderWindow*);
        void drawMenu(RenderWindow*, Text*, Text*, Text*, Text*);
};