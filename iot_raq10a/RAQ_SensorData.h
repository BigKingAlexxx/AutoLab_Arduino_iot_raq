#ifndef RAQ_SENSORDATA_h
#define RAQ_SENSORDATA_h

#include <Arduino.h>

struct SensorData {
  int timestamp;
  float temp;
  float pressure;
  float humidity;
  int co2;
  int tvoc;
  SensorData* next;
};

class SensorDataList {
  private:
    SensorData* firstItem;
    SensorData* lastItem;

  public:
    // Constructor
    SensorDataList();
    bool addSensorData(int timestamp, float temp, float pressure, float humidity, int co2, int tvoc);
    bool addSensorData(SensorData*, bool last);
    SensorData* getSensorData();
};

#endif
