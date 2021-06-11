#include "header.hpp"

#define DEVICE "CLIENT"
#define STR_SIZE 256
Mutex mutex;

int sep, dialog_count;
int sidebar_width = 400;
float str_length = 50;
float font_size = 14;
float string_count = 1;

struct GraphicsThread {
    Font* font;
    TcpSocket* socket;
    RenderWindow* window;
    RectangleShape* output_rect;
    RectangleShape output_text_rect[STR_SIZE];
    Text recv_text[STR_SIZE];
    struct networkStruct net_struct;
    float output_rect_pos;
}thread_struct;

void output_thread_func();

void create_rect(RectangleShape *rect, Color color, float width, float height, float x, float y) {
    rect->setSize(Vector2f(width, height));
    rect->setPosition(Vector2f(x, y));
    rect->setFillColor(Color(color));
}

void text_params_func(Font* font, Text *text, String message, Color color, float x, float y) {
    text->setFont(*font);
    text->setCharacterSize(14);
    text->setString(message);
    text->setFillColor(Color(color));
    text->setPosition(Vector2f(x, y));
}

void draw_message_rect(Font** font, RectangleShape** output_rect, RectangleShape** output_text_rect, Text** recv_text, const char *str, int pos) {
    int padding_x = 7, margin_x = 0, margin_y = 5; 
    float output_rect_x, output_rect_y, output_rect_w, output_rect_h;
    RectangleShape* pre_output_rect = *output_rect;
    RectangleShape* pre_output_text_rect = *output_text_rect;
    Text* pre_recv_text = *recv_text;

    output_rect_h = font_size * string_count;
    output_rect_y = pre_output_rect->getGlobalBounds().top;
    output_rect_w = (strlen(str) * font_size) * 0.5;

    if(pos) 
        output_rect_x = (pre_output_rect->getSize().x + sidebar_width) - output_rect_w - margin_x;
    else 
        output_rect_x = pre_output_rect->getGlobalBounds().left + margin_x;
    
    create_rect(&pre_output_text_rect[dialog_count], Color(34, 52, 79), output_rect_w, output_rect_h + margin_y, output_rect_x, output_rect_y + sep);
    text_params_func(*font, &pre_recv_text[dialog_count], str, Color::White, output_rect_x + padding_x, output_rect_y + sep);
    
    sep += string_count + 30;
    dialog_count++;
}

void history_dialog(FILE** history, Font* font, RectangleShape* output_rect, RectangleShape* output_text_rect, Text* recv_text, struct Settings *settings_struct) {
    ifstream in("history.txt"); 
    if (in.is_open()) {
        string line;
        char *str;
        int pos = 0;    // Позиция сообщения на экране (справа или слева)
        sep = 10, dialog_count = 0;

        while (getline(in, line)) {        
            str = strdup(line.c_str());
            str = strtok(str, ": ");

            if(!strcmp(str, settings_struct->username)) 
                pos = 1;
            else 
                pos = 0;

            str = strtok(NULL, ":");
            draw_message_rect(&font, &output_rect, &output_text_rect, &recv_text, str, pos);
        }

    }
    printf("\n");
    in.close();
}

String wrapText(String string, unsigned width, const Font &font, unsigned charicterSize, bool bold){
  unsigned currentOffset = 0;
  bool firstWord = true;
  std::size_t wordBegining = 0;

  for (std::size_t pos(0); pos < string.getSize(); ++pos) {
    auto currentChar = string[pos];
    if (currentChar == '\n'){
      currentOffset = 0;
      firstWord = true;
      continue;
    } else if (currentChar == ' ') {
      wordBegining = pos;
      firstWord = false;
    }

    auto glyph = font.getGlyph(currentChar, charicterSize, bold);
    currentOffset += glyph.advance;

    if (!firstWord && currentOffset > width) {
      pos = wordBegining;
      string[pos] = '\n';
      firstWord = true;
      currentOffset = 0;
    }
  }

  return string;
}

int main() {
    int winX, winY;
    int write_flag = 0;
    int min_x = 470, min_y = 600, sub_min_x = 830;
    float output_rect_pos = HEIGHT * 0.8;
    float input_rect_pos = HEIGHT - output_rect_pos;
    char title[100] = {"myICQ    Name: "};
    struct Settings settings_struct;
    struct networkStruct net_struct;
    Packet sendPacket;
    
    json_parser_create(&settings_struct);

    strcat(title, settings_struct.username);
    RenderWindow window(VideoMode(WIDTH, HEIGHT), title);

    /* Draw background */
    RectangleShape background, output_rect, input_rect, side_rect;

    create_rect(&background, Color(27, 28, 37), WIDTH, HEIGHT, 0, 0);
    create_rect(&output_rect, Color(27, 28, 37), (WIDTH - sidebar_width) - 20, output_rect_pos - 10, sidebar_width + 10, 10);
    create_rect(&input_rect, Color(39, 40, 49), (WIDTH - sidebar_width) - 20, input_rect_pos - 20, sidebar_width + 10, output_rect_pos + 10);
    create_rect(&side_rect, Color(39, 40, 49), sidebar_width, HEIGHT, 0, 0);

    /* Fonts and texts */
    String message = "Enter a message...";
    Text text;
    Font font;
    font.loadFromFile("./media/fonts/CyrilicOld.TTF");
    Color text_color(128, 128, 128, 100);
    text_params_func(&font, &text, message, text_color, input_rect.getGlobalBounds().left + 10, input_rect.getGlobalBounds().top + 5);
    
    /* Network */
    TcpSocket socket;
    network_func(&socket);
    
    /* Create the thread */
    // window.setActive(false);
    thread_struct.font = &font;
    thread_struct.socket = &socket;
    thread_struct.window = &window;
    thread_struct.output_rect = &output_rect;
    thread_struct.net_struct = net_struct;
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
                fclose(history);
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
                        net_struct.message = message;
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

    // thread_struct.window->setActive(true);
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