#include "header.hpp"

void network_func() {
    TcpSocket socket;
    Socket::Status status = socket.connect("127.0.0.1", PORT);
    if (status != Socket::Done)
        dieWithError("Failed to connect to server!");

    char data[20];
    size_t received;

    if (socket.receive(data, sizeof(data), received) != Socket::Done)
        dieWithError("Failed to receive a message from the server!");

    cout << "Received: " << data << " (" << received << " bytes)" << endl;

    getchar();
}