/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/*MP3 PLAYIG LIBRARY*/
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

SoftwareSerial mySerial(D0, D1); // RX, TX


#ifndef APSSID
#define APSSID "HeThongAmThanh"
#define APPSK  "12345678"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/

void handleRoot() {
  server.send(200, "text/html", "<h1>BENH VIEN TRUNG UONG HUE<br>XE PHAT THUOC TU XA</h1>");
}
void handleFile1() {
  server.send(200, "text/html", "<h1>Play file 1</h1>");
  mp3_play(1);
}
void handleFile2() {
  server.send(200, "text/html", "<h1>Play file 2</h1>");
  mp3_play(2);
}
void handleFile3() {
  server.send(200, "text/html", "<h1>Play file 3</h1>");
  mp3_play(3);
}
void handleFile4() {
  server.send(200, "text/html", "<h1>Play file 4</h1>");
  mp3_play(4);
}
void handleFile5() {
  server.send(200, "text/html", "<h1>Play file 5</h1>");
  mp3_play(5);
}
void handleFile6() {
  server.send(200, "text/html", "<h1>Play file 6</h1>");
  mp3_play(6);
}
void handleFile7() {
  server.send(200, "text/html", "<h1>Play file 7</h1>");
  mp3_play(7);
}
void handleFile8() {
  server.send(200, "text/html", "<h1>Play file 8</h1>");
  mp3_play(8);
}
void handleFile9() {
  server.send(200, "text/html", "<h1>Play file 9</h1>");
  mp3_play(9);
}
void handleFile0() {
  server.send(200, "text/html", "<h1>Stop</h1>");
  mp3_stop();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);// B Mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  delay(3000);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.on("/", handleRoot);
  server.on("/file1", handleFile1);
  server.on("/file2", handleFile2);
  server.on("/file3", handleFile3);
  server.on("/file4", handleFile4);
  server.on("/file5", handleFile5);
  server.on("/file6", handleFile6);
  server.on("/file7", handleFile7);
  server.on("/file8", handleFile8);
  server.on("/file9", handleFile9);
  server.on("/file0", handleFile0);
  
  server.begin();
  Serial.println("HTTP server started");
  
  /*MP3*/
  mySerial.begin (9600);
  mp3_set_serial (mySerial);  //set softwareSerial for DFPlayer-mini mp3 module 
  mp3_set_volume (25);
  delay(1000);
}

void loop() {
  server.handleClient();
}
