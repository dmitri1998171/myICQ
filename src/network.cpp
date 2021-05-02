#include "header.hpp"

#define DEVICE "CLIENT"

void network_func() {
    TcpSocket socket;
    IpAddress server_ip = "127.0.0.1";
    Socket::Status status = socket.connect(server_ip, PORT);
    if (status != Socket::Done)
        dieWithError(DEVICE, "Failed to connect to server!");

    char data[20];
    size_t received;

    if (socket.receive(data, sizeof(data), received) != Socket::Done)
        dieWithError(DEVICE, "Failed to receive a message from the server!");

    cout << "Received: " << data << " (" << received << " bytes)" << endl;

    getchar();
}