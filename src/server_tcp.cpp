#include "header.hpp"

#define DEVICE "SERVER"

int main() {
    TcpListener listener;

    if (listener.listen(53001) != Socket::Done)
        dieWithError(DEVICE, "Failed to connect to server!");

    TcpSocket client;
    char data[20] = "network message";

    // while (true)
    // {
        if (listener.accept(client) != Socket::Done)
            dieWithError(DEVICE, "Failed to accept to server!");

        if (client.send(data, sizeof(data)) != Socket::Done)
            dieWithError(DEVICE, "Failed to send a message to server!");
    // }

    getchar();
    return 0;
}