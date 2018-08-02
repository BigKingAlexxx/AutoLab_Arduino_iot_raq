#ifndef RAQ_CCS811_h
#define RAQ_CCS811_h


#include <Arduino.h>
#include "SparkFunCCS811.h"

#define CCS811_ADDR 0x5B

class RAQ_CCS811 {
  private:
    CCS811* sensor;
    bool isConnected = false;
    int tvoc = 0.0;
    int co2 = 0.0;

    void update();
    
  public:
    // Constructor
    RAQ_CCS811();

    int init();
    
    int getCO2();
    int getTVOC();
    
};

#endif
