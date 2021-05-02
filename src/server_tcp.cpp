#include "header.hpp"

#define DEVICE "SERVER"

int main() {
    TcpSocket client;
    TcpListener listener;
    size_t received;
    struct networkStruct {
        String message;
        struct Settings settings_struct;
    }net_struct;

    if (listener.listen(PORT) != Socket::Done)
        die_With_Error(DEVICE, "Failed to connect to server!");

    while (true) {
        if (listener.accept(client) != Socket::Done)
            die_With_Error(DEVICE, "Failed to accept to server!");

        if (client.receive(&net_struct, sizeof(net_struct), received) != Socket::Done)
            die_With_Error(DEVICE, "Failed to receive a message from the server!");

        // cout << "username: " << net_struct.settings_struct.username << "\nmessage: \"" << net_struct.message.toAnsiString() << "\" (" << received << " bytes)\n";

        if (client.send(&net_struct, sizeof(net_struct)) != Socket::Done)
            die_With_Error(DEVICE, "Failed to send a message to server!");
    }

    getchar();
    return 0;
}