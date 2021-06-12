#include "header.hpp"

#define DEVICE "SERVER"

void die_With_Error(const char *device, const char *error_message) {
    time_t It = time(NULL);
    struct tm *ptr = localtime(&It);

    FILE *f = fopen("errors.log", "a+");
    printf("%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fprintf(f, "%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fclose(f);
    exit(1);
}

int main() {
    TcpSocket client;
    TcpListener listener;
    Packet packet;
    

    if (listener.listen(PORT) != Socket::Done)
        die_With_Error(DEVICE, "Failed to connect to client!");

    if (listener.accept(client) != Socket::Done)
        die_With_Error(DEVICE, "Failed to accept to client!");

    while (true) {
        if (client.receive(packet) != Socket::Done)
            die_With_Error(DEVICE, "Failed to receive a message from the client!");

        if (client.send(packet) != Socket::Done)
            die_With_Error(DEVICE, "Failed to send a message to client!");
    }

    getchar();
    return 0;
}