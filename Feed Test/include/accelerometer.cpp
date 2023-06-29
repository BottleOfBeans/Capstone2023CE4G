#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "config.h"

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

bool seizureAlert = false;
int counter = 0;
int lackThereof = 0;
int cycle = 0;

namespace accelerometer
{

    void accelerometerSetup()
    {
        /* Initialise the sensor */
        if (!accel.begin())
        {
            /* There was a problem detecting the ADXL345 ... check your connections */
            Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
            while (1)
                ;
        }
        Serial.println("Accel Good");
    }

    bool checkForSeizure()
    {
        Serial.print("Updating:    ");
        // Creating the acceleration event
        sensors_event_t accelEvent;
        accel.getEvent(&accelEvent);

        // Getting the acceleration values on x,y,z
        float x = accelEvent.acceleration.x;
        float y = accelEvent.acceleration.y;
        float z = accelEvent.acceleration.z;

        // Calculating magnitude of Vector3
        float magnitude = sqrt(x * x + y * y + z * z);

        // If accel event magnitude if high enough, then add to counter
        if (magnitude > 20 && !seizureAlert)
        {
            counter++;
        } // If accel event magnitude is not high enough and seizure is active, then disable seizure
        else if (seizureAlert && magnitude <= 20)
        {
            seizureAlert = false;
        } // If just magnitude if not high enough then add to the lack of counter
        else if (magnitude <= 20)
        {
            lackThereof++;
        } // If lack of counter is high enough, recognize peak in acceleration pattern and record cycle
        if (lackThereof >= 20)
        {
            counter = 0;
            lackThereof = 0;
            counter = 0;
            cycle++;
        }

        // Once enough cycles are observed then send out the information to the feed
        if (cycle >= 3)
        {
            Serial.println("SEIZURE!?");
            seizureAlert = true;
            counter = 0;
            cycle = 0;
            return true;
        }
        return false;
    }

    void debugSeizure()
    {
        Serial.print(counter);
        Serial.print("     ");
        Serial.print(lackThereof);
        Serial.print("     ");
        Serial.println(cycle);
    }
}