#include "RAQ_SensorData.h"

SensorDataList::SensorDataList() {
  
}


bool SensorDataList::addSensorData(int timestamp, float temp, float pressure, float humidity, int co2, int tvoc) {
  SensorData* newSensorData = new SensorData{timestamp, temp, pressure, humidity, co2, tvoc, NULL};
  if (lastItem != NULL) {
    lastItem->next = newSensorData;
  } else {
    firstItem = newSensorData;
    lastItem = newSensorData;
  }
  return true;
}

bool SensorDataList::addSensorData(SensorData* newSensorData, bool last) {
  if (last && lastItem != NULL) {
    lastItem->next = newSensorData;
    lastItem = newSensorData;
  } else if (!last && firstItem != NULL){
    newSensorData->next = firstItem;
    firstItem = newSensorData;
  } else if (firstItem == NULL && lastItem == NULL) {
    lastItem = newSensorData;
    firstItem = newSensorData;
  } else {
    return false;
  }
  return true;
}

SensorData* SensorDataList::getSensorData() {
  if (firstItem != NULL) {
    SensorData* temp = firstItem;
    firstItem = firstItem->next;
    if (firstItem == NULL) {
      lastItem = NULL;
    }
    return temp;
  }
  return NULL;
}

