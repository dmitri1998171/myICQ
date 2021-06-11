#include "header.hpp"

#define DEVICE "SERVER"

int main() {
    struct networkStruct net_struct;
    TcpSocket client;
    TcpListener listener;
    // size_t received;
    Packet packet;
    

    if (listener.listen(PORT) != Socket::Done)
        die_With_Error(DEVICE, "Failed to connect to client!");

    if (listener.accept(client) != Socket::Done)
        die_With_Error(DEVICE, "Failed to accept to client!");

    while (true) {
        if (client.receive(packet) != Socket::Done)
            die_With_Error(DEVICE, "Failed to receive a message from the client!");

        // cout << "username: " << net_struct.settings_struct.username << "\nmessage: \"" << net_struct.message.toAnsiString() << "\" (" << received << " bytes)\n";

        if (client.send(packet) != Socket::Done)
            die_With_Error(DEVICE, "Failed to send a message to client!");
    }

    getchar();
    return 0;
}