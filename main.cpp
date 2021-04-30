#include <iostream>
#include <cstdio>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/document.h"
#include <rapidjson/filereadstream.h>
 
#define WIDTH 640 
#define HEIGHT 480
#define OUTLINE_WIDTH 5

using namespace std;
using namespace sf;
using namespace rapidjson;

Mutex mutex;

struct ThreadMain {
    RenderWindow* window;
    Font font;
    float output_rect_pos;
};

void outputFunc(struct ThreadMain thread_struct) {
    
    Text text("Enter a message...", thread_struct.font, 14);
    text.setFillColor(Color(128, 128, 128, 100));
    text.setStyle(Text::Bold | Text::Underlined);
    text.setPosition(Vector2f(10, thread_struct.output_rect_pos));

    // thread_struct.window->setActive(true);
    while (thread_struct.window->isOpen()) {

        // mutex.lock();
        // mutex.unlock();
        
        sleep(milliseconds(10));
    }
}

int main() {
    ThreadMain thread_struct;

    FILE* pFile = fopen("settings.json", "rb");
    char buffer[65536];
    FileReadStream is(pFile, buffer, sizeof(buffer));
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(is);

    assert(document["username"].IsString());
    printf("username = %s\n", document["username"].GetString());

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "myICQ");
    thread_struct.window = &window;

    float output_rect_pos = HEIGHT * 0.8;
    float input_rect_pos = HEIGHT - output_rect_pos;
    thread_struct.output_rect_pos = output_rect_pos;

    /* Draw background */
    RectangleShape output_rect(Vector2f(WIDTH - 10, output_rect_pos));
    output_rect.setPosition(Vector2f(OUTLINE_WIDTH, OUTLINE_WIDTH));
    output_rect.setFillColor(Color(150, 150, 150));
    output_rect.setOutlineThickness(OUTLINE_WIDTH);
    output_rect.setOutlineColor(Color(250, 150, 100));

    RectangleShape input_rect(Vector2f(WIDTH - 10, input_rect_pos - 5));
    input_rect.setPosition(Vector2f(OUTLINE_WIDTH, output_rect_pos));
    input_rect.setFillColor(Color::White);
    input_rect.setOutlineThickness(OUTLINE_WIDTH);
    input_rect.setOutlineColor(Color(250, 150, 100));
    
    /* Fonts and texts */
    Font font;
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    thread_struct.font = font;
    Text text("Enter a message...", font, 14);
    text.setFillColor(Color(128, 128, 128, 100));
    text.setStyle(Text::Bold | Text::Underlined);

    text.setPosition(Vector2f(10, output_rect_pos));

    /* Create thread */
    // window.setActive(false);
    Thread thread(&outputFunc, thread_struct);
    thread.launch();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) 
                window.close();
            if (event.type == Event::KeyPressed) {
                mutex.lock();

                mutex.unlock();
            }
        }  
        
        window.clear();
        window.draw(output_rect);
        window.draw(input_rect);
        window.draw(text);
        window.display();
        // thread.wait();
    }

    return 0;
}