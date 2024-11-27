#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoOTA.h>

#define TRIG_PIN 12 // GPIO6
#define ECHO_PIN 13 // GPIO7
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address 0x27, 16 columns, 2 rows

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
const char* ssid = "RoBorregos2";
const char* password = "RoBorregos2024";
HTTPClient httpClient;
WiFiClient wClient;
String URL = "http://fast-api-reto.onrender.com/add-log-sensor";
String device = "6";



void start_ota_update(){
  ArduinoOTA.setHostname("esp_lcdultra");
  ArduinoOTA.setPassword("esplcd01");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}



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
  start_ota_update();
  start_ota_update();
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
  ArduinoOTA.handle();
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
 ArduinoOTA.handle();
  delay(1000); // Wait for half a second before the next reading
  ArduinoOTA.handle();
// delay(2000);
}

