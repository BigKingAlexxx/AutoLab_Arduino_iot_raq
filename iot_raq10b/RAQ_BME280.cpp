#include "RAQ_BME280.h"

RAQ_BME280::RAQ_BME280() {
  
}

int RAQ_BME280::init() {
  //***Driver settings********************************//
  //commInterface can be I2C_MODE
  //specify I2C address.  Can be 0x77(default) or 0x76

  //For I2C, enable the following
  sensor.settings.commInterface = I2C_MODE;
  sensor.settings.I2CAddress = 0x77;

  //***Operation settings*****************************//

  //runMode can be:
  //  0, Sleep mode
  //  1 or 2, Forced mode
  //  3, Normal mode
  sensor.settings.runMode = 3; //Forced mode

  //tStandby can be:
  //  0, 0.5ms
  //  1, 62.5ms
  //  2, 125ms
  //  3, 250ms
  //  4, 500ms
  //  5, 1000ms
  //  6, 10ms
  //  7, 20ms
  sensor.settings.tStandby = 0;

  //filter can be off or number of FIR coefficients to use:
  //  0, filter off
  //  1, coefficients = 2
  //  2, coefficients = 4
  //  3, coefficients = 8
  //  4, coefficients = 16
  sensor.settings.filter = 0;

  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  sensor.settings.tempOverSample = 1;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  sensor.settings.pressOverSample = 1;

  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  sensor.settings.humidOverSample = 1;

  //Start the sensor.
  int sensorID = sensor.begin();

  if (sensorID == 0x60) {
    isConnected = true;
    Serial.println("BME280 online.");
    return 1;
  } else {
    isConnected = false;
    Serial.println("Problem with BME280.");
    return 0;
  }
}

float RAQ_BME280::getTemp() {
  return isConnected ? sensor.readTempC() : 0.0;
}

float RAQ_BME280::getPressure() {
  return isConnected ? (sensor.readFloatPressure() / 100000) : 0.0;
}

float RAQ_BME280::getHumidity() {
  return isConnected ? sensor.readFloatHumidity() : 0.0;
}

float RAQ_BME280::getAltitude() {
  return isConnected ? sensor.readFloatAltitudeMeters() : 0.0;
}

