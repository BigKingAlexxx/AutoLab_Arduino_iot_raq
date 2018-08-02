#ifndef RAQ_BME280_h
#define RAQ_BME280_h

#include <SparkFunBME280.h>

class RAQ_BME280 {
  private:
    BME280 sensor;
    bool isConnected = false;
    
  public:
    // Constructor
    RAQ_BME280();

    int init();
    
    float getTemp();
    float getPressure();
    float getHumidity();
    float getAltitude();
    
};

#endif
