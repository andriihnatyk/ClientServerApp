#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Сonstants for connection
    const char SERVER_IP[] = "127.0.0.1";         
    const short SERVER_PORT_NUM = 9090;           
    const short BUFF_SIZE = 1024;               

    int erStat;                     
    in_addr ip_to_num;

    if (inet_pton(AF_INET, SERVER_IP, &ip_to_num) <= 0) {
        std::cout << "Error in IP translation to numeric format" << std::endl;
        return 1;
    }

    // Ініціалізація WinSock
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2,2), &wsData);
    if (erStat != 0) {
        std::cout << "Error WinSock initialization #" << WSAGetLastError() << std::endl;
        return 1;
    }
    std::cout << "WinSock initialization is OK" << std::endl;
   
    // Creating clients socket
    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSock == INVALID_SOCKET) {
        std::cout << "Error creating client socket #" << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Client socket initialization is OK" << std::endl;

    // Connection to the server
    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));
    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;  
    servInfo.sin_port = htons(SERVER_PORT_NUM);
    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if (erStat != 0) {
        std::cout << "Connection to Server FAILED. Error #" << WSAGetLastError() << std::endl;
        closesocket(ClientSock);
        WSACleanup();
        return 1;
    }
    std::cout << "Connection established SUCCESSFULLY." << std::endl;

    // Buffers for communication
    std::vector<char> servBuff(BUFF_SIZE);
    std::string clientInput;
    int packet_size;

    // Greeting message from the server
    packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
    if (packet_size <= 0) {
        std::cout << "Server closed connection or error occurred. Code: " << WSAGetLastError() << std::endl;
        closesocket(ClientSock);
        WSACleanup();
        return 1;
    }
    std::cout << servBuff.data() << std::endl;

    while (true) {

        std::getline(std::cin, clientInput);

        //Sending message to the server
        packet_size = send(ClientSock, clientInput.c_str(), clientInput.size() + 1, 0);
        if (packet_size == SOCKET_ERROR) {
            std::cout << "Can't send message to Server. Error #" << WSAGetLastError() << std::endl;
            closesocket(ClientSock);
            WSACleanup();
            return 1;
        }

        // Leave server if enterred "xxx" 
        if (clientInput == "xxx") {
            shutdown(ClientSock, SD_BOTH);
            closesocket(ClientSock);
            WSACleanup();
            return 0;
        }

        // Receiving message from server
        packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
        if (packet_size <= 0) {
            std::cout << "Can't receive message from Server. Error #" << WSAGetLastError() << std::endl;
            closesocket(ClientSock);
            WSACleanup();
            return 1;
        }

         std::cout <<  servBuff.data() << std::endl;
    }

    //Closing socket
    closesocket(ClientSock);
    WSACleanup();
    return 0;
}
