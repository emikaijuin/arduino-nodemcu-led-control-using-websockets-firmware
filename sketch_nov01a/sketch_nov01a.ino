#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>


// Initialize pins
int redpin = D0;
int greenpin = D2;
int bluepin = D4;

//// Connecting to the internet
const char* ssid = "YOUR WIFI";
const char* password = "YOUR PASSWORD";

// Setting up the websocket client
WebSocketsClient webSocket;

void setup() {
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10);
  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED) {
    Serial.print(".");
    delay(500);  
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());

  // Initializing the WS2812B communication
  setRgb(255,80,90);

  // Initializing the websocket connection
  webSocket.begin("nodemcu-led-lights.herokuapp.com", 80, "/subscribe");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  
}
void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();

} 

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch(type) {
    Serial.write(type);
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("[WSc] Connected to url: %s\n", payload);
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] Received text: %s\n", payload);
      DynamicJsonBuffer jBuffer;
      JsonObject &root = jBuffer.parseObject(payload);
      setRgb(root["r"],root["g"],root["b"]);
      break;
  }
}

  void setRgb(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(redpin, r);
  analogWrite(bluepin, b);
  analogWrite(greenpin, g);
  delay(10);
}
