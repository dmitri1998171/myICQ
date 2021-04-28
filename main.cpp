#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#define WIDTH 640 
#define HEIGHT 480

#define OUTLINE_WIDTH 5

Mutex mutex;

void outputFunc(RenderWindow* window) {
    

    // window->setActive(true);
    // while (window->isOpen()) {

    //     // mutex.lock();
    //     // mutex.unlock();
        
    //     sleep(milliseconds(10));
    // }
}

int main() {
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "myICQ");

    float input_rect_pos = HEIGHT * 0.7;

    /* Draw background */
    RectangleShape output_rect(Vector2f(WIDTH - 10, input_rect_pos));
    output_rect.setPosition(Vector2f(OUTLINE_WIDTH, OUTLINE_WIDTH));
    output_rect.setFillColor(Color(150, 150, 150));
    output_rect.setOutlineThickness(OUTLINE_WIDTH);
    output_rect.setOutlineColor(Color(250, 150, 100));

    RectangleShape input_rect(Vector2f(WIDTH - 10, HEIGHT * 0.3 - 5));
    input_rect.setPosition(Vector2f(OUTLINE_WIDTH, input_rect_pos));
    input_rect.setFillColor(Color::White);
    input_rect.setOutlineThickness(OUTLINE_WIDTH);
    input_rect.setOutlineColor(Color(250, 150, 100));
    

    /* Fonts and texts */
    Font font;
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    Text text("Enter a message...", font, 24);
    text.setFillColor(Color(128, 128, 128, 100));
    text.setStyle(Text::Bold | Text::Underlined);

    text.setPosition(Vector2f(10, input_rect_pos));






    /* Create thread */
    // window.setActive(false);
    // Thread thread(&outputFunc, &window);
    // thread.launch();

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