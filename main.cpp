#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <windows.h>
#include "NetworkHandler.h"
#include "Task.h"

#pragma comment(lib, "Ws2_32.lib")

int main(void) {

    const char IP_SERV[] = "127.0.0.1";         
    const int PORT_NUM = 9090;            

    int erStat;                        
    in_addr ip_to_num;

    erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);
    if (erStat <= 0) {
        std::cout << "Error in IP translation to special numeric format" << std::endl;
        return 1;
    }

    // WinSock initialization
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if ( erStat != 0 ) {
        std::cout << "Error WinSock version initializaion #";
        std::cout << WSAGetLastError();
        return 1;
    }
    else
    std::cout << "WinSock initialization is OK" << std::endl;

    // Server socket initialization
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSock == INVALID_SOCKET) {
        std::cout << "Error initialization socket # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Server socket initialization is OK" << std::endl;
  
    sockaddr_in servInfo;

    // Initializing servInfo structure
    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(PORT_NUM);

    // Server socket binding
    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
    if ( erStat != 0 ) {
        std::cout << "Error Socket binding to server info. Error #" << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Binding socket to Server info is OK" << std::endl;

    //Starting to listen to any Clients
    erStat = listen(ServSock, SOMAXCONN);
    if ( erStat != 0 ) {
        std::cout << "Can't start to listen to. Error #" << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else
        std::cout << "Listening..." << std::endl;

    sockaddr_in clientInfo;
    // Initializing clientInfo structure
    ZeroMemory(&clientInfo, sizeof(clientInfo));
    int clientInfo_size = sizeof(clientInfo);
    SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
    if (ClientConn == INVALID_SOCKET) {
        std::cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        closesocket(ClientConn);
        WSACleanup();
        return 1;
    }
    else {
        std::cout << "Connection to a client established with IP address"<< inet_ntoa(clientInfo.sin_addr) << std::endl;              
    }

    // creating network handler
    NetworkHandler network(ServSock, ClientConn);
    // sending welcome message
    network.sendMessage("\nWelcome to the server!\nPlease choose a task:\n1. Sum of two numbers\n2. Exit\n");

    while (true) {
        std::string choice = network.receiveMessage();
    
        if (choice == "1") {
            SumTask task(network);
            task.execute();
        } 
        else if (choice == "2" || choice == "xxx") {
            network.cleanup();
            return 0;
        }
        else  {
            continue;
        }
    }
    
    return 0;
}
