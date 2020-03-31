#include <Arduino_FreeRTOS.h>

#define Relay 10 //chan dieu khien relay
#define V1 A1 //chan vao V+
#define V2 A0 //chan vao V-

#define echoPin1    7     // chân echo của cảm biến AS1
#define trigPin1    6     // chân trig của cảm biến AS1
#define echoPin2    5     // chân echo của cảm biến AS2
#define trigPin2    4
#define echoPin3    3     // chân echo của cảm biến AS3
#define trigPin3    2

const int Gioi_han = 30;  //cách 50cm, cảm biến AS1
int khoang_cach;

void TaskReadSensors( void *pvParameters );
void TaskControl( void *pvParameters );


void setup() {
  
  Serial.begin(115200);
  
  while (!Serial);
  Serial.println("Start .....");
  
  xTaskCreate(
    TaskReadSensors
    ,  (const portCHAR *)"ReadSensors"   // A name just for humans
    ,  128  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL );

  xTaskCreate(
    TaskControl
    ,  (const portCHAR *)"Control"
    ,  128  // Stack size
    ,  NULL
    ,  2  // Priority
    ,  NULL );
}

void loop()
{
}


void TaskReadSensors(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  // khoi tao cac chan trigger, echo
  int duration; // biến đo thời gian//số nguyên 0--2^32)-1
  int khoang_cachInt1, khoang_cachInt2, khoang_cachInt3;

  pinMode(trigPin1, OUTPUT); // transmit
  pinMode(echoPin1, INPUT_PULLUP); // receive
  
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT_PULLUP);
  
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT_PULLUP);
  
  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(trigPin1,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trigPin1,1);   // phát xung từ chân trig
    delayMicroseconds(10);   // xung có độ dài 10 microSeconds
    digitalWrite(trigPin1,0);   // tắt chân trig

    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echoPin1,HIGH);
    khoang_cachInt1= duration/58;//Serial.print("khoang_cachInt1 ");Serial.println(khoang_cachInt1);
    vTaskDelay(1/portTICK_PERIOD_MS);
    
    digitalWrite(trigPin2,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trigPin2,1);   // phát xung từ chân trig
    delayMicroseconds(10);   // xung có độ dài 10 microSeconds
    digitalWrite(trigPin2,0);   // tắt chân trig

    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echoPin2,HIGH);
    khoang_cachInt2= duration/58;//Serial.print("khoang_cachInt2 ");Serial.println(khoang_cachInt2);
    vTaskDelay(1/portTICK_PERIOD_MS);

    digitalWrite(trigPin3,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trigPin3,1);   // phát xung từ chân trig
    delayMicroseconds(10);   // xung có độ dài 10 microSeconds
    digitalWrite(trigPin3,0);   // tắt chân trig

    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echoPin3,HIGH);
    khoang_cachInt3= duration/58;//Serial.print("khoang_cachInt3 ");Serial.println(khoang_cachInt3);
    khoang_cach = khoang_cachInt1;
    if(khoang_cach > khoang_cachInt2) khoang_cach = khoang_cachInt2;
    if(khoang_cach > khoang_cachInt3) khoang_cach = khoang_cachInt3;

    Serial.print("Khaong cach: ");Serial.println(khoang_cach);
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void TaskControl(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  pinMode(Relay, OUTPUT);//digitalWrite(Relay, HIGH);
  pinMode(V1, INPUT);digitalWrite(V1, LOW);
  pinMode(V2, INPUT);digitalWrite(V2, LOW);

  for (;;)
  {
    if((digitalRead(V1) == HIGH) && (khoang_cach < Gioi_han))
    {
      digitalWrite(Relay, HIGH);
    }
    else
    {
      digitalWrite(Relay, LOW);
    }
    Serial.print("V1: ");Serial.println(digitalRead(V1));
    vTaskDelay(100/portTICK_PERIOD_MS); // delay 100ms
  }
}
