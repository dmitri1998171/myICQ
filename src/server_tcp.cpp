#include "header.hpp"

#define DEVICE "SERVER"

int main() {
    TcpSocket client;
    TcpListener listener;
    size_t received;
    // char data[20];
    String data;

    if (listener.listen(PORT) != Socket::Done)
        dieWithError(DEVICE, "Failed to connect to server!");

    while (true) {
        if (listener.accept(client) != Socket::Done)
            dieWithError(DEVICE, "Failed to accept to server!");

        if (client.receive(&data, sizeof(data), received) != Socket::Done)
            dieWithError(DEVICE, "Failed to receive a message from the server!");

        // cout << "Received: \"" << data << "\" (" << received << " bytes)" << endl;

        if (client.send(&data, sizeof(data)) != Socket::Done)
            dieWithError(DEVICE, "Failed to send a message to server!");
    }

    getchar();
    return 0;
}