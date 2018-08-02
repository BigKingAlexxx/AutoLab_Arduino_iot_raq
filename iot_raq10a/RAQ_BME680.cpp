#include "RAQ_BME680.h"

RAQ_BME680::RAQ_BME680() {
  
}

int RAQ_BME680::init() {
  // Set up oversampling and filter initialization
  sensor.setTemperatureOversampling(BME680_OS_8X);
  sensor.setHumidityOversampling(BME680_OS_2X);
  sensor.setPressureOversampling(BME680_OS_4X);
  sensor.setIIRFilterSize(BME680_FILTER_SIZE_3);
  sensor.setGasHeater(320, 150); // 320*C for 150 ms
  
  //Start the sensor.
  if (sensor.begin()) {
    isConnected = true;
    Serial.println("BME680 online.");
    return 1;
  } else {
    isConnected = false;
    Serial.println("Problem with BME680.");
    return 0;
  }
}

float RAQ_BME680::getTemp() {
  return isConnected ? sensor.readTemperature() : 0.0;
}

float RAQ_BME680::getPressure() {
  return isConnected ? (sensor.readPressure() / 100000) : 0.0;
}

float RAQ_BME680::getHumidity() {
  return isConnected ? sensor.readHumidity() : 0.0;
}

float RAQ_BME680::getAltitude() {
  return isConnected ? sensor.readAltitude(1013.25) : 0.0;    //1013.25 = Sealevelpressure in hPa
}
