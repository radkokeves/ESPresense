#ifdef SENSORS
#include "WS.h"

#include "globals.h"
#include "mqtt.h"
#include "defaults.h"
#include <AsyncWiFiSettings.h>
#include <AsyncMqttClient.h>
#include "string_utils.h"

namespace WS
{
    int powerPin = 17;
    int signalPin = 35;
    int value = 0;
    //int gain = 1;
    int sensorInterval = 5000;
    unsigned long lastMillis = 0;

    void Setup()
    {
        if (!powerPin && !signalPin) return;
        pinMode(powerPin, OUTPUT);  // configure pin as an OUTPUT
        digitalWrite(powerPin, LOW); // turn the sensor OFF  
    }

    void ConnectToWifi()
    {
        AsyncWiFiSettings.html("h4", "WS - Watter Sensor:");
        powerPin = AsyncWiFiSettings.integer("WS_powerPin", 0, "Watter Sensor power pin");
        signalPin = AsyncWiFiSettings.integer("WS_signalPin", 0, "Watter Sensor signal pin");
    }

    void SerialReport()
    {
        if (!powerPin && !signalPin) return;
        Serial.print("HX711 Weight Sensor: ");
        Serial.println(String(powerPin) + "/" + String(signalPin));
    }

    void Loop()
    {
        if (!powerPin && !signalPin) return;
        if (millis() - lastMillis < sensorInterval) return;
        if (digitalRead(powerPin) != LOW)
        {
            //Serial1.println("WS - Watter Sensor: Not ready yet");
            //Watter sensor is not ready for new measurements yet!
            return;
        }

        lastMillis = millis();

        uint32_t data = 0;

        for (uint8_t i = 0; i < 24; i++)
        {
            digitalWrite(powerPin, HIGH);
            delayMicroseconds(1);
            value = analogRead(signalPin); // read the analog value from sensor
            digitalWrite(powerPin, LOW);   // turn the sensor OFF
            delayMicroseconds(1);
        }

        pub((roomsTopic + "/watter_level").c_str(), 0, true, String(value).c_str());
    }

    bool SendDiscovery()
    {
        if (!powerPin && !signalPin) return true;
        return sendSensorDiscovery("Watter level", EC_NONE);
    }
}

#endif
