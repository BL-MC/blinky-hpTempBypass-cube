#include "BlinkyMqttCube.h"
struct
{
  int16_t state;
  int16_t watchdog;
  int16_t relay;
} cubeData;

void subscribeCallback(uint8_t address, int16_t value)
{
  if (address == 2) setRelay();
}

int relayPin = 22;
int relayLedPin = 12;

void setup() 
{
  pinMode(relayPin, OUTPUT);
  pinMode(relayLedPin, OUTPUT);
//  Serial.begin(115200);
  delay(1000);
  BlinkyMqttCube.setChattyCathy(false);
  BlinkyMqttCube.init(2000,true, 10, 100, 16, (int16_t*)& cubeData,  sizeof(cubeData), subscribeCallback);

  cubeData.relay = 0;
  setRelay();
}

void loop() 
{
  BlinkyMqttCube.loop();
}

void setRelay()
{
  digitalWrite(relayPin, cubeData.relay);
  int ledIntensity = 0;
  if (cubeData.relay > 0) ledIntensity = 50;
  analogWrite(relayLedPin, ledIntensity);
}
