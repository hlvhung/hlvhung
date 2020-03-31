#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <LiquidCrystal.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

#define DHTPIN D6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float humi;
float temp;
float fahrenheit;

void GetTemp();
String ConvertTimeToString();

char auth[] = "hQFPG-Z_pKMiIgA-9ceP6niqEj0FRmr0";
WidgetRTC rtc;

const int rs = D0, en = D1, d4 = D2, d5 = D3, d6 = D4, d7 = D5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("\r\nDHT vs ESP8266 TESTING.......\r\n");

  dht.begin();

  lcd.begin(16, 2);
  
  Serial.println("\r\nCONNECT TO WIFI.......\r\n");
  WiFiManager wifiManager;
  wifiManager.autoConnect("Temprature Server");
  
  
  Blynk.begin(auth);
  rtc.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Blynk.run();
  GetTemp();
}


void GetTemp()
{
	String currentTime;
  humi = dht.readHumidity();
  temp = dht.readTemperature();

  if(isnan(humi)||isnan(temp))
  {
    Serial.println("\r\nFail to read data from DHT sensor\r\n");
    return;
  }
  else
  {
	currentTime = ConvertTimeToString();
	
    Serial.println("\r\n");
	Serial.println(currentTime);
    Serial.print("Temp: ");Serial.print(temp, 1); Serial.println("oC");

    Serial.print("Humidity: ");Serial.print(humi, 0);Serial.println("%");
    Serial.println("\r\n\r\n");

	
	lcd.setCursor(3, 0);
    lcd.print(currentTime.substring(0, 8));
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(temp, 1);lcd.print("oC     ");lcd.print(humi, 0);lcd.print("%  ");
  
	
	Blynk.virtualWrite(V6, String("Nhiệt độ: ") + String(temp, 1) + String("oC"));
	Blynk.virtualWrite(V5, String("Độ ẩm: ") + String(humi,0) + String("%"));
	Blynk.virtualWrite(V4, currentTime);
  }
}


String ConvertTimeToString()
{
  String result = "";
  result = String(hour());
  
  result += String(":");
  
  if(minute() < 10) result += String("0") + String(minute());
  else result += String(minute());
  
  result += String(":");
  
  if(second() < 10) result += String("0") + String(second());
  else result += String(second());
  
  result += String(" ");
  
  if(day() < 10) result += String("0") + String(day());
  else result += String(day());

  
  result += String("/");
  
  if(month() < 10) result += String("0") + String(month());
  else result += String(month());

  
  result += String("/");
  
  result += String(year());
  return result;  
}
