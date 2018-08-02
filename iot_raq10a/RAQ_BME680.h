#include <bme680.h>
#include <bme680_defs.h>

#ifndef RAQ_BME680_h
#define RAQ_BME680_h

#include <Adafruit_BME680.h>

class RAQ_BME680 {
  private:
    Adafruit_BME680 sensor;
    bool isConnected = false;
    
  public:
    // Constructor
    RAQ_BME680();

    int init();
    
    float getTemp();
    float getPressure();
    float getHumidity();
    float getAltitude();
};

#endif
