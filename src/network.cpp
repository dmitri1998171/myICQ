#include "header.hpp"

#define DEVICE "CLIENT"

void network_thread_func(TcpSocket* socket) {
    size_t received;
    char data[256] = "network message";

    if (socket->send(data, sizeof(data)) != Socket::Done)
        dieWithError(DEVICE, "Failed to send a message to server!");

    if (socket->receive(data, sizeof(data), received) != Socket::Done)
        dieWithError(DEVICE, "Failed to receive a message from the server!");

    cout << "Received: \"" << data << "\" (" << received << " bytes)" << endl;
}

void network_func(struct Settings settings_struct) {
    TcpSocket socket;
    IpAddress server_ip = "127.0.0.1";

    Socket::Status status = socket.connect(server_ip, PORT);
    if (status != Socket::Done)
        dieWithError(DEVICE, "Failed to connect to server!");
}