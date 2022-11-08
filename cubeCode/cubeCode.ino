
const char* ssid = "Tele2_2BA356";
const char* wifiPassword = "";
const char* mqttServer = "hip-lifeguard.cloudmqtt.com";
const char* mqttUsername = "";
const char* mqttPassword = "";
const char* mqttPublishTopic = "blinky-hpTempBypass/01/reading";
const char* mqttSubscribeTopic = "blinky-hpTempBypass/01/setting";


#define BLINKYMQTTBUSBUFSIZE  3
union BlinkyBusUnion
{
  struct
  {
    int16_t state;
    int16_t watchdog;
    int16_t relay;
  };
  int16_t buffer[BLINKYMQTTBUSBUFSIZE];
} blinkyBus;
void subscribeCallback(uint8_t address, int16_t value)
{
  if (address == 2) setRelay();
}

#include "blinky-wifiMqtt-cube.h"
int relayPin = 22;
int wirelessConnectedPin = 10;
int relayLedPin = 12;
unsigned long tnow;
unsigned long lastWirelessBlink;
boolean wirelessLed = false;


void setup() 
{
  pinMode(relayPin, OUTPUT);
  pinMode(wirelessConnectedPin, OUTPUT);
  pinMode(relayLedPin, OUTPUT);
  digitalWrite(wirelessConnectedPin, 1);
  Serial.begin(115200);
  delay(5000);
  initBlinkyBus(2000,true, 11);

  blinkyBus.relay = 0;
  setRelay();
  digitalWrite(wirelessConnectedPin, 0);
  tnow = millis();
  lastWirelessBlink = tnow;
}

void loop() 
{
  tnow = millis();
  blinkyBusLoop();
//  publishBlinkyBusNow(); 
  if (g_wifiStatus == WL_CONNECTED)
  {
    wirelessLed = true;
    digitalWrite(wirelessConnectedPin, wirelessLed);
  }
  else
  {
    if ((tnow - lastWirelessBlink) > 1000)
    {
      wirelessLed = !wirelessLed;
      lastWirelessBlink = tnow;
      digitalWrite(wirelessConnectedPin, wirelessLed);
    }
  }

}
void setRelay()
{
  digitalWrite(relayPin, blinkyBus.relay);
  digitalWrite(relayLedPin, blinkyBus.relay);
}
