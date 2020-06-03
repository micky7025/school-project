#include <OneWire.h>
#include <DallasTemperature.h>
const int oneWireBus = 4; 


OneWire oneWire(pin);
DallasTemperature sensors(&oneWire); //call DataBase

void setup (void)
  {
    Serial.begin(9600);
    sensors.begin();
  }


void loop (void)
  {
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0));
    delay(2000);
  }
