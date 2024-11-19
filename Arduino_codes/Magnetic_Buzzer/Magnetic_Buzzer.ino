#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define MAG 12 // D6
#define BUZZER 13 // D7

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid = "RoBorregos2";
const char* password = "RoBorregos2024";
HTTPClient httpClient;
WiFiClient wClient;
String URL = "http://192.168.1.102:8000/add-log-sensor";
String device = "6";

void logAttempt(String data){
  if(WiFi.status() == WL_CONNECTED){
    String postData;
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String dayStamp = formattedDate.substring(0, splitT);
    String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    Serial.println(timeStamp);
    String date_up = "\""+dayStamp+" "+timeStamp+"\"";
    Serial.println(date_up);
    postData= "{\"date_\": "+date_up+",\"sensor_id\":"+device+",\"measure\":"+data+"}";
    Serial.print("Post Data String: ");
    Serial.println(postData);
    Serial.print("Host: ");
    Serial.println(URL);
    httpClient.begin(wClient, URL);
    httpClient.addHeader("Content-Type", "application/json");
    int httpCode = httpClient.POST(postData);
    Serial.print("Response Code: ");
    Serial.println(httpCode);
 
    httpClient.end();
  }
  return;
}

void setup(){
Serial.begin(115200);

  pinMode(BUZZER, OUTPUT); // Set TRIG_PIN as an output
  pinMode(MAG, INPUT);  // Set ECHO_PIN as an input

 WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi net \"");
  Serial.print(ssid);
  Serial.print("\"");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected! IP: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  delay(500);
}
void loop(){
    timeClient.update();
    String door_state = "door_closed";
    int distance = digitalRead(MAG);
    if(distance == 0){
        digitalWrite(BUZZER, HIGH);
        Serial.println("Puerta Abierta");
        door_state = "door_open";
    }else{
        digitalWrite(BUZZER, LOW);
    }
 logAttempt(door_state);
 delay(1000); // Wait for half a second before the next reading
  
// delay(2000);
}

