#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <vector>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

constexpr short BUFFER_SIZE = 1024;

class NetworkHandler {
private:
    SOCKET ServSock, ClientConn;
    std::vector<char> buffer;

    void handleSendError(int packet_size);
    bool handleRecvError(int packet_size);

public:
    explicit NetworkHandler(SOCKET serverSocket, SOCKET clientSocket);
    void sendMessage(const std::string &message);
    std::string receiveMessage();
    void cleanup();
};

#endif 