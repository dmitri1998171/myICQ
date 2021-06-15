#include "header.hpp"

#define DEVICE "CLIENT"
#define STR_SIZE 256
Mutex mutex;

enum States {
    CHAT_STATE,
    SETTINGS_STATE,
    CONTACTS_STATE
};

struct GraphicsThread {
    Font* font;
    TcpSocket* socket;
    RenderWindow* window;
    RectangleShape* output_rect;
    RectangleShape output_text_rect[STR_SIZE];
    Text recv_text[STR_SIZE];
    float output_rect_pos;
}thread_struct;

int sidebar_width;
int sep, dialog_count;
float font_size;
int state = CHAT_STATE;
int menu_flag, chat_add_flag, contact_add_flag = 0;

Texture settings_texture, 
chats_texture,
contacts_texture,
add_chat_texture,
add_contact_texture,
menu_texture;

Texture settings_active_texture, 
chats_active_texture,
contacts_active_texture,
add_chat_active_texture,
add_contact_active_texture,
menu_active_texture;

Sprite settings_button, 
chats_button,
contacts_button,
add_button,
menu_button;

void output_thread_func();

void settings_mode_func();
void chat_mode_func();
void contacts_mode_func();
void add_mode_func();
void menu_mode_func();

void click_check(void func(), Sprite* sprite, int mouse_x, int mouse_y) {
    if(mouse_y > sprite->getGlobalBounds().top && mouse_y < sprite->getGlobalBounds().height + sprite->getGlobalBounds().top) 
        if(mouse_x > sprite->getGlobalBounds().left && mouse_x < sprite->getGlobalBounds().width + sprite->getGlobalBounds().left) {
            func();
        }
}

int main() {
    int winX, winY;
    int write_flag = 0;
    int min_x = 470, min_y = 600, sub_min_x = 830;
    float output_rect_pos = HEIGHT * 0.9;
    float input_rect_pos = HEIGHT - output_rect_pos  - 20;
    char title[100] = {"myICQ    Name: "};
    struct Settings settings_struct;
    Packet sendPacket;
    Color line_color = Color(128, 128, 128);
    Color output_color = Color(27, 28, 37);
    Color sidebar_color = Color(39, 40, 49);
    
    json_parser_create(&settings_struct);
    
    sidebar_width = 400;
    font_size = settings_struct.font_size;
    strcat(title, settings_struct.username);

    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

/* Draw background */
    RectangleShape background, output_rect, input_rect, side_rect;

    create_rect(&background, output_color, WIDTH, HEIGHT, 0, 0);
    create_rect(&output_rect, output_color, (WIDTH - sidebar_width) - 20, output_rect_pos - 10, sidebar_width + 10, 10);
    create_rect(&input_rect, sidebar_color, (WIDTH - sidebar_width) - 20, input_rect_pos, sidebar_width + 10, output_rect_pos + 10);
    create_rect(&side_rect, sidebar_color, sidebar_width, HEIGHT, 0, 0);

/* Fonts and texts */
    Text text;
    Font font;
    String message = "Enter a message...";
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    text_params_func(&font, &text, message, line_color, input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + (input_rect.getGlobalBounds().height / 4));
    
/*UI*/ 
    RectangleShape line, search;
    Text search_text;
    create_rect(&line, line_color, sidebar_width, 1, 0, HEIGHT - 65);
    create_rect(&search, sidebar_color, sidebar_width - 30, input_rect_pos, 15, 50);
    search.setOutlineThickness(0.5);
    search.setOutlineColor(line_color);
    text_params_func(&font, &search_text, "serach", line_color, 35, input_rect_pos + getCenter_y(search, search_text));


// add sidebar buttons
    texture_loader(&add_contact_texture, "./media/icons/add_contact.png");
    texture_loader(&chats_texture, "./media/icons/chats.png");
    texture_loader(&settings_active_texture, "./media/icons/active/settings_active.png");
    texture_loader(&contacts_active_texture, "./media/icons/active/contacts_active.png");

    settings_button = UI_shedule(&settings_texture, "./media/icons/settings.png", ((sidebar_width / 4) * 3) - (settings_button.getGlobalBounds().width / 2), HEIGHT - 55);
    chats_button = UI_shedule(&chats_active_texture, "./media/icons/active/chat_active.png", ((sidebar_width / 4) * 2) - (chats_button.getGlobalBounds().width / 2), HEIGHT - 55);
    contacts_button = UI_shedule(&contacts_texture, "./media/icons/contacts.png", (sidebar_width / 4) - (contacts_button.getGlobalBounds().width / 2), HEIGHT - 55);
    add_button = UI_shedule(&add_chat_texture, "./media/icons/add_chat.png", sidebar_width - 35, 5);
    menu_button = UI_shedule(&menu_texture, "./media/icons/menu.png", 0, 5);

    Text settings_text, chats_text, contacts_text;
    text_params_func(&font, &settings_text, "settings", line_color, getCenter(settings_button, settings_text), settings_button.getGlobalBounds().top + settings_button.getGlobalBounds().height);
    text_params_func(&font, &chats_text, "chats", line_color, getCenter(chats_button, chats_text) + 15, chats_button.getGlobalBounds().top + chats_button.getGlobalBounds().height);
    text_params_func(&font, &contacts_text, "contacts", line_color, getCenter(contacts_button, contacts_text), contacts_button.getGlobalBounds().top + contacts_button.getGlobalBounds().height);

// menu button
    RectangleShape menu, 
    menu_add_contact, 
    menu_add_group, 
    menu_add_channel, 
    menu_read_all, 
    menu_line;
    
    Texture menu_add_contact_texture,
    menu_add_group_texture,
    menu_add_channel_texture,
    menu_read_all_texture;

    Sprite menu_add_contact_sprite,
    menu_add_group_sprite,
    menu_add_channel_sprite,
    menu_read_all_sprite;

    Text add_contact_text, add_group_text, add_channel_text, read_all_text;

    int menu_x = 5, menu_y = 5, menu_w = 160, menu_h = 200;
    int menu_button_h = (menu_h / 4);


    create_rect(&menu, sidebar_color, menu_w, menu_h, menu_x, menu_y);
    create_rect(&menu_line, line_color, menu_w, 1, menu_x, menu_button_h * 3);
    menu.setOutlineThickness(0.5);
    menu.setOutlineColor(line_color);

    create_rect(&menu_add_contact, sidebar_color, menu_w, menu_button_h, menu_x, menu_y);
    create_rect(&menu_add_group, sidebar_color, menu_w, menu_button_h, menu_x, menu_y + menu_button_h);
    create_rect(&menu_add_channel, sidebar_color, menu_w, menu_button_h, menu_x, menu_y + (menu_button_h * 2));
    create_rect(&menu_read_all, sidebar_color, menu_w, menu_button_h, menu_x, menu_y + (menu_button_h * 3));


    int button_y = menu_add_contact.getPosition().y;
    menu_add_contact_sprite = UI_shedule(&menu_add_contact_texture, "./media/icons/menu/menu_add_contact.png", menu_x + 10, button_y + 10);
    menu_add_group_sprite = UI_shedule(&menu_add_group_texture, "./media/icons/menu/menu_add_group.png", menu_x + 10, button_y + 10 + menu_button_h);
    menu_add_channel_sprite = UI_shedule(&menu_add_channel_texture, "./media/icons/menu/menu_add_channel.png", menu_x + 10, button_y + 10 + (menu_button_h * 2));
    menu_read_all_sprite = UI_shedule(&menu_read_all_texture, "./media/icons/menu/menu_read_all.png", menu_x + 10, button_y + 10 + (menu_button_h * 3));

    int m_i_w = menu_add_contact_sprite.getGlobalBounds().width * 2;
    text_params_func(&font, &add_contact_text, "Add contact", Color::White, menu_x + m_i_w, getCenter_y(menu_add_contact, add_contact_text));
    text_params_func(&font, &add_group_text, "Add group", Color::White, menu_x + m_i_w, getCenter_y(menu_add_group, add_group_text) + menu_button_h);
    text_params_func(&font, &add_channel_text, "Add channel", Color::White, menu_x + m_i_w, getCenter_y(menu_add_channel, add_channel_text) + (menu_button_h * 2));
    text_params_func(&font, &read_all_text, "Read all", Color::White, menu_x + m_i_w, getCenter_y(menu_read_all, read_all_text) + (menu_button_h * 3));

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
                    create_rect(&output_rect, output_color, winX - 20, output_rect_pos - 10, 10, 10);
                    create_rect(&input_rect, sidebar_color, winX - 20, input_rect_pos, 10, output_rect_pos + 10);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                    history_dialog(&history, &font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
                }

        // 1
                if(winX > sub_min_x) {
                    sidebar_width = 400;
                    output_rect_pos = winY - input_rect_pos;

                    create_rect(&background, output_color, winX, winY, 0, 0);
                    create_rect(&output_rect, output_color, winX - sidebar_width - 20, output_rect_pos - 10, sidebar_width + 10, 10);
                    create_rect(&input_rect, sidebar_color, (winX - sidebar_width) - 20, input_rect_pos, sidebar_width + 10, output_rect_pos + 10);
                    create_rect(&side_rect, sidebar_color, sidebar_width, winY, 0, 0);
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

        /* Collision cursor with UI */ 
            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mouse_pos = Mouse::getPosition(window);
                // printf("mouse_x: %i\tmouse_y: %i\n", mouse_pos.x, mouse_pos.y);

                write_flag = 0;
                menu_flag = 0;

                text.setFillColor(Color(128, 128, 128, 100));
                if(message.getSize() == 0) {
                    message = "Enter a message...";
                    text.setString(message);
                }
                
            // input_rect
                if(mouse_pos.y > input_rect.getGlobalBounds().top && mouse_pos.x > input_rect.getGlobalBounds().left) {
                    if(message == "Enter a message...") {
                        message.clear();
                        text.setString(message);
                    }
                    write_flag = 1;
                    text.setFillColor(Color::White);
                }

            // Buttons
                click_check(settings_mode_func, &settings_button, mouse_pos.x, mouse_pos.y);
                click_check(chat_mode_func, &chats_button, mouse_pos.x, mouse_pos.y);
                click_check(contacts_mode_func, &contacts_button, mouse_pos.x, mouse_pos.y);
                click_check(add_mode_func, &add_button, mouse_pos.x, mouse_pos.y);
                click_check(menu_mode_func, &menu_button, mouse_pos.x, mouse_pos.y);
            }

        // input_rect is active
            if(write_flag) {   
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
        
        window.draw(side_rect);
        window.draw(line);
        window.draw(settings_button);
        window.draw(chats_button);
        window.draw(contacts_button);
        window.draw(add_button);
        window.draw(menu_button);

        window.draw(settings_text);
        window.draw(chats_text);
        window.draw(contacts_text);

        if(menu_flag) {
            window.draw(menu);
            window.draw(menu_add_contact);
            window.draw(menu_add_group);
            window.draw(menu_add_channel);
            window.draw(menu_read_all);
            window.draw(menu_line);

            window.draw(add_contact_text);
            window.draw(add_group_text);
            window.draw(add_channel_text);
            window.draw(read_all_text);

            window.draw(menu_add_contact_sprite);
            window.draw(menu_add_group_sprite);
            window.draw(menu_add_channel_sprite);
            window.draw(menu_read_all_sprite);
        }

        if(chat_add_flag) {
            button_y = 120;
            // menu_add_contact.setPosition(15, button_y);
            // menu_add_group.setPosition(15, button_y + menu_button_h);
            // menu_add_channel.setPosition(15, button_y + (menu_button_h * 2));
            // menu_read_all.setPosition(15, button_y + (menu_button_h * 3));

            add_contact_text.setPosition(60, button_y);
            add_group_text.setPosition(60, button_y + menu_button_h);
            add_channel_text.setPosition(60, button_y + (menu_button_h * 2));
            read_all_text.setPosition(60, button_y+ (menu_button_h * 3));

            menu_add_contact_sprite.setPosition(15, button_y);
            menu_add_group_sprite.setPosition(15, button_y + menu_button_h);
            menu_add_channel_sprite.setPosition(15, button_y + (menu_button_h * 2));
            menu_read_all_sprite.setPosition(15, button_y+ (menu_button_h * 3));

            window.draw(search);
            window.draw(search_text);

            // window.draw(menu_add_contact);
            // window.draw(menu_add_group);
            // window.draw(menu_add_channel);
            // window.draw(menu_read_all);

            window.draw(add_contact_text);
            window.draw(add_group_text);
            window.draw(add_channel_text);
            window.draw(read_all_text);

            window.draw(menu_add_contact_sprite);
            window.draw(menu_add_group_sprite);
            window.draw(menu_add_channel_sprite);
            window.draw(menu_read_all_sprite);
        }

        switch(state) {
            case CHAT_STATE:
                window.draw(input_rect);
                window.draw(text);

                for (int i = 0; i < dialog_count; i++) {
                    window.draw(thread_struct.output_text_rect[i]);
                    window.draw(thread_struct.recv_text[i]);
                }
                break;
            case SETTINGS_STATE:
                break;
            case CONTACTS_STATE:
                break;
        }
        window.display();
    }
    return 0;
}

void settings_mode_func() {
    cout << "settings_mode_func" << endl;
    state = SETTINGS_STATE;
    settings_button.setTexture(settings_active_texture);
    chats_button.setTexture(chats_texture);
    contacts_button.setTexture(contacts_texture);
}

void chat_mode_func() {
    cout << "chat_mode_func" << endl;
    state = CHAT_STATE;
    settings_button.setTexture(settings_texture);
    chats_button.setTexture(chats_active_texture);
    contacts_button.setTexture(contacts_texture);
}

void contacts_mode_func() {
    cout << "contacts_mode_func" << endl;
    state = CONTACTS_STATE;
    settings_button.setTexture(settings_texture);
    chats_button.setTexture(chats_texture);
    contacts_button.setTexture(contacts_active_texture);
}

void add_mode_func() {
    cout << "add_mode_func" << endl;
    chat_add_flag = 1;
}

void menu_mode_func() {
    cout << "menu_mode_func" << endl;
    menu_flag = 1;
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