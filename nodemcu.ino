{\rtf1\ansi\ansicpg1252\cocoartf1561\cocoasubrtf400
{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 #include <Arduino.h>\
#include <SimpleDHT.h>\
\
#include <DNSServer.h>\
#include <ESP8266WebServer.h>\
#include <WiFiManager.h>\
#include <ESP8266HTTPClient.h>\
int pinDHT11 = 2;\
SimpleDHT11 dht11;\
void setup() \{\
\
    Serial.begin(115200);\
    \
    for(uint8_t t = 4; t > 0; t--) \{\
        Serial.printf("[SETUP] WAIT %d...\\n", t);\
        Serial.flush();\
        delay(1000);\
    \}\
    WiFiManager wifiManager;\
    wifiManager.autoConnect("Temp&HumiSensor");\
    Serial.println("connected...yeey :)");\
\}\
\
void loop() \{\
        byte temperature = 0;\
        byte humidity = 0;\
        int err = SimpleDHTErrSuccess;\
        if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) \{\
          Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);\
          return;\
        \}\
\
        HTTPClient http;\
\
        Serial.print("[HTTP] begin...\\n");\
        // configure traged server and url\
        String base_url = \'93YOUR BASE URL\'93;\
        String endpoint = base_url+\'94YOUR ENDPOINT\'93;\
        Serial.print(endpoint);\
        http.begin(endpoint); \
        // start connection and send HTTP header\
        int httpCode = http.GET();\
\
        // httpCode will be negative on error\
        if(httpCode > 0) \{\
            // HTTP header has been send and Server response header has been handled\
            Serial.printf("[HTTP] GET... code: %d\\n", httpCode);\
\
            // file found at server\
            if(httpCode == HTTP_CODE_OK) \{\
                String payload = http.getString();\
                Serial.println(payload);\
            \}\
        \} else \{\
            Serial.printf("[HTTP] GET... failed, error: %s\\n", http.errorToString(httpCode).c_str());\
        \}\
\
        http.end();\
\
    delay(10000);\
\}}