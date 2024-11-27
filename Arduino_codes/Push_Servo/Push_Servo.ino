// 180 para cerrar
// 0 para abrir
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#define SERVO_PIN 12
#define BUTTON_PIN 13
Servo servo;  // create servo object to control a servo  // variable to store the servo position


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "2.mx.pool.ntp.org", -21600, 60000);


// const char* ssid = "RoBorregos2";
// const char* password = "RoBorregos2024";

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";


HTTPClient httpClient;
WiFiClient wClient;
WiFiClientSecure httpsClient;


const char *host = "fast-api-reto.onrender.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
const String Link = "/add-log-sensor";
const String Actuator_link = "/add-log-actuator";

String device = "14";
String actuator = "4";


void start_ota_update(){
  ArduinoOTA.setHostname("esp_pushservo");
  ArduinoOTA.setPassword("esppush01");
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


void logAttempt(int data, bool actuator_s = false){
  if(WiFi.status() == WL_CONNECTED){
    String formattedDate = timeClient.getFormattedDate();

    int splitT = formattedDate.indexOf("T");
    String dayStamp = formattedDate.substring(0, splitT);
    String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    Serial.println(timeStamp);
    String date_up = "\""+dayStamp+" "+timeStamp+"\"";
    Serial.println(date_up);
    String msg;
    if(actuator_s){
      
      msg= "{\"date_a\": "+date_up+",\"actuator_id\":"+actuator+",\"active\":"+String(data)+"}";
    }else{
    msg= "{\"date_\": "+date_up+",\"sensor_id\":"+device+",\"measure\":"+String(data)+"}";
    }
    

    httpsClient.setInsecure();
    httpsClient.setTimeout(10000);

    int r=0; //retry counter
    while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }

  if(actuator_s){
      
       httpsClient.print(String("POST ") + Actuator_link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json"+ "\r\n" +
               "Content-Length:" + msg.length() + "\r\n\r\n" +
               msg + "\r\n" +
               "Connection: close\r\n\r\n");
    }
    else{
      httpsClient.print(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json"+ "\r\n" +
               "Content-Length:" + msg.length() + "\r\n\r\n" +
               msg + "\r\n" +
               "Connection: close\r\n\r\n");
    }
  

  }
  Serial.println("Message sent");
  return;
}

void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  start_ota_update();
    
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

void loop() {
  ArduinoOTA.handle();
  timeClient.update();

  int button_state = digitalRead(BUTTON_PIN);
  if(button_state == LOW){
    logAttempt(1, true);
    Serial.println("Button pressed");
    servo.write(180);
    delay(1000);
  }
  servo.write(0);

  logAttempt(button_state);
  ArduinoOTA.handle();
  delay(1000);
  ArduinoOTA.handle();

  

}