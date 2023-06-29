#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GPS.h>
#include <SPI.h>
#include "config.h"


AdafruitIO_Feed *alert = io.feed("seizure-alert");
AdafruitIO_Feed *locationInfo = io.feed("locationinfo");

namespace feeds
{
    void callibrateIO()
    {
        io.connect();
        while (io.status() < AIO_CONNECTED)
        {
            Serial.print(".");
            delay(100);
        }
        // CONNECTED! :)
        Serial.println();
        Serial.println(io.statusText());
        Serial.println("Feeds Good");
    }
    void seizureAlert()
    {
        alert->save(1);
    }
    void seizureSafe()
    {
        alert->save(0);
    }
    void sendLocation(int value, double lat, double lon, double ele)
    {
        locationInfo->save(value, lat, lon, ele);
    }
}