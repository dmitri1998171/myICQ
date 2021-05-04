#include "header.hpp"

#define DEVICE "CLIENT"

void die_With_Error(const char *device, const char *error_message) {
    time_t It = time(NULL);
    struct tm *ptr = localtime(&It);

    FILE *f = fopen("errors.log", "a+");
    printf("%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fprintf(f, "%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fclose(f);
    // exit(1);
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
    strcpy(settings_struct->username, document["username"].GetString()); 
    strcpy(settings_struct->status, document["status"].GetString()); 
    settings_struct->id = document["id"].GetInt();

    printf("username = %s\n", settings_struct->username);
    printf("status = %s\n", settings_struct->status);
    printf("id = %i\n\n", settings_struct->id);

    fclose(pFile);
}

void network_func(TcpSocket* socket) {
    IpAddress server_ip = "127.0.0.1";
    Socket::Status status = socket->connect(server_ip, PORT);
    if (status != Socket::Done)
        die_With_Error(DEVICE, "Failed to connect to server!");
}