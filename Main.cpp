

#include <iostream>
#include "libs/EdubotLib.hpp"

#define N_SONARES 7
#define N_BUMPERS 4
#define N_CELULAS 3
using namespace std;
int main(){
	int i;
	double posX;
	double posY;
	double sonares[N_SONARES];
	
	EdubotLib *edubotLib = new EdubotLib();

	//try to connect on robot
	if(edubotLib->connect()){
		while(edubotLib->isConnected()){
			cout<<"---"<<endl;
			for(i =0; i<N_SONARES; i++){
				sonares[i] = edubotLib->getSonar(i);
				cout<<"Sonar "<<i<<": "<<sonares[i]<<"m, ";
		}
		cout<<endl;
		for(i =0; i<N_BUMPERS; i++){
			cout<<"Bumper "<< i << ": "<<edubotLib->getBumper(i);	
		}
		cout<<endl;

		cout<<"X: "<<edubotLib->getX();
		cout<<", Y: "<<edubotLib->getY();
		cout<<", Theta: "<<edubotLib->getTheta()<<endl;

		posX = edubotLib->getX();

		for(i = 0; i<N_CELULAS; i++){
			cout<<"Celula "<<i<<": "<<edubotLib->getBatteryCellVoltage(i)<<", ";	
		}
		cout<<endl;
		
		cout<<"Encoder Esq: "<<edubotLib->getEncoderCountLeft()<< ", ";
		cout<<"Encoder Dir: "<<edubotLib->getEncoderCountRight()<<", ";
		cout<<"Dt: "<<edubotLib->getEncoderCountDT()<<endl;

		
		
		edubotLib->sleepMilliseconds(1000);		}
		edubotLib->disconnect();
	}
	else{
		std::cout << "Could not connect on robot!" << std::endl;
	}

	return 0;
}
