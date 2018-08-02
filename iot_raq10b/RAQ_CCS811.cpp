#include "RAQ_CCS811.h"

RAQ_CCS811::RAQ_CCS811() {
  
}

int RAQ_CCS811::init() {
  sensor = new CCS811(CCS811_ADDR);
  
  //Start the sensor.
  int sensorStatus = sensor->begin();

  if (sensorStatus == 0) {
    isConnected = true;
    Serial.println("CCS811 online.");
    return 1;
  } else {
    isConnected = false;
    Serial.println("Problem with CCS811.");
    return 0;
  }
}

int RAQ_CCS811::getCO2() {
  update();
  return isConnected ? co2 : 0;
}

int RAQ_CCS811::getTVOC() {
  update();
  return isConnected ? tvoc : 0;
}

void RAQ_CCS811::update() {
  if (isConnected && sensor->dataAvailable()) {
    sensor->readAlgorithmResults();

    co2  = sensor->getCO2();
    tvoc = sensor->getTVOC();
  }
}
