#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_gfx.h>
#include <Adafruit_GPS.h>
#include <SPI.h>
#include <accelerometer.cpp>
#include <globalPosSystem.cpp>
#include <feeds.cpp>

struct GPS_Data {
  int value;
  double lattitude;
  double longitude;
  double elevation;
};

void setup() {

  Serial.begin(9600);
  
 

  //SERIAL
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  accelerometer::accelerometerSetup();
  GPSS::callibrateGPS();
  feeds::callibrateIO();
  Serial.println("Everything good");
}

void loop() {
  // Start Inputting things here.
  accelerometer::debugSeizure();
  if(accelerometer::checkForSeizure()){
    GPSS::calculateLocation();
    GPSS::GPS_Data currentData = GPSS::seizureLocation(); 
    feeds::sendLocation(currentData.value, currentData.lattitude, currentData.longitude, currentData.elevation);
    feeds::seizureAlert();
  }else{
    feeds::seizureSafe();
  }
  delay(100);
}