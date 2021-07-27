#include "header.hpp"
#include "draw.hpp"

#define DEVICE "CLIENT"

void die_With_Error(const char *device, const char *error_message) {
    time_t It = time(NULL);
    struct tm *ptr = localtime(&It);

    FILE *f = fopen("errors.log", "a+");
    printf("%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fprintf(f, "%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fclose(f);
    getchar();
    exit(1);
}

void json_parser_create(struct Settings* settings_struct) {
    FILE* pFile = fopen("settings.json", "rb");
    char buffer[65536];
    FileReadStream is(pFile, buffer, sizeof(buffer));
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(is);

    assert(document["username"].IsString());
    assert(document["status"].IsString());
    assert(document["id"].IsInt());
    assert(document["font_size"].IsInt());
    strcpy(settings_struct->username, document["username"].GetString()); 
    strcpy(settings_struct->status, document["status"].GetString()); 
    settings_struct->id = document["id"].GetInt();
    settings_struct->font_size = document["font_size"].GetInt();

    printf("username = %s\n", settings_struct->username);
    printf("status = %s\n", settings_struct->status);
    printf("id = %i\n", settings_struct->id);
    printf("font_size = %i\n\n", settings_struct->font_size);

    fclose(pFile);
}

void network_func(TcpSocket* socket) {
  IpAddress server_ip = "127.0.0.1";

  if(socket->connect(server_ip, PORT) != sf::Socket::Done){
    logl("Could not connect to the server\n");
  }
  else {
    logl("Connected to the server\n");
  }
}

void history_dialog(FILE** history, Font* font, RectangleShape* output_rect, RectangleShape* output_text_rect, Text* recv_text, struct Settings *settings_struct) {
    IGUI gui;
    
    ifstream in("history.txt"); 
    if(in.is_open()) {
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
            gui.createMessageRect(&font, &output_rect, output_text_rect, recv_text, str, pos);
        }

    }
    printf("\n");
    in.close();
}

String wrapText(String string, unsigned width, const Font &font, unsigned charicterSize, bool bold) {
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
