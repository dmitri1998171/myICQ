#include "header.hpp"

#define DEVICE "SERVER"

using namespace std;
using namespace sf;

TcpListener listener;
vector<TcpSocket *> client_array;

void die_With_Error(const char *device, const char *error_message) {
    time_t It = time(NULL);
    struct tm *ptr = localtime(&It);

    FILE *f = fopen("errors.log", "a+");
    printf("%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fprintf(f, "%s - %s - ERROR: %s\n", device, asctime(ptr), error_message);
    fclose(f);
    exit(1);
}

void ConnectFunc() {
    while(true){
        TcpSocket * new_client = new sf::TcpSocket();
        if(listener.accept(*new_client) == sf::Socket::Done){
            new_client->setBlocking(false);
            client_array.push_back(new_client);
            logl("Added client " << new_client->getRemoteAddress() << ":" << new_client->getRemotePort() << " on slot " << client_array.size());
        }else{
            logl("Server listener error, restart the server");
            delete(new_client);
            break;
        }
     }
}

void DisconnectClient(sf::TcpSocket * socket_pointer, size_t position) {
     logl("Client " << socket_pointer->getRemoteAddress() << ":" << socket_pointer->getRemotePort() << " disconnected, removing");
     socket_pointer->disconnect();
     delete(socket_pointer);
     client_array.erase(client_array.begin() + position);
}

void BroadcastPacket(sf::Packet & packet, sf::IpAddress exclude_address, unsigned short port){
     for(size_t iterator = 0; iterator < client_array.size(); iterator++){
          sf::TcpSocket * client = client_array[iterator];
          if(client->getRemoteAddress() != exclude_address || client->getRemotePort() != port){
               if(client->send(packet) != sf::Socket::Done){
                    logl("Could not send packet on broadcast");
               }
          }
     }
}

void ReceivePacket(TcpSocket * client, size_t iterator) {
    Packet packet;
    if(client->receive(packet) == sf::Socket::Disconnected)
        DisconnectClient(client, iterator);
    else {
        if(packet.getDataSize() > 0) {
            String received_message;
            packet >> received_message;
            packet.clear();

            logl(received_message.toAnsiString());
            packet << received_message;// << client->getRemoteAddress().toString() << client->getRemotePort();

            BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
            logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " '" << received_message.toAnsiString() << "'");
        }
    }
}

void ManagePackets() {
    while(true){
        for(size_t iterator = 0; iterator < client_array.size(); iterator++)
            ReceivePacket(client_array[iterator], iterator);
        sleep(milliseconds(10));
    }
}

int main() {
	TcpSocket client;
    Packet sendPacket;
    string message;

    if(listener.listen(PORT) != sf::Socket::Done){
          logl("Could not listen");
     }

	Thread receive_thr(&ConnectFunc);
	receive_thr.launch();

     ManagePackets();

    return 0;				
}
