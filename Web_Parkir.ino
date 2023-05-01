// Import required libraries
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char* ssid = "WiFi Parkir";
const char* password = "wifiparkir";

//int dist1 = 0;//
//int dist2 = 0;//
String ST = "  AVAILABLE";
String DV = "  AVAILABLE";

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Mengganti Placeholder pada HTML dengan ketersediaan parkir
String processor(const String& var){
  //Kondisional tempat parkir
  if(0.01723 * readUltrasonicDistance(13, 12)<6){
      ST = "UNAVAILABLE";
    }
    else{
      ST = "  AVAILABLE";
    }
  if(0.01723 * readUltrasonicDistance(14, 27)<6){
      DV = "UNAVAILABLE";
    }
    else{
      DV = "  AVAILABLE";
    }
  if(var == "PROGBAR"){
    if(ST=="  AVAILABLE" && DV=="  AVAILABLE"){
      return String(0);
    }
    else if (ST=="  AVAILABLE" && DV=="UNAVAILABLE"){
      return String(50);
    }
    else if (DV=="  AVAILABLE" && ST=="UNAVAILABLE"){
      return String(50);
    }
    else{
      return String(100);
    }
  }
  if(var == "SLOTONE"){
      return String(ST);
    }
  if(var == "SLOTTWO"){
      return String(DV);
    }
  return String();
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Start server
  server.begin();
}
 
void loop(){
}
