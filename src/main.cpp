#include "gui.hpp"

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

void output_thread_func(TcpSocket*);
void splitString(string, string []);

Assets* Assets::instance = 0;
int sep;
int dialog_count;
float font_size;

int main() {
    int stringsCount = 0, stringsLexemes = 0;
    time_t t=time(NULL);
    string systemTime = ctime(&t);
    string date_time[6];
    struct Settings settings_struct;
    Packet sendPacket;
    
    Assets* assets = Assets::getInstance();
    Registry reg;
    IDrawUI drawUI;
    IGUI ui;

    json_parser_create(&settings_struct);
    font_size = settings_struct.font_size;
    
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "myICQ");

    /* Load assets */ 

    reg.font.loadFromFile("./media/fonts/CyrilicOld.TTF");

    assets->texture_loader(&reg.add_contact_texture, "./media/icons/add_contact.png");
    assets->texture_loader(&reg.chats_texture, "./media/icons/chats.png");
    assets->texture_loader(&reg.settings_active_texture, "./media/icons/active/settings_active.png");
    assets->texture_loader(&reg.contacts_active_texture, "./media/icons/active/contacts_active.png");
    
    assets->sprite_loader(&ui.add_button, &reg.add_chat_texture, "./media/icons/add_chat.png", drawUI.sidebar_width - 35, 5);
    assets->sprite_loader(&ui.menu_button, &reg.menu_texture, "./media/icons/menu.png", 0, 5);

    /*UI*/ 

// Background 
    RectangleShape background, output_rect, input_rect, side_rect, line, search;
    ui.addBackground(&background, &output_rect, &input_rect, &side_rect, &line, &search);

    Text text, search_text;
    drawUI.addText(&reg.font, &search_text, "search", reg.line_color, 35, reg.input_rect_pos + drawUI.getCenter_y(search, search_text));
    drawUI.addText(&reg.font, &text, reg.message, reg.line_color, input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + (input_rect.getGlobalBounds().height / 4));

// Buttons
    Text settings_text, chats_text, contacts_text;
    drawUI.addButton(&ui.contacts_button, &reg.contacts_texture, reg.line_color, &settings_text, &reg.font, "./media/icons/contacts.png", "contacts", 1);
    drawUI.addButton(&ui.chats_button, &reg.chats_active_texture, reg.line_color, &chats_text, &reg.font, "./media/icons/active/chat_active.png", "chats", 2);
    drawUI.addButton(&ui.settings_button, &reg.settings_texture, reg.line_color, &contacts_text, &reg.font, "./media/icons/settings.png", "settings", 3);

// Menu
    Text add_contact_text, add_group_text, add_channel_text, read_all_text;
    ui.addMenu(&add_contact_text, &add_group_text, &add_channel_text, &read_all_text);
    
    drawUI.addMenuButton(&ui.menu_add_contact, &reg.add_contact_texture, &ui.menu_add_contact_sprite, reg.sidebar_color, &add_contact_text, &reg.font, "./media/icons/menu/menu_add_contact.png", "Add contact", 0);
    drawUI.addMenuButton(&ui.menu_add_group, &reg.menu_add_group_texture, &ui.menu_add_group_sprite, reg.sidebar_color, &add_group_text, &reg.font, "./media/icons/menu/menu_add_group.png", "Add group", 1);
    drawUI.addMenuButton(&ui.menu_add_channel, &reg.menu_add_channel_texture, &ui.menu_add_channel_sprite, reg.sidebar_color, &add_channel_text, &reg.font, "./media/icons/menu/menu_add_channel.png", "Add channel", 2);
    drawUI.addMenuButton(&ui.menu_read_all, &reg.menu_read_all_texture, &ui.menu_read_all_sprite, reg.sidebar_color, &read_all_text, &reg.font, "./media/icons/menu/menu_read_all.png", "Read all", 3);

    /* Network */

    TcpSocket socket;
    network_func(&socket);
    
    /* Threads */

    thread_struct.font = &reg.font;
    thread_struct.socket = &socket;
    thread_struct.window = &window;
    thread_struct.output_rect = &output_rect;
    thread_struct.output_rect_pos = reg.output_rect_pos;
    
    /* History */ 

    FILE* history;
    history_dialog(&history, &reg.font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
    
    Thread thread(&output_thread_func, &socket);
    thread.launch();
    
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

                reg.winX = window.getSize().x;
                reg.winY = window.getSize().y;
                
        // 2
                if(reg.min_x < reg.winX < reg.sub_min_x) {
                    reg.output_rect_pos = HEIGHT * 0.8;
                    drawUI.sidebar_width = 0;
                    
                    side_rect.setSize(Vector2f(0, 0));
                    (&output_rect, reg.output_color, reg.winX - 20, reg.output_rect_pos - 10, 10, 10);
                    drawUI.createRect(&input_rect, reg.sidebar_color, reg.winX - 20, reg.input_rect_pos, 10, reg.output_rect_pos + 10);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                    history_dialog(&history, &reg.font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
                }

        // 1
                if(reg.winX > reg.sub_min_x) {
                    drawUI.sidebar_width = 400;
                    reg.output_rect_pos = reg.winY - reg.input_rect_pos;

                    drawUI.createRect(&background, reg.output_color, reg.winX, reg.winY, 0, 0);
                    drawUI.createRect(&output_rect, reg.output_color, reg.winX - drawUI.sidebar_width - 20, reg.output_rect_pos - 10, drawUI.sidebar_width + 10, 10);
                    drawUI.createRect(&input_rect, reg.sidebar_color, (reg.winX - drawUI.sidebar_width) - 20, reg.input_rect_pos, drawUI.sidebar_width + 10, reg.output_rect_pos + 10);
                    drawUI.createRect(&side_rect, reg.sidebar_color, drawUI.sidebar_width, reg.winY, 0, 0);
                    text.setPosition(Vector2f(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5));
                    thread_struct.output_rect = &output_rect;
                    history_dialog(&history, &reg.font, thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, &settings_struct);
                }
            
        // 3
                if(reg.winX < reg.min_x) 
                    window.setSize(Vector2u(reg.min_x, reg.winY)); 

                if(reg.winY < reg.min_y) 
                    window.setSize(Vector2u(reg.winX, reg.min_y)); 

            }

        /* Click on UI elements */ 
        
            if(event.type == Event::MouseButtonReleased) 
                if(event.mouseButton.button == Mouse::Left) {
                    Vector2i mouse_pos = Mouse::getPosition(window);

                    text.setFillColor(reg.line_color);
                    if(reg.message.getSize() == 0) {
                        reg.message = "Enter a message...";
                        text.setString(reg.message);
                    }
                    
                // input_rect
                    if(ui.checkToClickRect(&window, &input_rect)) {
                        if(reg.message == "Enter a message...") {
                            reg.message.clear();
                            text.setString("");
                        }
                        reg.write_flag = 1;
                        text.setFillColor(Color::White);
                    }
                    else
                        reg.write_flag = 0;

                // Sidebar buttons
                    if(ui.checkToClickSprite(&window, &ui.settings_button)) {
                        logl("SETTINGS_STATE");
                        reg.state = SETTINGS_STATE;
                        ui.changeTexture(&reg.settings_active_texture, &reg.chats_texture, &reg.contacts_texture);
                    }
                    if(ui.checkToClickSprite(&window, &ui.chats_button)) {
                        logl("CHAT_STATE");
                        reg.state = CHAT_STATE;
                        ui.changeTexture(&reg.settings_texture, &reg.chats_active_texture, &reg.contacts_texture);
                    }
                    if(ui.checkToClickSprite(&window, &ui.contacts_button)) {
                        logl("CONTACTS_STATE");
                        reg.state = CONTACTS_STATE;
                        ui.changeTexture(&reg.settings_texture, &reg.chats_texture, &reg.contacts_active_texture);
                    }
                    
                // Menu or Add chat button
                    if(reg.menu_flag || reg.add_chat_flag)
                        ui.checkToClickMenuButtons(&window);

                    if(ui.checkToClickSprite(&window, &ui.add_button)) {
                        logl("add_chat_button");
                        reg.add_chat_flag++;
                    }
                    else
                        reg.add_chat_flag = 0;

                    if(ui.checkToClickSprite(&window, &ui.menu_button)) {
                        logl("menu_button");
                        reg.menu_flag++;
                    }
                    else
                        reg.menu_flag = 0;
                }

        // input_rect is active
            if(reg.write_flag) {   
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                        // logl("Character typed: " + static_cast<char>(event.text.unicode));

                        stringsLexemes = static_cast<int>(reg.message.getSize() * text.getCharacterSize() / 2) % static_cast<int>(input_rect.getGlobalBounds().width);

                        if(stringsLexemes == 1 || stringsLexemes == 2 || stringsLexemes == 3) {
                            text.setPosition(input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top);
                            reg.message.insert(reg.message.getSize(), "\n");
                            stringsCount++;
                        }

                        if(stringsCount < 3) {
                            reg.message.insert(reg.message.getSize(), event.text.unicode);
                            text.setString(reg.message);
                        }
                    }
                }

                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Enter) {
                        if(reg.message.getSize() > 0) {
                            logl("message: " + reg.message.toAnsiString());
                            reg.message.insert(reg.message.getSize(), " ");
                            // if(history != NULL)
                            //     fprintf(history, "%s: %s\n", settings_struct.username, reg.message.getData());
                            sendPacket << reg.message;

                            if (socket.send(sendPacket) == Socket::Done) {
                                splitString(systemTime, date_time);
                                reg.time = date_time[3];

                                for(int i = 0; i < 6; i++) 
                                    if(i != 3) 
                                        reg.date.append(date_time[i] + " ");

                                logl(reg.date);

                                reg.message.insert(reg.message.getSize(), reg.time);
                                drawUI.createMessageRect(&thread_struct.font, &thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, reg.message.toAnsiString().c_str(), 1);
                                sendPacket.clear();
                                reg.message.clear();
                                text.setString(reg.message);
                            }
                            else
                                die_With_Error(DEVICE, "Failed to send a message to server!");
                        }
                    } 
                    if(event.key.code == Keyboard::BackSpace && reg.message.getSize() > 0) {
                        reg.message.erase(reg.message.getSize() - 1);
                        text.setString(reg.message);
                    }
                }
            }
        }  

        window.clear();
        ui.drawBackground(&window, &background, &output_rect, &input_rect, &side_rect, &line);
        ui.drawGrandButtons(&window, &settings_text, &chats_text, &contacts_text);

        if(reg.menu_flag) {
            ui.drawMenuRects(&window);
            ui.drawMenu(&window, &add_contact_text, &add_group_text, &add_channel_text, &read_all_text);
        }

        if(reg.add_chat_flag) {
            int button_y = 120;
            ui.changeMenuPosition(button_y, &add_contact_text, &add_group_text, &add_channel_text, &read_all_text);
            ui.drawMenu(&window, &add_contact_text, &add_group_text, &add_channel_text, &read_all_text);
            
            window.draw(search);
            window.draw(search_text);
        }

        switch(reg.state) {
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
    IDrawUI drawUI;

    while (true) {
        if (socket->receive(receivePacket) == Socket::Done) {
            receivePacket >> received_string;
            // logl("Received: \"" + received_string.toAnsiString() + "\" (" + received_string.getSize() + " bytes)");

            mutex.lock();
            drawUI.createMessageRect(&thread_struct.font, &thread_struct.output_rect, thread_struct.output_text_rect, thread_struct.recv_text, received_string.toAnsiString().c_str(), 0);
            mutex.unlock();
        }
        else 
            die_With_Error(DEVICE, "Failed to receive a reg.message from the server!");

        sleep(milliseconds(10));
    }
}