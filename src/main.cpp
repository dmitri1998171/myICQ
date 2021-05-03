#include "header.hpp"

#define DEVICE "CLIENT"

Mutex mutex;

struct GraphicsThread {
    Font* font;
    Text* recv_text;
    TcpSocket* socket;
    RenderWindow* window;
    RectangleShape* output_rect;
    RectangleShape* output_text_rect;
    float output_rect_pos;
}thread_struct;

struct networkStruct {
    String message;
    struct Settings settings_struct;
}net_struct;

void output_thread_func();

void create_rect(RectangleShape *rect, float width, float height, float x, float y) {
    rect->setSize(Vector2f(width, height));
    rect->setPosition(Vector2f(x, y));
}

void text_params_func(Font* font, Text* text, String message, Color color, float x, float y) {
    text->setFont(*font);
    text->setCharacterSize(14);
    text->setString(message);
    text->setFillColor(Color(color));
    text->setPosition(Vector2f(x, y));
}

int main() {
    struct Settings settings_struct;
    int write_flag = 0;
    float output_rect_pos = HEIGHT * 0.8;
    float input_rect_pos = HEIGHT - output_rect_pos;
    float sidebar_width = 400;
    char title[100] = {"myICQ    Name: "};

    json_parser_create(&settings_struct);

    strcat(title, settings_struct.username);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

    /* Draw background */
    RectangleShape output_rect;
    RectangleShape input_rect;
    RectangleShape side_rect;
    RectangleShape output_text_rect;

    create_rect(&output_rect, WIDTH - sidebar_width, HEIGHT, sidebar_width, 0);
    create_rect(&input_rect, (WIDTH - sidebar_width) - 20, input_rect_pos - 20, sidebar_width + 10, output_rect_pos + 10);
    create_rect(&side_rect, sidebar_width, HEIGHT, 0, 0);
    output_rect.setFillColor(Color(27, 28, 37));
    input_rect.setFillColor(Color(39, 40, 49));
    side_rect.setFillColor(Color(39, 40, 49));

    /* Fonts and texts */
    String message = "Enter a message...";
    Text text;
    Text recv_text;
    Font font;
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    Color text_color(128, 128, 128, 100);
    text_params_func(&font, &text, message, text_color, input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5);
    
    /* Network */
    TcpSocket socket;
    network_func(&socket);
    
    /* Create a thread */
    // window.setActive(false);
    thread_struct.font = &font;
    thread_struct.recv_text = &recv_text;
    thread_struct.socket = &socket;
    thread_struct.window = &window;
    thread_struct.output_rect = &output_rect;
    thread_struct.output_text_rect = &output_text_rect;
    thread_struct.output_rect_pos = output_rect_pos;

    Thread thread(&output_thread_func);
    thread.launch();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) 
                window.close();
            
            if (event.type == Event::Resized) {
                int min_x = 470;
                int min_y = 600;
                int sub_min_x = 830;
                
                int winX = window.getSize().x;
                int winY = window.getSize().y;

                if(winX > min_x || winY > min_y) 
                    window.setSize(Vector2u(winX, winY)); 
                if(winX < min_x) 
                    window.setSize(Vector2u(min_x, winY)); 
                if(winY < min_y) 
                    window.setSize(Vector2u(winX, min_y)); 

                if(winX < sub_min_x) {
                    sidebar_width = 0;
                    output_rect_pos = HEIGHT * 0.8;
                    side_rect.setSize(Vector2f(0, 0));
                    create_rect(&output_rect, WIDTH, HEIGHT, 0, 0);
                    create_rect(&input_rect, WIDTH - 20, input_rect_pos - 20, 10, output_rect_pos + 10);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                }
                if(winX > sub_min_x) {
                    sidebar_width = 400;
                    output_rect_pos = HEIGHT * 0.8;
                    create_rect(&output_rect, WIDTH - sidebar_width, HEIGHT, sidebar_width, 0);
                    create_rect(&input_rect, (WIDTH - sidebar_width) - 20, input_rect_pos - 20, sidebar_width + 10, output_rect_pos + 10);
                    create_rect(&side_rect, sidebar_width, HEIGHT, 0, 0);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                }
            }

            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mouse_pos = Mouse::getPosition(window);
                if(mouse_pos.y > input_rect.getGlobalBounds().top && mouse_pos.x > input_rect.getGlobalBounds().left) {
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
        window.draw(side_rect);
        window.draw(output_rect);
        window.draw(input_rect);
        window.draw(output_text_rect);
        window.draw(text);
        window.draw(recv_text);
        window.display();
    }
    return 0;
}

void output_thread_func() {
    int sep = 10;
    float output_rect_x, output_rect_y, output_rect_w;
    float str_length = 50;
    float font_size = 14;
    float string_count = 1;
    size_t received;

    // thread_struct.window->setActive(true);
    while (thread_struct.window->isOpen()) {
        if (thread_struct.socket->receive(&net_struct, sizeof(net_struct), received) != Socket::Done)
            die_With_Error(DEVICE, "Failed to receive a message from the server!");

        cout << "Received: \"" << net_struct.message.toAnsiString() << "\" (" << received << " bytes)" << endl;

        output_rect_x = thread_struct.output_rect->getGlobalBounds().left + 10;
        output_rect_y = thread_struct.output_rect->getGlobalBounds().top;
        output_rect_w = font_size * string_count;

        create_rect(thread_struct.output_text_rect, str_length, output_rect_w + 5, output_rect_x, output_rect_y + sep);
        thread_struct.output_text_rect->setFillColor(Color(34, 52, 79));
        text_params_func(thread_struct.font, thread_struct.recv_text, net_struct.message, Color::Black, output_rect_x + 5, output_rect_y + sep);
        
        sep += output_rect_w + 10;
        sleep(milliseconds(10));
    }
}