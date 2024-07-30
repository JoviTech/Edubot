/*
    @Author: Maik Basso <maik@maikbasso.com.br>
*/

#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
 
class SocketClient{
private:
    int sock;
    struct sockaddr_in server;
     
public:
    SocketClient();
    bool connectToServer(std::string address , int port);
    bool disconnectFromServer();
    bool isConnected();
    bool sendData(std::string data);
    std::string receiveData();
};
 
SocketClient::SocketClient(){
    this->sock = -1;
}

bool SocketClient::connectToServer(std::string address , int port){
    //create socket if it is not already created
    if(this->sock == -1){
        //Create socket
        this->sock = socket(AF_INET, SOCK_STREAM, 0);
        if (this->sock == -1){
            return false;
        }
    }
    //setup address structure
    in_addr_t addr = -1;
    if(inet_addr(address.c_str()) == addr){
        struct hostent *he;
        struct in_addr **addr_list;
         
        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL){
            return false;
        }
         
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;
 
        for(int i = 0; addr_list[i] != NULL; i++){
            server.sin_addr = *addr_list[i];             
            break;
        }
    }
    else{
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }
     
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
     
    //Connect to remote server
    if (connect(this->sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        return false;
    }
     
    return true;
}

bool SocketClient::disconnectFromServer(){
    close(this->sock);

    this->sock = -1;

    return true;
}

bool SocketClient::isConnected(){

    if(this->sock != -1){
        return true;
    }

    return false;
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
