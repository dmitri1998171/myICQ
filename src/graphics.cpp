#include "header.hpp"

#define DEVICE "CLIENT"

Mutex mutex;

struct GraphicsThread {
    Font font;
    TcpSocket* socket;
    RenderWindow* window;
    float output_rect_pos;
}graph_thread_struct;

void outputFunc(struct GraphicsThread graph_thread_struct) {
    size_t received;
    String data;

    // graph_thread_struct.window->setActive(true);
    while (graph_thread_struct.window->isOpen()) {
        if (graph_thread_struct.socket->receive(&data, sizeof(data), received) != Socket::Done)
            die_With_Error(DEVICE, "Failed to receive a message from the server!");

        Text recv_text;
        recv_text.setFont(graph_thread_struct.font);

        cout << "Received: \"" << data.toAnsiString() << "\" (" << received << " bytes)" << endl;
        sleep(milliseconds(10));
    }
}

void SFML_GUI(struct Settings settings_struct) {
    int write_flag = 0;
    float output_rect_pos = HEIGHT * 0.8;
    float input_rect_pos = HEIGHT - output_rect_pos;
    char title[100] = {"myICQ    Username: "};

    strcat(title, settings_struct.username);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

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
    
    String message = "Enter a message...";
    Text text(message, font, 14);
    text.setFillColor(Color(128, 128, 128, 100));
    text.setPosition(Vector2f(10, output_rect_pos));

    /* Network */
    TcpSocket socket;
    IpAddress server_ip = "127.0.0.1";

    Socket::Status status = socket.connect(server_ip, PORT);
    if (status != Socket::Done)
        die_With_Error(DEVICE, "Failed to connect to server!");

    /* Create a thread */
    // window.setActive(false);
    graph_thread_struct.font = font;
    graph_thread_struct.socket = &socket;
    graph_thread_struct.window = &window;
    graph_thread_struct.output_rect_pos = output_rect_pos;

    Thread thread(&outputFunc, graph_thread_struct);
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
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode < 128) {
                        cout << "Character typed: " << static_cast<char>(event.text.unicode) << endl;
                        message.insert(message.getSize(), event.text.unicode);
                        text.setString(message);
                    }
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (socket.send(&message, sizeof(message)) != Socket::Done)
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
}