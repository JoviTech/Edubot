/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "Properties.hpp"

#ifdef _WIN32

#define CONFIG_FILE "windows-config.properties"

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include "SocketClientWindows.hpp"
#include "mingw-thread/mingw.thread.h"

#else

#define CONFIG_FILE "linux-config.properties"

#include "SocketClientLinux.hpp"
#include <unistd.h>
#include <thread>

#endif

class EdubotLib {
    
private:

    //properties
    Properties *properties;
    std::string host;
    int port;

    //connection
    SocketClient *socket;
    
    // robot variables
    std::vector<double> sonars;
    std::vector<bool> bumpers;
    int encoderCountLeft;
    int encoderCountRight;
    int encoderCountDT;
	double x;
	double y;
	double theta;
	std::vector<double> batteryCellsVoltage;

    //functions
    bool toBool(std::string s);
    std::vector<std::string> split(const std::string& s, char delimiter);
    void readReceivedMessage(std::string message);
    bool sendToRobot(std::string text);


public:
    void receiverWorker();
    EdubotLib();
    bool isConnected();
    bool connect();
    bool disconnect();
    bool move(double velocity);
    bool stop();
    bool rotate(double angle);
    bool neutral();
    double getSonar(size_t id);
    bool getBumper(size_t id);
    int getEncoderCountLeft();
    int getEncoderCountRight();
    int getEncoderCountDT();
	double getX();
	double getY();
	double getTheta();
	double getBatteryCellVoltage(size_t id);
    void sleepMilliseconds(int ms);
};

bool EdubotLib::toBool(std::string s) {
     return s == "1" || s == "true" || s == "True" || s == "TRUE";
}

std::vector<std::string> EdubotLib::split(const std::string& s, char delimiter){
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

void EdubotLib::readReceivedMessage(std::string message){

    //std::cout << "received message: " << message << std::endl;

    //message template: 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0,0,0,0,0,0,0,0.0,0.0,0.0

    //slice message
    std::vector<std::string> strArray = this->split(message, ',');

//    for (int i=0; i<strArray.size();i++) {
//        std::cout << i << ": " << strArray.at(i) << std::endl;
//    }

    //validate message 7 sonars + 4 bumpers + 3 encoders info + 3 pose + 3 battery cells voltage = 20
    if(strArray.size() == 20){
		
		try{

			//sonars
			for (int i = 0; i < 7; i++) {
				this->sonars.at(i) = std::stod(strArray.at(i));
			}

			//bumpers
			for (int i = 7; i < 11; i++) {
				this->bumpers.at(i-7) = this->toBool(strArray.at(i));
			}

			//encoder info
			this->encoderCountDT = std::stoi(strArray.at(11));
			this->encoderCountLeft = std::stoi(strArray.at(12));
			this->encoderCountRight = std::stoi(strArray.at(13));
			
			//pose information
			this->x = std::stod(strArray.at(14));
			this->y = std::stod(strArray.at(15));
			this->theta = std::stod(strArray.at(16));
			
			//battery Cells Voltage
			for (int i = 17; i < 20; i++) {
				this->batteryCellsVoltage.at(i-17) = std::stod(strArray.at(i));
			}
			
		}
		catch (int n){
			std::cout << "Error on format data!" << std::endl;
		}

    }

}

bool EdubotLib::sendToRobot(std::string text){

    if(this->socket->isConnected()){

        if (this->socket->sendData(text)) {

            return true;

        }
    }

    return false;
}

void EdubotLib::receiverWorker(){

    while (this->socket->isConnected()) {

        std::string messageStr = this->socket->receiveData();

        if(messageStr.size() > 0){
            this->readReceivedMessage(messageStr);
        }

    }
}

EdubotLib::EdubotLib() {

    this->properties = new Properties(CONFIG_FILE);
    
    if(this->properties->readProperties()){

        //just to test
        //this->properties->printPropertiesMap();

        // initialize connection parameters
        this->host = this->properties->getPropertieValue("robotConnectionHost");
        this->port = std::stoi(this->properties->getPropertieValue("robotConnectionPort"));
        this->socket = new SocketClient();
    }
    else{
        std::cout << "Could not read .properties file." << std::endl;
        exit(0);
    }

    // initialize sonars
    for (int i = 0; i < 7; i++) {
        this->sonars.push_back(0.0);
    }

    // initialize bumpers
    for (int i = 0; i < 4; i++) {
        this->bumpers.push_back(false);
    }

    // initialize encoder information
    this->encoderCountDT = 0;
    this->encoderCountLeft = 0;
    this->encoderCountRight = 0;
	
	//pose information
	this->x = 0.0;
	this->y = 0.0;
	this->theta = 0.0;
	
	// initialize battery cells voltage
    for (int i = 0; i < 3; i++) {
        this->batteryCellsVoltage.push_back(0.0);
    }
}

bool EdubotLib::isConnected() {
    return this->socket->isConnected();
}
    
bool EdubotLib::connect(){

    if(!this->socket->isConnected()){

        if(this->socket->connectToServer(this->host, this->port)){

            std::thread receiverThread(&EdubotLib::receiverWorker, this);
            receiverThread.detach();

            return true;
        }

    }

    return false;
}
    
bool EdubotLib::disconnect(){
    return this->socket->disconnectFromServer();
}

/*
velocity: min = -1.0 and max = 1.0
*/
bool EdubotLib::move(double velocity){

    //message template: v-0.0
    if(velocity >= -1.0 && velocity <= 1.0){
        std::stringstream ss;
        ss << "v" << std::to_string(velocity);
        std::cout << "Sending move command, velocity = " << velocity << std::endl;
        return this->sendToRobot(ss.str());
    }
    else {
        std::cout << "Velocity out of range [-1.0, 1.0]." << std::endl;
    }

    return false;

}
    
bool EdubotLib::stop(){
    //message template: s
    std::cout << "Sending stop command." << std::endl;
    return this->sendToRobot("b");
}
    
/*
angle:
    right: min = 0.0 and max = 180.0
    left: min = -0.0 and max = -180.0
*/
bool EdubotLib::rotate(double angle){
    //message template: r-0.0
    if(angle >= -180.0 && angle <= 180.0){
        std::stringstream ss;
        ss << "r" << std::to_string(angle);
        std::cout << "Sending rotate command, angle = " << angle << std::endl;
        return this->sendToRobot(ss.str());
    }
    else {
        std::cout << "Angle out of range [-180.0, 180.0]." << std::endl;
    }

    return false;
}
    
bool EdubotLib::neutral(){
    //message template: n
    std::cout << "Sending neutral command." << std::endl;
    return this->sendToRobot("n");
}

double EdubotLib::getSonar(size_t id){

    //if(id >= 0 && id < this->sonars.size()){
    if(id < this->sonars.size()){
        return this->sonars.at(id);
    }

    return 0.0;
}

bool EdubotLib::getBumper(size_t id){

    //if(id >= 0 && id < this->bumpers.size()){
    if(id < this->bumpers.size()){
        return this->bumpers.at(id);
    }

    return false;
}

int EdubotLib::getEncoderCountLeft(){

    return this->encoderCountLeft;
}

int EdubotLib::getEncoderCountRight(){

    return this->encoderCountRight;
}

int EdubotLib::getEncoderCountDT(){

    return this->encoderCountDT;
}

double EdubotLib::getX(){

    return this->x;
}


double EdubotLib::getY(){

    return this->y;
}


double EdubotLib::getTheta(){

    return this->theta;
}

double EdubotLib::getBatteryCellVoltage(size_t id){

    //if(id >= 0 && id < this->sonars.size()){
    if(id < this->batteryCellsVoltage.size()){
        return this->batteryCellsVoltage.at(id);
    }

    return 0.0;
}

void EdubotLib::sleepMilliseconds(int ms){
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}
