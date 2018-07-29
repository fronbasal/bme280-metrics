#include "ESP8266WiFi.h"

const char* ssid = "foobar";

void setup(void)
{ 
  Serial.begin(9600);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.print("[METRICS] UP WITH IP ");
  Serial.println(WiFi.localIP());

}
void loop() {}


