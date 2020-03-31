/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

int led_pin = 2;

  int C1 = D6;
  int C2 = D7;
  int C3 = D8;

  int H1 = D1;
  int H2 = D2;
  int H3 = D3;
  int H4 = D4;
  
char ScanKey()
{
  digitalWrite(C1, LOW);digitalWrite(C2, HIGH);digitalWrite(C3, HIGH);
  if(digitalRead(H1) == LOW) return '1';
  else if(digitalRead(H2) == LOW) return '4';
  else if(digitalRead(H3) == LOW) return '7';
  else if(digitalRead(H4) == LOW) return '*';

  digitalWrite(C1, HIGH);digitalWrite(C2, LOW);digitalWrite(C3, HIGH);
  if(digitalRead(H1) == LOW) return '2';
  else if(digitalRead(H2) == LOW) return '5';
  else if(digitalRead(H3) == LOW) return '8';
  else if(digitalRead(H4) == LOW) return '0';
  
  digitalWrite(C1, HIGH);digitalWrite(C2, HIGH);digitalWrite(C3, LOW);
  if(digitalRead(H1) == LOW) return '3';
  else if(digitalRead(H2) == LOW) return '6';
  else if(digitalRead(H3) == LOW) return '9';
  else if(digitalRead(H4) == LOW) return '#';

  return '@';
}

void setup() {

  Serial.begin(115200);
  Serial.println("Ready");

  pinMode(led_pin, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("HeThongAmThanh", "12345678");   

  pinMode(C1, OUTPUT); digitalWrite(C1, HIGH);
  pinMode(C2, OUTPUT); digitalWrite(C2, HIGH);
  pinMode(C3, OUTPUT); digitalWrite(C3, HIGH);

  pinMode(H1, INPUT_PULLUP);digitalWrite(H1, HIGH);
  pinMode(H2, INPUT_PULLUP);digitalWrite(H2, HIGH);
  pinMode(H3, INPUT_PULLUP);digitalWrite(H3, HIGH);
  pinMode(H4, INPUT_PULLUP);digitalWrite(H4, HIGH);

}

void loop() {
    char num_file;
    
    digitalWrite(led_pin, LOW);
    
    do{
      delay(50);
      num_file =  ScanKey();
    }while( num_file == '@');
    
    String http_str = "http://192.168.4.1/file" + String(num_file);
    Serial.print("http_str = ");Serial.println(http_str);
    
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    digitalWrite(led_pin, HIGH); 
    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    http.begin(client, http_str); // HTTP
    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    http.end();    
  }
  delay(1000);
}
