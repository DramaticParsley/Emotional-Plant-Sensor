#include <WiFi.h>
#include <HTTPClient.h>
#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
#define TIME_TO_SLEEP 5 /* Time ESP32 will go to sleep (in seconds) */


const int LOGGER_CHANNEL  = 115200;

const char* ssid     = "wifi name";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "wifi password";     // The password of the Wi-Fi network

const String sensorName       = "Your Sensor Name";
const String localApiEndpoint  = "http://yourbackend/soilmoisture";


void setup() {
  setupSerial();
  setupWifiConnection();
  //checkingPins();
  generateResponseBody();
  uploadSensorData();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}


void loop() {
}

void setupSerial() {
  Serial.begin(LOGGER_CHANNEL);
}

void checkingPins() {
  Serial.printf("Printing the A0: ");
  Serial.println(analogRead(A0));
  
  Serial.printf("Printing the A1: ");
  Serial.println(analogRead(A1));
  
  Serial.printf("Printing the A2: ");
  Serial.println(analogRead(A2));
  
  Serial.printf("Printing the A3: ");
  Serial.println(analogRead(A3));
  
  Serial.printf("Printing the A4: ");
  Serial.println(analogRead(A4));
  
  Serial.printf("Printing the A5: ");
  Serial.println(analogRead(A5));
}

void setupWifiConnection() {
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
}

void uploadSensorData() {
  HTTPClient http;
  http.begin(localApiEndpoint);
  http.addHeader("Content-Type", "application/json");
  String body = generateResponseBody();
//  Serial.printf("Printing the body : ");
//  Serial.println(body);
  int httpResponseCode = http.POST(body);

  http.end();
}


String generateResponseBody() {
  String body = "{\"name\":\"";
  body += sensorName;
  body += "\", \"value\":" ;
  body += analogRead(A2);
  body += "}";
  return body;
}
