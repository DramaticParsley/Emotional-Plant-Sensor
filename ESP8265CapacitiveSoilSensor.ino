#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266HTTPClient.h>

const int LOGGER_CHANNEL  = 115200;

const char* ssid     = "";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "";     // The password of the Wi-Fi network

const String localApiEndpoint  = "http://backend_url/soilmoisture";
const String httpVariableValue  = "value=";

HTTPClient http;
WiFiClient client;

void setup() {
  setupLogger(LOGGER_CHANNEL);
  connectWifi(ssid, password);
  makeSensorHttpCall();
  Serial.println("!!!Going to sleep for seconds = " );
  Serial.println(3600);
  ESP.deepSleep(3600e6);
  Serial.println("Does it do anything after deep sleep");
}


void loop() {
}

void setupLogger(int loggerChannel) {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  Serial.println('\n');
  while(!Serial) { }
}

void connectWifi(String user, String pass) {
  WiFi.begin(user, pass);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(user); 
  Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void makeSensorHttpCall() {
  Serial.println('\n');
  Serial.println("Starting http call!");  
  int val;
  
  val = analogRead(A0); //connect sensor to Analog 0
  Serial.println(val); //print the value to serial port

  // Make a HTTP request:
  http.begin(client, localApiEndpoint + "?" + httpVariableValue + val);
  int code = http.POST(" ");

  if (code > 0) {
    String payload = http.getString();
    Serial.println(payload);
  }
  http.end();
  client.stop();
  Serial.println("Finished making the HTTP Call");

}
