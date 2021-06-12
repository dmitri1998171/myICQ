#include "header.hpp"
#include "vars.hpp"

#define DEVICE "CLIENT"
#define STR_SIZE 256
Mutex mutex;

struct GraphicsThread {
    Font* font;
    TcpSocket* socket;
    RenderWindow* window;
    RectangleShape* output_rect;
    RectangleShape output_text_rect[STR_SIZE];
    Text recv_text[STR_SIZE];
    float output_rect_pos;
}thread_struct;

void output_thread_func();

int sidebar_width;
int sep, dialog_count;
float font_size;

int main() {
    int winX, winY;
    int write_flag = 0;
    int min_x = 470, min_y = 600, sub_min_x = 830;
    float output_rect_pos = HEIGHT * 0.9;
    float input_rect_pos = HEIGHT - output_rect_pos;
    char title[100] = {"myICQ    Name: "};
    struct Settings settings_struct;
    Packet sendPacket;
    
    json_parser_create(&settings_struct);
    
    sidebar_width = 400;
    font_size = settings_struct.font_size;

    strcat(title, settings_struct.username);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

    /* Draw background */
    RectangleShape background, output_rect, input_rect, side_rect;

    create_rect(&background, Color(27, 28, 37), WIDTH, HEIGHT, 0, 0);
    create_rect(&output_rect, Color(27, 28, 37), (WIDTH - sidebar_width) - 20, output_rect_pos - 10, sidebar_width + 10, 10);
    create_rect(&input_rect, Color(39, 40, 49), (WIDTH - sidebar_width) - 20, input_rect_pos - 20, sidebar_width + 10, output_rect_pos + 10);
    create_rect(&side_rect, Color(39, 40, 49), sidebar_width, HEIGHT, 0, 0);

    /* Fonts and texts */
    Text text;
    Font font;
    String message = "Enter a message...";
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    Color text_color(128, 128, 128, 100);
    text_params_func(&font, &text, message, text_color, input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + (input_rect.getGlobalBounds().height / 4));
    
    /*UI*/ 
    RectangleShape line(Vector2f(sidebar_width, 1.f));
    line.setPosition(0, HEIGHT - 65);

    Texture contacts_texture;
    if (!contacts_texture.loadFromFile("./media/icons/contacts.png")) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    contacts_texture.setSmooth(true);
    
    Texture chats_texture;
    if (!chats_texture.loadFromFile("./media/icons/chats.png")) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    chats_texture.setSmooth(true);

    Texture settings_texture;
    if (!settings_texture.loadFromFile("./media/icons/settings.png")) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    settings_texture.setSmooth(true);

    Texture add_texture;
    if (!add_texture.loadFromFile("./media/icons/add_chat.png")) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    add_texture.setSmooth(true);

    Texture menu_texture;
    if (!menu_texture.loadFromFile("./media/icons/menu.png")) 
        die_With_Error(DEVICE, "Can't load UI icons!");
    menu_texture.setSmooth(true);


    Sprite contacts_button(contacts_texture);
    contacts_button.setPosition((sidebar_width / 4) - (contacts_button.getGlobalBounds().width / 2), HEIGHT - 55);
    Text contacts_text;
    text_params_func(&font, &contacts_text, "contacts", text_color, getCenter(contacts_button, contacts_text), HEIGHT - 25);

    Sprite chats_button(chats_texture);
    chats_button.setPosition(((sidebar_width / 4) * 2) - (chats_button.getGlobalBounds().width / 2), HEIGHT - 55);
    Text chats_text;
    text_params_func(&font, &chats_text, "chats", text_color, chats_button.getGlobalBounds().left - 5, HEIGHT - 25);

    Sprite settings_button(settings_texture);
    settings_button.setPosition(((sidebar_width / 4) * 3) - (settings_button.getGlobalBounds().width / 2), HEIGHT - 55);
    Text settings_text;
    text_params_func(&font, &settings_text, "settings", text_color, getCenter(settings_button, settings_text), HEIGHT - 25);

    Sprite add_button(add_texture);
    add_button.setPosition(sidebar_width - add_button.getGlobalBounds().width, 5);

    Sprite menu_button(menu_texture);
    menu_button.setPosition(0, 5);

    // Sprite settings_ui = UI_shedule("./media/icons/settings.png", sidebar_width - 50, HEIGHT - 55);

    /* Network */
    TcpSocket socket;
    network_func(&socket);
    
    /* Create the thread */
    thread_struct.font = &font;
    thread_struct.socket = &socket;
    thread_struct.window = &window;
    thread_struct.output_rect = &output_rect;
    thread_struct.output_rect_pos = output_rect_pos;
    
    Thread thread(&output_thread_func);
    thread.launch();
    
    /* Dialog history */ 
    FILE* history;
    history_dialog(&history, &font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
    
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
                exit(0);
            }
            
            if (event.type == Event::Resized) {
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));

                winX = window.getSize().x;
                winY = window.getSize().y;
                
        // 2
                if(min_x < winX < sub_min_x) {
                    output_rect_pos = HEIGHT * 0.8;
                    sidebar_width = 0;
                    
                    side_rect.setSize(Vector2f(0, 0));
                    create_rect(&output_rect, Color(27, 28, 37), winX - 20, output_rect_pos - 10, 10, 10);
                    create_rect(&input_rect, Color(39, 40, 49), winX - 20, input_rect_pos - 20, 10, output_rect_pos + 10);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                    history_dialog(&history, &font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
                }

        // 1
                if(winX > sub_min_x) {
                    sidebar_width = 400;
                    output_rect_pos = winY - input_rect_pos;

                    create_rect(&background, Color(27, 28, 37), winX, winY, 0, 0);
                    create_rect(&output_rect, Color(27, 28, 37), winX - sidebar_width - 20, output_rect_pos - 10, sidebar_width + 10, 10);
                    create_rect(&input_rect, Color(39, 40, 49), (winX - sidebar_width) - 20, input_rect_pos - 20, sidebar_width + 10, output_rect_pos + 10);
                    create_rect(&side_rect, Color(39, 40, 49), sidebar_width, winY, 0, 0);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                    thread_struct.output_rect = &output_rect;
                    history_dialog(&history, &font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
                }
            
        // 3
                if(winX < min_x) 
                    window.setSize(Vector2u(min_x, winY)); 

                if(winY < min_y) 
                    window.setSize(Vector2u(winX, min_y)); 

            }

            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mouse_pos = Mouse::getPosition(window);
                printf("mouse_x: %i\tmouse_y: %i\n", mouse_pos.x, mouse_pos.y);
                if(mouse_pos.y > input_rect.getGlobalBounds().top && mouse_pos.x > input_rect.getGlobalBounds().left) {
                    if(message == "Enter a message...") {
                        message.clear();
                        text.setString(message);
                    }
                    write_flag = 1;
                    text.setFillColor(Color::White);
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

            if(write_flag) {   // Поле ввода активировано
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                        // if (text.getLocalBounds().width > 8) {
                            // ++lineNumber; 
                            // message.clear();
                            // for (int i = 0; i < lineNumber; ++i)
                                // message += "\n";
                                // wrapText(message, 8, font, 14, false);
                        // }

                        cout << "Character typed: " << static_cast<char>(event.text.unicode) << endl;
                        message.insert(message.getSize(), event.text.unicode);
                        text.setString(message);
                    }
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Enter) {
                        cout << "message: " << message.toAnsiString() << endl;
                        message.insert(message.getSize(), " ");
                        // if(history != NULL)
                        //     fprintf(history, "%s: %s\n", settings_struct.username, message.getData());
                        sendPacket << message;

                        if (socket.send(sendPacket) != Socket::Done)
                            die_With_Error(DEVICE, "Failed to send a message to server!");
                        
                        sendPacket.clear();
                        message.clear();
                        text.setString(message);
                    } 
                    if(event.key.code == Keyboard::BackSpace && message.getSize() > 0) {
                        message.erase(message.getSize() - 1);
                        text.setString(message);
                    }       
                }
            }
        }  

        window.clear();
        window.draw(background);
        window.draw(output_rect);
        window.draw(input_rect);
        window.draw(side_rect);

        window.draw(text);
        window.draw(settings_text);
        window.draw(chats_text);
        window.draw(contacts_text);

        window.draw(settings_button);
        window.draw(chats_button);
        window.draw(contacts_button);
        window.draw(add_button);
        window.draw(menu_button);
        window.draw(line);

        for (int i = 0; i < dialog_count; i++) {
            window.draw(thread_struct.output_text_rect[i]);
            window.draw(thread_struct.recv_text[i]);
        }
        window.display();
    }
    // thread.wait();
    return 0;
}

void output_thread_func() {
    size_t received;
    Text* pre_recv_text;
    RectangleShape* pre_output_text_rect;
	Packet receivePacket;
    String receive;

    while (thread_struct.window->isOpen()) {
        if (thread_struct.socket->receive(receivePacket) != Socket::Done) {
            die_With_Error(DEVICE, "Failed to receive a message from the server!");
        }

		receivePacket >> receive;

        cout << "Received: \"" << receive.toAnsiString() << "\" (" << receive.getSize() << " bytes)" << endl;

        mutex.lock();
        pre_output_text_rect = thread_struct.output_text_rect;
        pre_recv_text = thread_struct.recv_text;
        
        draw_message_rect(&thread_struct.font, &thread_struct.output_rect, &pre_output_text_rect, &pre_recv_text, receive.toAnsiString().c_str(), 1);
        mutex.unlock();

        sleep(milliseconds(10));
    }
}