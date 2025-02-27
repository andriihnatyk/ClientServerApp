#include "NetworkHandler.h"
#include <iostream>

NetworkHandler::NetworkHandler(SOCKET serverSocket, SOCKET clientSocket)
    : ServSock(serverSocket), ClientConn(clientSocket), buffer(BUFFER_SIZE) {}

void NetworkHandler::handleSendError(int packet_size) {
    if (packet_size == SOCKET_ERROR) {
        std::cout << "Can't send message to Client. Error # " << WSAGetLastError() << std::endl;
        cleanup();
        exit(1);
    }
}

bool NetworkHandler::handleRecvError(int packet_size) {
    if (packet_size == 0) {
        std::cout << "Client disconnected. Waiting for new connection..." << std::endl;
        return false; 
    } 
    else if (packet_size == SOCKET_ERROR) {
        std::cout << "Error receiving data. Error # " << WSAGetLastError() << std::endl;
        cleanup();
        exit(1);
    }
    return true;
}

void NetworkHandler::sendMessage(const std::string &message) {
    int packet_size = send(ClientConn, message.c_str(), message.size() + 1, 0);
    handleSendError(packet_size);
}

std::string NetworkHandler::receiveMessage() {
    int packet_size = recv(ClientConn, buffer.data(), buffer.size(), 0);
    
    if (!handleRecvError(packet_size)) {
        return "";  
    }

    buffer[packet_size] = '\0';  
    std::string receivedData(buffer.data());

    std::cout << "Received data: " << receivedData << std::endl;
    return receivedData;
}


void NetworkHandler::cleanup() {
    closesocket(ClientConn);  
    std::cout << "Client connection closed. Server is still running." << std::endl;
}
