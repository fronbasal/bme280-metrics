#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

const char* ssid = "";
const char* password = "";
float h, t, p, pin, dp;
char temperatureFString[6];
char dpString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  Wire.begin(D3, D4);
  Wire.setClock(100000);
  Serial.println("[METRICS] Connecting to network");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("/\\");
  }
  Serial.println("[METRICS] Successfully connected to WIFI");
  
  server.begin();
  Serial.println("[METRICS] Waiting for DHCP");
  delay(5000);
  
  Serial.println("[METRICS Device has IP: " + WiFi.localIP());
  Serial.println(F("[METRICS] Testing sensor"));

  if (!bme.begin()) {
    Serial.println("[METRICS] Failed to get sensor. Check wiring.");
    while (1);
  }
}

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("[METRICS] New client");
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection: close");
            client.println();
            client.print("bme280_temperature ");
            client.println(bme.readTemperature());
            client.print("bme280_humidity ");
            client.println(bme.readHumidity());
            client.print("bme280_pressure ");
            client.println(bme.readPressure() / 100.0F);
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("[METRICS] Client disconnected.");
  }
} 
