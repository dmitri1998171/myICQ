#include "header.hpp"

#define DEVICE "CLIENT"

Mutex mutex;

struct GraphicsThread {
    Font font;
    TcpSocket* socket;
    RenderWindow* window;
    float output_rect_pos;
}graph_thread_struct;

struct networkStruct {
    String message;
    struct Settings settings_struct;
}net_struct;

void output_thread_func();

RectangleShape create_backround(float height, float y, Color color) {
    RectangleShape rect(Vector2f(WIDTH, height));
    rect.setPosition(Vector2f(0, y));
    rect.setFillColor(color);
    return rect;
}

void text_params_func(Font* font, Text* text, String message, float y) {
    font->loadFromFile("./media/fonts/CyrilicOld.TTF");
   
    message = "Enter a message...";
    text->setFont(*font);
    text->setCharacterSize(14);
    text->setString(message);
    text->setFillColor(Color(128, 128, 128, 100));
    text->setPosition(Vector2f(10, y));
}

int main() {
    struct Settings settings_struct;
    int write_flag = 0;
    float output_rect_pos = HEIGHT * 0.8;
    float input_rect_pos = HEIGHT - output_rect_pos;
    char title[100] = {"myICQ    Username: "};
    
    json_parser_create(&settings_struct);

    strcat(title, settings_struct.username);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

    /* Draw background */
    RectangleShape output_rect = create_backround(output_rect_pos, 0, Color(27, 28, 37));
    RectangleShape input_rect = create_backround(input_rect_pos, output_rect_pos, Color(39, 40, 49));
    
    /* Fonts and texts */
    Font font;
    Text text;
    String message;
    text_params_func(&font, &text, message, output_rect_pos + 5);
    
    /* Network */
    TcpSocket socket;
    network_func(&socket);
    
    /* Create a thread */
    // window.setActive(false);
    graph_thread_struct.font = font;
    graph_thread_struct.socket = &socket;
    graph_thread_struct.window = &window;
    graph_thread_struct.output_rect_pos = output_rect_pos;

    Thread thread(&output_thread_func);
    thread.launch();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) 
                window.close();

            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i localPosition = Mouse::getPosition(window);
                if(localPosition.y > output_rect_pos) {
                    write_flag = 1;
                    message = "";
                    text.setString(message);
                    text.setFillColor(Color::Black);
                }
                else {
                    write_flag = 0;
                    text.setFillColor(Color(128, 128, 128, 100));
                    if(message.getSize() == 0) {
                        message = "Enter a message...";
                        text.setString(message);
                    }
                }
            }

            if(write_flag == 1) {   // Поле ввода активировано
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode < 128) {
                        cout << "Character typed: " << static_cast<char>(event.text.unicode) << endl;
                        message.insert(message.getSize(), event.text.unicode);
                        text.setString(message);
                    }
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Enter) {
                        net_struct.message = message;
                        if (socket.send(&net_struct, sizeof(net_struct)) != Socket::Done)
                            die_With_Error(DEVICE, "Failed to send a message to server!");
                        message.clear();
                        text.setString(message);
                    } 
                }
            }
        }  
        
        window.clear();
        window.draw(output_rect);
        window.draw(input_rect);
        window.draw(text);
        window.display();
    }
    return 0;
}

void output_thread_func() {
    size_t received;

    // graph_thread_struct.window->setActive(true);
    while (graph_thread_struct.window->isOpen()) {
        if (graph_thread_struct.socket->receive(&net_struct, sizeof(net_struct), received) != Socket::Done)
            die_With_Error(DEVICE, "Failed to receive a message from the server!");

        // Text recv_text;
        // recv_text.setFont(graph_thread_struct.font);

        cout << "Received: \"" << net_struct.message.toAnsiString() << "\" (" << received << " bytes)" << endl;
        sleep(milliseconds(10));
    }
}