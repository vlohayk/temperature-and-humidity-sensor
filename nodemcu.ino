#include <Arduino.h>
#include <SimpleDHT.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
int pinDHT11 = 2;
SimpleDHT11 dht11;
void setup() {

    Serial.begin(115200);
    Serial.println(WiFi.macAddress());
    
    
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
    WiFiManager wifiManager;
    wifiManager.autoConnect("Temp&HumiSensor");
    Serial.println("connected...yeey :)");
}

void loop() {
        byte temperature = 0;
        byte humidity = 0;
        int err = SimpleDHTErrSuccess;
        String mac = WiFi.macAddress();
        if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
          Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
          return;
        }

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        String base_url = "http://vlohayk.com/request/nodemcu/";
        String endpoint = base_url;
        Serial.print(endpoint);
        http.begin(endpoint); //HTTP
        http.addHeader("Content-Type", "text/plain");

        // start connection and send HTTP header
        int httpCode = http.POST(mac+","+temperature+","+humidity);

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] POST... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

    delay(30000);
}
