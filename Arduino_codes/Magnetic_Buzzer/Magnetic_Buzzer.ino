#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>



#define MAG 12 // D6
#define BUZZER 13 // D7

///Certificate	3560b24af863c672601dc22bbdbf6fefd3ec508c72438ac03d938184357f2be9
//Public Key	2204991132aadfdbd5323c2a378e09f7f5012f815214b81f0a3bc8dc9694ca83

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


const char* ssid = "RoBorregos2";
const char* password = "RoBorregos2024";



HTTPClient httpClient;
WiFiClient wClient;
WiFiClientSecure httpsClient;


const char *host = "fast-api-reto.onrender.com";
const int httpsPort = 443;  //HTTPS= 443 and HTTP = 80
const String Link = "/add-log-sensor";

String device = "11";

void start_ota_update(){
  ArduinoOTA.setHostname("esp_magbuzz");
  ArduinoOTA.setPassword("espmag01");
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
    String formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    String dayStamp = formattedDate.substring(0, splitT);
    String timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
    Serial.println(timeStamp);
    String date_up = "\""+dayStamp+" "+timeStamp+"\"";
    Serial.println(date_up);
    String msg= "{\"date_\": "+date_up+",\"sensor_id\":"+device+",\"measure\":"+String(data)+"}";

    httpsClient.setInsecure();
    httpsClient.setTimeout(10000);

    int r=0; //retry counter
    while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }

  httpsClient.print(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json"+ "\r\n" +
               "Content-Length:" + msg.length() + "\r\n\r\n" +
               msg + "\r\n" +
               "Connection: close\r\n\r\n");

  }
  Serial.println("Message sent");
  return;
}

void setup(){
Serial.begin(115200);
  start_ota_update();
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
  ArduinoOTA.handle();

    timeClient.update();
    int door_state = 0;
    int distance = digitalRead(MAG);
    if(distance == 0){  
        digitalWrite(BUZZER, HIGH);
        Serial.println("Puerta Abierta");
        door_state = 1;
    }else{
        digitalWrite(BUZZER, LOW);
    }
 logAttempt(door_state);
 
 ArduinoOTA.handle();
 delay(1000); // Wait for half a second before the next reading
  ArduinoOTA.handle();
// delay(2000);
}

