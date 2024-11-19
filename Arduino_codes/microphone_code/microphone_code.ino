#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid = "RoBorregos2";
const char* password = "RoBorregos2024";
HTTPClient httpClient;
WiFiClient wClient;
String URL = "http://192.168.1.102:8000/add-log-sensor";
const int micPin = 14;

void logAttempt(int decibel){
  if(WiFi.status() == WL_CONNECTED){
    String device = "16";
    String postData;
    String decibels;
    decibels= String(decibel);
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String dayStamp = formattedDate.substring(0, splitT);
    String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    Serial.println(timeStamp);
    String date_up = "\""+dayStamp+" "+timeStamp+"\"";
    Serial.println(date_up);
    postData= "{\"date_\": "+date_up+",\"sensor_id\":"+device+",\"measure\":"+decibel+"}";
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
pinMode(micPin, INPUT);
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
int dato = map(analogRead(A0), 500, 650, 0,100);
Serial.println(dato);
logAttempt(dato);
delay(2000);
}

