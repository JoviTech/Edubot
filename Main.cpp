
//Incluindo a biblioteca iostream para printar no serial.
#include <iostream>
//Incluindo a biblioteca do Ebubot, para realizar a movimentacao do robo.
#include "libs/EdubotLib.hpp"
//Namespace utilizado para facilitar a leitura do codigo.
using namespace std;

//Funcao main
int main(){

//Declaracao das variaveis
	int verdade = 1;
	int angulo;
	double side;

//Inicializacao.
	EdubotLib *edubotLib = new EdubotLib();
	
	if(edubotLib->connect()){
		edubotLib->sleepMilliseconds(2000);
		//Loop principal do programa		
		while(edubotLib->isConnected()){
			//In?cio do movimento			
			edubotLib->move(0.3);				
			cout<< "Moving..." <<endl;	
			cout<< "X: " <<edubotLib->getX();
			cout<< ", Y: " <<edubotLib->getY();
			cout<< ", Theta:" <<edubotLib->getTheta()<<endl;
			//Aqui o robo se move at? que encontre um obst?culo a sua frente, ap?s isso gira ? esquerda e move-se novamente				
			while(verdade > 0){			
				if(edubotLib->getSonar(3)<0.060){
					edubotLib->rotate(-90);
					edubotLib->sleepMilliseconds(2000);
					verdade = 0;
					edubotLib->move(0.3);
					edubotLib->sleepMilliseconds(500);
					cout<< "Moving..." <<endl;
				}
			}	
		     //Condi??o usando os Bumpers para verificar se o rob? bateu
		    	if(edubotLib->getBumper(0) == 1 || edubotLib->getBumper(1) == 1){
		    		//Se houve colis?o frontal, printa no serial, move-se para tras e ajeita o angulo para rotacionar.
		     	cout<< "Crashed." <<endl;
		     	edubotLib->move(-0.3);
		     	edubotLib->sleepMilliseconds(400);
		     	edubotLib->stop();
		     	edubotLib->sleepMilliseconds(1000);
		     	angulo = edubotLib->getTheta();
		     	angulo = angulo%90;
		     	edubotLib->rotate(-angulo);	
		     	edubotLib->sleepMilliseconds(3000);		     				     				     		     			
		     }
		     if(edubotLib->getBumper(2) == 1 || edubotLib->getBumper(3) == 1){
		     	//Se houve colisao traseira, printa no serial, move-se para frente e ajeita o angulo para rotacionar.
		     	cout<< "Crashed." <<endl;
		     	edubotLib->move(0.3);
		     	edubotLib->sleepMilliseconds(400);
		     	edubotLib->stop();
		     	edubotLib->sleepMilliseconds(1000);		     	
		     	angulo = edubotLib->getTheta();
		     	angulo = angulo%90;
		     	edubotLib->rotate(-angulo);
		     	edubotLib->sleepMilliseconds(3000);		     			     				     				     		     			
		     }
		     
			cout<< "Moving..." <<endl;
			//Aqui o rob? fica seguindo a parede at? que encontre um obstaculo a frente ou n?o encontre mais a parede de refer?ncia.
			do{	
				//Nesse if verifica se o rob? saiu do laibirinto, usando todos os Sonares, se saiu para e desconecta.
				if(edubotLib->getSonar(0)==2.0 && edubotLib->getSonar(1)==2.0 && edubotLib->getSonar(2)==2.0 && edubotLib->getSonar(3)==2.0 && edubotLib->getSonar(4)==2.0 && edubotLib->getSonar(5)==2.0 && edubotLib->getSonar(6)==2.0){
					cout<< "Out of the maze" <<endl;
					edubotLib->sleepMilliseconds(2000);
					edubotLib->stop();
					edubotLib->disconnect();
				}						
				side = edubotLib->getSonar(6);
			}while(side<0.1 && edubotLib->getSonar(3)>0.065);
			
			cout<< "Theta:" <<edubotLib->getTheta()<<endl;
			edubotLib->stop();
			cout<< "Rotating..." <<endl;
			//Se o rob? encontra algo a frente h? duas condi??es:
			if(edubotLib->getSonar(3)<0.065){
				cout<< "Central sonar" <<endl;
				//Se ainda h? a parede de referencia q o rob? segue, gira para a esquerda
				if(edubotLib->getSonar(6)<0.1){
					edubotLib->rotate(-90);
					edubotLib->sleepMilliseconds(2000);
				}
				//Se n?o, gira pra direita
				else{
					edubotLib->rotate(90);
					edubotLib->sleepMilliseconds(2000);
				}
			}
			//Se o rob? n?o encontrou nenhum obstaculo a frente, ou seja, parou de se mover pois n?o h? mais a parede de refer?ncia.
			else{
				cout<< "Side sonar" <<endl;
				//Anda por mais 500 ms
				edubotLib->sleepMilliseconds(500);
				//Se h? uma parede na frente:
				if(edubotLib->getSonar(3)<1.0){
					cout<< "Wall nearby: " <<endl;
					//Move-se at? que encontre essa parede
					edubotLib->move(0.3);
					do{
						
					}while(edubotLib->getSonar(3)<0.065);
					edubotLib->sleepMilliseconds(500);
				}
				//Se n?o h? uma parede a frente:				
				else{	
					cout<< "Without wall nearby: " <<endl;				
					edubotLib->sleepMilliseconds(500);
					//Move-se por mais 690ms
					edubotLib->move(0.3);								
					edubotLib->sleepMilliseconds(690);	
				}
				//Rotaciona para a direita para continuar seguindo a parede de refer?ncia.			
				edubotLib->rotate(90);
				edubotLib->sleepMilliseconds(2000);
				edubotLib->move(0.3);								
				edubotLib->sleepMilliseconds(1000);					
			}
		}
	}	
	else{
		cout<< "Could not connect on robot" <<endl;
	}
 
	return 0;
}
