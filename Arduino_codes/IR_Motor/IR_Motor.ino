#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define IR 12 // D6
#define Motor 13 // D7

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";
HTTPClient httpClient;
WiFiClient wClient;
String URL = "http://10.22.183.248:8000/add-log-sensor";
String device = "21";

void logAttempt(int data){
  if(WiFi.status() == WL_CONNECTED){
    String postData;
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String dayStamp = formattedDate.substring(0, splitT);
    String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    Serial.println(timeStamp);
    String date_up = "\""+dayStamp+" "+timeStamp+"\"";
    Serial.println(date_up);
    postData= "{\"date_\": "+date_up+",\"sensor_id\":"+device+",\"measure\":"+String(data)+"}";
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

  pinMode(Motor, OUTPUT); // Set TRIG_PIN as an output
  pinMode(IR, INPUT);  // Set ECHO_PIN as an input

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
    int move_state = 0;
    int movement_det = digitalRead(IR);
    if(movement_det == 0){
        digitalWrite(Motor, HIGH);
        Serial.println("Movement Detected");
        move_state = 1;
    }else{
        digitalWrite(Motor, LOW);
    }
 logAttempt(move_state);
 delay(1000); // Wait for half a second before the next reading
  
// delay(2000);
}

