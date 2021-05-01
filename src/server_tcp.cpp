#include "header.hpp"

int main() {
    TcpListener listener;

    if (listener.listen(PORT) != Socket::Done)
        dieWithError("Failed to connect to server!");

    TcpSocket client;
    char data[20] = "network message";

    // while (true)
    // {
        if (listener.accept(client) != Socket::Done)
            dieWithError("Failed to accept to server!");

        if (client.send(data, sizeof(data)) != Socket::Done)
            dieWithError("Failed to send a message to server!");
    // }

    getchar();
    return 0;
}