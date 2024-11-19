#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 12 // GPIO5
#define ECHO_PIN 13 // GPIO4
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid = "RoBorregos2";
const char* password = "RoBorregos2024";
HTTPClient httpClient;
WiFiClient wClient;
String URL = "http://192.168.1.102:8000/add-log-sensor";
String device = "5";

void logAttempt(int data){
  if(WiFi.status() == WL_CONNECTED){
    String postData;
    String datas;
    datas= String(data);
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String dayStamp = formattedDate.substring(0, splitT);
    String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    Serial.println(timeStamp);
    String date_up = "\""+dayStamp+" "+timeStamp+"\"";
    Serial.println(date_up);
    postData= "{\"date_\": "+date_up+",\"sensor_id\":"+device+",\"measure\":"+datas+"}";
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

  pinMode(TRIG_PIN, OUTPUT); // Set TRIG_PIN as an output
  pinMode(ECHO_PIN, INPUT);  // Set ECHO_PIN as an input
  lcd.init();
  lcd.backlight(); // Turn on the backlight
  lcd.clear();

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
long duration, distance;
 digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the duration of the echo pulse
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in cm
  distance = duration * 0.034 / 2; // Speed of sound is 343 m/s, converted to cm/us
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance < 10){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALERTA!");
    lcd.setCursor(0, 1);
    lcd.print("PERSONA CERCA");
    delay(2000);
    lcd.clear();
  }
 logAttempt(distance);
  delay(1000); // Wait for half a second before the next reading
  
// delay(2000);
}

