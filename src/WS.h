#pragma once
#ifdef SENSORS
#include <ArduinoJson.h>

namespace WS
{
    void Setup();
    void ConnectToWifi();
    void SerialReport();
    void Loop();
    bool SendDiscovery();
}

#endif
