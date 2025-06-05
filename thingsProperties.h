#ifndef THINGS_PROPERTIES_H
#define THINGS_PROPERTIES_H

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[] = "f8cbb3e8-6305-45b7-aa24-772c7cd8104a";
const char DEVICE_KEY[] = "m7gh5kPRYMDUvOl0UJrHB";  // Özel karakterler kaldırıldı

const char SSID[] = "Wokwi-GUEST";  
const char PASS[] = "";    

void onMesajChange();
void onLedChange();

String mesaj;
CloudTemperatureSensor sicaklik;
CloudRelativeHumidity nem;
bool led;

void initProperties() {
    ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
    ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
    ArduinoCloud.addProperty(mesaj, READWRITE, ON_CHANGE, onMesajChange);
    ArduinoCloud.addProperty(sicaklik, READ, ON_CHANGE, NULL);
    ArduinoCloud.addProperty(nem, READ, ON_CHANGE, NULL);
    ArduinoCloud.addProperty(led, READWRITE, ON_CHANGE, onLedChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

#endif
