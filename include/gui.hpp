#include "header.hpp"

class IGUI {
    public:
        IDrawUI gui;
        Registry reg;
        
        void drawBackground(RectangleShape*, RectangleShape*, RectangleShape*, RectangleShape*);
        void drawUI(RectangleShape*, RectangleShape*);
        void loadFontAndText(RectangleShape*);
};