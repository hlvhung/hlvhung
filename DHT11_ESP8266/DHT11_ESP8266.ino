#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN D6
#define DHTTYPE DHT11

#define BELL D8

DHT dht(DHTPIN, DHTTYPE);
float humi;
float temp;

void GetTemp();
void CalcTimer();

void Http_ReQuest(String request);
void GetTimer();

void Beat();

uint8_t h, m, s;
String s_time;

uint8_t count;

//const int rs = D0, en = D1, d4 = D2, d5 = D3, d6 = D4, d7 = D5;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//const String Host = "http://192.168.1.7:8080//temp/";
const String Host = "http://hunghlv.onlinewebshop.net/temp/";
String request;
/*
 Mach NodeMCU
 SDA: D2
 SCL: D1 
 */


void setup() {
  // put your setup code here, to run once:
  pinMode(BELL, OUTPUT);
  digitalWrite(BELL, HIGH);
  
  Serial.begin(115200);
  Serial.println("\r\nDHT vs ESP8266 TESTING.......\r\n");

  dht.begin();

  lcd.begin();
  
  Serial.println("\r\nCONNECT TO WIFI.......\r\n");
  WiFiManager wifiManager;
  wifiManager.autoConnect("Temprature Server");

  //GetTimer();
  count = 0;

  Beat();
}

void loop() {
  if(count % 15 == 0)
  {
    //http://hunghlv.onlinewebshop.net/temp/update.php?temp=33&humi=99
    request = Host + "update.php?temp=" + String(temp) + "&humi=" + String(humi);
    Http_ReQuest(request);
    Beat();
    
    GetTimer();
  }
  
  GetTemp();
  CalcTimer();  
  lcd.setCursor(3, 0);
  lcd.print(s_time);
  
  delay(1000);
  count++;
}


void CalcTimer()
{
  if(++s > 59)
  {
    s = 0;
    if(++m > 59)
    {
      m = 0;
      if(++h > 23)
      {
        h = 0;
      }
    }
  }
  if(h < 10) s_time = " " + String(h) + ":";
  else s_time = String(h) + ":";
  
  if(m < 10) s_time += "0" + String(m) + ":";
  else s_time += String(m) + ":";
  
  if(s < 10) s_time += "0" + String(s);
  else s_time += String(s);
}


void GetTemp()
{
  humi = dht.readHumidity();
  temp = dht.readTemperature();

  if(isnan(humi)||isnan(temp))
  {
    Serial.println("\r\nFail to read data from DHT sensor\r\n");
    return;
  }
  else
  {	
    Serial.println("\r\n");
    Serial.print("Temp: ");Serial.print(temp, 1); Serial.println("oC");

    Serial.print("Humidity: ");Serial.print(humi, 0);Serial.println("%");
    //Serial.println("\r\n\r\n");

    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(temp, 1);
    lcd.write(0xDF);//ky hieu do C
    lcd.print("C    ");
    lcd.print(humi, 0);
    lcd.print("%  ");
  }
}

void Http_ReQuest(String request){
 if(WiFi.status() == WL_CONNECTED)
 {
   HTTPClient http;
   http.begin(request);
   int httpCODE = http.GET();
   Serial.println(request);
   Serial.println("http code: " + String(httpCODE));
   http.end();
   }
 }

void GetTimer()
{
 if(WiFi.status() == WL_CONNECTED)
 {
  request = Host + "gettime.php";
   HTTPClient http;
   http.begin(request);
   if(http.GET() > 0)
   {
    s_time = http.getString();
    h = s_time.substring(0, 2).toInt();
    m = s_time.substring(3, 5).toInt();
    s = s_time.substring(6, 8).toInt();
   }
   http.end();
  }
}

void Beat()
{
  digitalWrite(BELL, HIGH);
  delay(100);
  digitalWrite(BELL, LOW);
}
