#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

Adafruit_BMP280 bmp; // Create an instance of the BMP280 library

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize the BMP280 library
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  
  // Send the data to the website using an HTTP request
  WiFiClient client;
  if (client.connect("www.example.com", 80)) {
    String request = "GET /update_data?temperature=" + String(temperature) + "&pressure=" + String(pressure) + " HTTP/1.1\r\n";
    request += "Host: www.example.com\r\n";
    request += "Connection: close\r\n\r\n";
    client.print(request);
    Serial.println("Data sent to website");
  }
  else {
    Serial.println("Could not connect to website");
  }
  
  delay(5000);
}
