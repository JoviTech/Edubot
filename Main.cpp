/*
 * EDUBOT HELLO WORLD EXAMPLE
 * @Author: Maik Basso <maik@maikbasso.com.br>
 */

#include <iostream>

#include "libs/EdubotLib.hpp"

#define MIN_PAREDE 0.04
#define VELOCIDADE 0.4
#define TEMPO 10000

int main() {
  int a = 0;
  EdubotLib *edubotLib = new EdubotLib();

  // try to connect on robot
  if (edubotLib->connect()) a = 1;
  if (a) {
    // shows edubot sensors
    int showSensorsTimes = 1;
    int conta_gyro = 0;

    while (a) {
      std::cout << "Teste ";
      while (edubotLib->getSonar(6) <= MIN_PAREDE &&
             edubotLib->getSonar(3) >= MIN_PAREDE) {
        edubotLib->move(VELOCIDADE);
        edubotLib->sleepMilliseconds(TEMPO);
      }

      if (edubotLib->getSonar(6) <= MIN_PAREDE &&
          edubotLib->getSonar(3) <= MIN_PAREDE) {
        edubotLib->rotate(-90);
        edubotLib->sleepMilliseconds(TEMPO);
        conta_gyro = conta_gyro - 1;
      }

      if (edubotLib->getSonar(6) >= MIN_PAREDE) {
        edubotLib->rotate(90);
        edubotLib->sleepMilliseconds(TEMPO);
        conta_gyro = conta_gyro + 1;
        edubotLib->move(VELOCIDADE);
        edubotLib->sleepMilliseconds(TEMPO);
      }
    }
    while (showSensorsTimes > 0) {
      // Waits for two seconds
      edubotLib->sleepMilliseconds(2000);

      // sonars
      for (int i = 0; i < 7; i++) {
        std::cout << "S" << i << ": " << edubotLib->getSonar(i) << "m, ";
      }

      // bumpers
      for (int i = 0; i < 4; i++) {
        std::cout << "B" << i << ": "
                  << (edubotLib->getBumper(i) == true ? "true" : "false")
                  << ", ";
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

    // edubotLib->disconnect();
  } else {
    std::cout << "Could not connect on robot!" << std::endl;
  }

  return 0;
}
