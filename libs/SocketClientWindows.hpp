/*
    @Author: Maik Basso <maik@maikbasso.com.br>
    Version: Windows
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
 
class SocketClient{
private:
    SOCKET sock;
     
public:
    SocketClient();
    bool connectToServer(std::string address , int port);
    bool sendData(std::string data);
	bool isConnected();
	bool disconnectFromServer();
    std::string receiveData();
};
 
SocketClient::SocketClient(){
    this->sock = INVALID_SOCKET;
}

bool SocketClient::connectToServer(std::string address , int port){
    
    WSADATA wsaData;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    int iResult;
    std::string portStr = std::to_string(port);
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        //printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(address.c_str(), portStr.c_str(), &hints, &result);
    if ( iResult != 0 ) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return false;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        this->sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (this->sock == INVALID_SOCKET) {
            //printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( this->sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (this->sock == INVALID_SOCKET) {
        //printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
     
    return true;
}
 
bool SocketClient::sendData(std::string data){
    
    std::stringstream ss;
    ss << data << '\n';

    std::string dataReady = ss.str();

    if(send(this->sock , dataReady.c_str() , dataReady.size() , 0) < 0){
        return false;
    }

    return true;
}
 
std::string SocketClient::receiveData(){
    
    std::stringstream data;
    char buffer[1];

    while(recv(this->sock, buffer, 1, 0) > 0){
        if(buffer[0] != '\n'){
            data << buffer[0];
        }
        else {
            break;
        }
    }

    return data.str();
}


bool SocketClient::isConnected(){
    
    if (this->sock == INVALID_SOCKET) {
        return false;
    }

    return true;
}


bool SocketClient::disconnectFromServer(){

    shutdown(this->sock, SD_SEND);
    closesocket(this->sock);
    this->sock = INVALID_SOCKET;
    WSACleanup();

    return true;
}