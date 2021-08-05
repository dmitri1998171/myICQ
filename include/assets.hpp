#include "header.hpp"

#define DEVICE "CLIENT"

class Assets {
    private:

        Assets() {}

    public:
        static Assets* instance;
        static Assets* getInstance() {
            if (instance == 0)
                instance = new Assets();

            return instance;
        }

        void texture_loader(Texture* texture, string path) {
            if (!texture->loadFromFile(path)) 
                die_With_Error(DEVICE, "Can't load UI icons!");
                texture->setSmooth(true);
        }

        void sprite_loader(Sprite* sprite, Texture* texture, string path, float x, float y) {
            texture_loader(texture, path);
            sprite->setTexture(*texture);
            sprite->setPosition(x, y);
        }
};