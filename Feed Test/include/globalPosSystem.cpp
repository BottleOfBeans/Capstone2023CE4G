#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <SPI.h>
#include "config.h"
#include <Adafruit_ADXL345_U.h>

#define GPSECHO false
#define IO_LOOP_DELAY 5000
#define IO_LOOP_DELAY 5000

namespace GPSS
{

  struct GPS_Data {
    int value;
    double lattitude;
    double longitude;
    double elevation;
  };

  Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
  AdafruitIO_Feed *alert = io.feed("seizure-alert");
  AdafruitIO_Feed *locationinfo = io.feed("locationinfo");

  Adafruit_GPS GPS(&Wire);
  uint32_t timer = millis();
  int value = 0;
  double lat = 42.331427;
  double lon = -83.045754;
  double ele = 0;
  unsigned long lastUpdate;
  double convertToDD(double n);

  void callibrateGPS()
  {
    GPS.begin(0x10); // The I2C address to use is 0x10
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    GPS.sendCommand(PGCMD_ANTENNA);
    delay(1000);
    GPS.println(PMTK_Q_RELEASE);
    lat = convertToDD(GPS.latitude);
    lon = convertToDD(-GPS.longitude);
    ele = GPS.altitude;
    Serial.println("GPS Good");
  }

  void calculateLocation()
  {
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    if (GPSECHO)
      if (c)
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived())
    {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
      if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        return;                       // we can fail to parse a sentence in which case we should just wait for another
    }

    value += 1;
    lat = convertToDD(GPS.latitude);
    lon = convertToDD(-GPS.longitude);
    ele = GPS.altitude;

    lastUpdate = millis();
  }

  double convertToDD(double n)
  {
    int deg = (int)n / 100;
    double min = n - deg * 100;
    return deg + min / 60;
  }

  GPS_Data seizureLocation(){
    return GPS_Data{value,lat,lon,ele};
  }

}