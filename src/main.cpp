#include "header.hpp"
#include "draw.hpp"

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
int menu_x, menu_y, menu_w, menu_h;
int menu_button_h;

Sprite settings_button, chats_button, contacts_button,
add_button, menu_button;

void output_thread_func(TcpSocket *socket);

inline void setActiveButtonIcon(Texture *settings, Texture *chats, Texture *contacts) {
    settings_button.setTexture(*settings);
    chats_button.setTexture(*chats);
    contacts_button.setTexture(*contacts);
}

int main() {
    int winX = 0, winY = 0;
    int state = CHAT_STATE;
    int write_flag = 0, menu_flag = 0, add_chat_flag = 0, contact_add_flag = 0;
    int min_x = 470, min_y = 600, sub_min_x = 830;
    float output_rect_pos = HEIGHT * 0.9;
    float input_rect_pos = HEIGHT - output_rect_pos  - 20;
    char title[100] = {"myICQ    Name: "};
    struct Settings settings_struct;
    Packet sendPacket;
    Color line_color = Color(128, 128, 128);
    Color output_color = Color(27, 28, 37);
    Color sidebar_color = Color(39, 40, 49);
    
    IGUI gui;

    json_parser_create(&settings_struct);
    
    sidebar_width = 400;
    font_size = settings_struct.font_size;
    strcat(title, settings_struct.username);
    menu_x = 45, menu_y = 5, menu_w = 160, menu_h = 200;
    menu_button_h = (menu_h / 4);

    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

/* Draw background */
    RectangleShape background, output_rect, input_rect, side_rect;

    gui.createRect(&background, output_color, WIDTH, HEIGHT, 0, 0);
    gui.createRect(&output_rect, output_color, (WIDTH - sidebar_width) - 20, output_rect_pos - 10, sidebar_width + 10, 10);
    gui.createCircleAngleRect(&input_rect, sidebar_color, (WIDTH - sidebar_width) - 20, input_rect_pos, sidebar_width + 10, output_rect_pos + 10);
    gui.createRect(&side_rect, sidebar_color, sidebar_width, HEIGHT, 0, 0);

/* Fonts and texts */
    Text text;
    Font font;
    String message = "Enter a message...";
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    gui.addText(&font, &text, message, line_color, input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + (input_rect.getGlobalBounds().height / 4));
    
/*UI*/ 
    RectangleShape line, search;
    Text search_text;
    gui.createRect(&line, line_color, sidebar_width, 1, 0, HEIGHT - 65);
    gui.createRect(&search, sidebar_color, sidebar_width - 30, input_rect_pos, 15, 50);
    search.setOutlineThickness(0.5);
    search.setOutlineColor(line_color);
    gui.addText(&font, &search_text, "serach", line_color, 35, input_rect_pos + gui.getCenter_y(search, search_text));

// add sidebar buttons
    Texture settings_texture, chats_texture, contacts_texture,
    add_chat_texture, add_contact_texture, menu_texture;

    Texture settings_active_texture, chats_active_texture, contacts_active_texture,
    add_chat_active_texture, add_contact_active_texture, menu_active_texture;

    Text settings_text, chats_text, contacts_text;

    gui.texture_loader(&add_contact_texture, "./media/icons/add_contact.png");
    gui.texture_loader(&chats_texture, "./media/icons/chats.png");
    gui.texture_loader(&settings_active_texture, "./media/icons/active/settings_active.png");
    gui.texture_loader(&contacts_active_texture, "./media/icons/active/contacts_active.png");

    gui.sprite_loader(&add_button, &add_chat_texture, "./media/icons/add_chat.png", sidebar_width - 35, 5);
    gui.sprite_loader(&menu_button, &menu_texture, "./media/icons/menu.png", 0, 5);

    gui.addButton(&contacts_button, &contacts_texture, line_color, &settings_text, &font, "./media/icons/contacts.png", "contacts", 1);
    gui.addButton(&chats_button, &chats_active_texture, line_color, &chats_text, &font, "./media/icons/active/chat_active.png", "chats", 2);
    gui.addButton(&settings_button, &settings_texture, line_color, &contacts_text, &font, "./media/icons/settings.png", "settings", 3);

// menu button
    Sprite menu_add_contact_sprite, menu_add_group_sprite,
    menu_add_channel_sprite, menu_read_all_sprite;
    
    RectangleShape menu, menu_add_contact, menu_add_group, 
    menu_add_channel, menu_read_all, menu_line;
    
    Texture menu_add_contact_texture, menu_add_group_texture,
    menu_add_channel_texture, menu_read_all_texture;

    Text add_contact_text, add_group_text, add_channel_text, read_all_text;

    gui.createRect(&menu, sidebar_color, menu_w, menu_h, menu_x, menu_y);
    gui.createRect(&menu_line, line_color, menu_w, 1, menu_x, menu_button_h * 3);
    menu.setOutlineThickness(0.5);
    menu.setOutlineColor(line_color);

    gui.addMenuButton(&menu_add_contact, &add_contact_texture, &menu_add_contact_sprite, sidebar_color, &add_contact_text, &font, "./media/icons/menu/menu_add_contact.png", "Add contact", 0);
    gui.addMenuButton(&menu_add_group, &menu_add_group_texture, &menu_add_group_sprite, sidebar_color, &add_group_text, &font, "./media/icons/menu/menu_add_group.png", "Add group", 1);
    gui.addMenuButton(&menu_add_channel, &menu_add_channel_texture, &menu_add_channel_sprite, sidebar_color, &add_channel_text, &font, "./media/icons/menu/menu_add_channel.png", "Add channel", 2);
    gui.addMenuButton(&menu_read_all, &menu_read_all_texture, &menu_read_all_sprite, sidebar_color, &read_all_text, &font, "./media/icons/menu/menu_read_all.png", "Read all", 3);

/* Network */
    TcpSocket socket;
    network_func(&socket);
    
/* Create the thread */
    thread_struct.font = &font;
    thread_struct.socket = &socket;
    thread_struct.window = &window;
    thread_struct.output_rect = &output_rect;
    thread_struct.output_rect_pos = output_rect_pos;
    
/* Dialog history */ 
    FILE* history;
    history_dialog(&history, &font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
    
    Thread thread(&output_thread_func, &socket);
    thread.launch();

    int stringsCount = 0;    
    int stringsLexemes = 0;

    while (true) {
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
                    (&output_rect, output_color, winX - 20, output_rect_pos - 10, 10, 10);
                    gui.createRect(&input_rect, sidebar_color, winX - 20, input_rect_pos, 10, output_rect_pos + 10);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                    history_dialog(&history, &font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
                }

        // 1
                if(winX > sub_min_x) {
                    sidebar_width = 400;
                    output_rect_pos = winY - input_rect_pos;

                    gui.createRect(&background, output_color, winX, winY, 0, 0);
                    gui.createRect(&output_rect, output_color, winX - sidebar_width - 20, output_rect_pos - 10, sidebar_width + 10, 10);
                    gui.createRect(&input_rect, sidebar_color, (winX - sidebar_width) - 20, input_rect_pos, sidebar_width + 10, output_rect_pos + 10);
                    gui.createRect(&side_rect, sidebar_color, sidebar_width, winY, 0, 0);
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
            if(event.type == Event::MouseButtonReleased) 
                if(event.mouseButton.button == Mouse::Left) {
                    Vector2i mouse_pos = Mouse::getPosition(window);

                    text.setFillColor(line_color);
                    if(message.getSize() == 0) {
                        message = "Enter a message...";
                        text.setString(message);
                    }
                    
                // input_rect
                    if(state == CHAT_STATE && input_rect.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                        if(message == "Enter a message...") {
                            message.clear();
                            text.setString(message);
                        }
                        write_flag = 1;
                        text.setFillColor(Color::White);
                    }
                    else
                        write_flag = 0;

                // Sidebar buttons
                    if(settings_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                        cout << "SETTINGS_STATE" << endl;
                        state = SETTINGS_STATE;
                        setActiveButtonIcon(&settings_active_texture, &chats_texture, &contacts_texture);
                    }
                    if(chats_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                        cout << "CHAT_STATE" << endl;
                        state = CHAT_STATE;
                        setActiveButtonIcon(&settings_texture, &chats_active_texture, &contacts_texture);
                    }
                    if(contacts_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                        cout << "CONTACTS_STATE" << endl;
                        state = CONTACTS_STATE;
                        setActiveButtonIcon(&settings_texture, &chats_texture, &contacts_active_texture);
                    }

                // Add chat button
                    if(add_chat_flag) {
                        if(menu_add_contact.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "add contact" << endl;
                        }
                        if(menu_add_group.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "add group" << endl;
                        }
                        if(menu_add_channel.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "add channel" << endl;
                        }
                        if(menu_read_all.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "read all" << endl;
                        }
                    }

                    if(add_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                        cout << "add_chat_button" << endl;
                        add_chat_flag++;
                    }
                    else
                        add_chat_flag = 0;

                // Menu button
                    if(menu_flag) {
                        if(menu_add_contact.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "menu_add_contact_func" << endl;
                        }
                        if(menu_add_group.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "menu_add_group" << endl;
                        }
                        if(menu_add_channel.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "menu_add_channel" << endl;
                        }
                        if(menu_read_all.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                            cout << "menu_read_all" << endl;
                        }
                    }

                    if(menu_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
                        cout << "menu_button" << endl;
                        menu_flag++;
                    }
                    else
                        menu_flag = 0;
                }

        // input_rect is active
            if(write_flag) {   
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                        // cout << "Character typed: " << static_cast<char>(event.text.unicode) << endl;

                        stringsLexemes = static_cast<int>(message.getSize() * (font_size / 2)) % static_cast<int>(input_rect.getGlobalBounds().width);

                        if(stringsLexemes == 1 || stringsLexemes == 2 || stringsLexemes == 3) {
                            text.setPosition(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top);
                            message.insert(message.getSize(), "\n");
                            stringsCount++;
                        }

                        if(stringsCount < 3) {
                            message.insert(message.getSize(), event.text.unicode);
                            text.setString(message);
                        }
                    }
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Enter) {
                        cout << "message: " << message.toAnsiString() << endl;
                        message.insert(message.getSize(), " ");
                        // if(history != NULL)
                        //     fprintf(history, "%s: %s\n", settings_struct.username, message.getData());
                        sendPacket << message;

                        if (socket.send(sendPacket) == Socket::Done) {
                            gui.createMessageRect(&thread_struct.font, &thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, message.toAnsiString().c_str(), 1);
                            sendPacket.clear();
                            message.clear();
                            text.setString(message);
                        }
                        else
                            die_With_Error(DEVICE, "Failed to send a message to server!");
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

        if(add_chat_flag) {
            int button_y = 120;
            menu_add_contact.setPosition(15, button_y);
            menu_add_group.setPosition(15, button_y + menu_button_h);
            menu_add_channel.setPosition(15, button_y + (menu_button_h * 2));
            menu_read_all.setPosition(15, button_y + (menu_button_h * 3));

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

void output_thread_func(TcpSocket *socket) {
	Packet receivePacket;
    String received_string;
    IGUI gui;

    while (true) {
        if (socket->receive(receivePacket) == Socket::Done) {
            receivePacket >> received_string;
            cout << "Received: \"" << received_string.toAnsiString() << "\" (" << received_string.getSize() << " bytes)" << endl;

            mutex.lock();
            gui.createMessageRect(&thread_struct.font, &thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, received_string.toAnsiString().c_str(), 0);
            mutex.unlock();
        }
        else 
            die_With_Error(DEVICE, "Failed to receive a message from the server!");

        sleep(milliseconds(10));
    }
}