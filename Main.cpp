/*
 * EDUBOT HELLO WORLD EXAMPLE
 * @Author: Maik Basso <maik@maikbasso.com.br>
*/

#include <iostream>
#include "libs/EdubotLib.hpp"

int main(){

	EdubotLib *edubotLib = new EdubotLib();

	//try to connect on robot
	if(edubotLib->connect()){

		// shows edubot sensors
		int showSensorsTimes = 10;
		while (showSensorsTimes > 0) {

			// Waits for two seconds
			edubotLib->sleepMilliseconds(2000);

			// sonars
			for (int i=0; i<7; i++) {
				std::cout << "S" << i << ": " << edubotLib->getSonar(i) << "m, ";
			}

			// bumpers
			for (int i=0; i<4; i++) {
				std::cout << "B" << i << ": " << (edubotLib->getBumper(i) == true? "true":"false") << ", ";
			}

			std::cout << "leftcount: " << edubotLib->getEncoderCountLeft() << ", ";
			std::cout << "rightcount: " << edubotLib->getEncoderCountRight() << ", ";
			std::cout << "dt(looptime): " << edubotLib->getEncoderCountDT() << ", ";
			
			std::cout << "x: " << edubotLib->getX() << ", ";
			std::cout << "y: " << edubotLib->getY() << ", ";
			std::cout << "theta: " << edubotLib->getTheta() << ", ";
			
			std::cout << "battCell0: " << edubotLib->getBatteryCellVoltage(0) << ", ";
			std::cout << "battCell1: " << edubotLib->getBatteryCellVoltage(1) << ", ";
			std::cout << "battCell2: " << edubotLib->getBatteryCellVoltage(2);

			// line break
			std::cout << std::endl;

			showSensorsTimes--;

		}

		while(edubotLib->connect()){

			if(getSonar(6)<0.2 && getSonar(3)>0.2){
				edubotLib->move(0.3);
			}else{
				if(getSonar(6)>0.2){
					edubotLib->rotate(90);
				}
				if(getSonar(3)<0.2){
					edubotLib->rotate(-90);
				}
			}
			
		}
		

		/*
		 * Function move
		 * Parameter velocity (-1.0, 1.0)
		 * Positive values move forward, negative values move backward
		*/
		edubotLib->move(0.3);
		// Waits for two seconds for processes to reflect on the robot
		edubotLib->sleepMilliseconds(2000);

		/*
		 * Function rotate
		 * Parameter angle (-180, 180)
		 * Positive values rotate right, negative values rotate left
		*/
		edubotLib->rotate(90);
		// Waits for two seconds for processes to reflect on the robot
		edubotLib->sleepMilliseconds(2000);

		/*
		 * Function neutral
		 * put the motors in the neutral state
		*/
		edubotLib->neutral();
		// Waits for two seconds for processes to reflect on the robot
		edubotLib->sleepMilliseconds(2000);

		/*
		 * Function stop
		 * Stop the motors / break
		*/
		edubotLib->stop();
		// Waits for two seconds for processes to reflect on the robot
		edubotLib->sleepMilliseconds(2000);

		/*
		 * Function disconnect
		 * disconnect from robot
		 * Important to user EVER
		*/
		edubotLib->disconnect();
	}
	else{
		std::cout << "Could not connect on robot!" << std::endl;
	}

	return 0;
}
