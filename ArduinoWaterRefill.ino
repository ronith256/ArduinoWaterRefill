#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>
#include <NewPing.h>
#include <NoDelay.h>

#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "WaterESP"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377

#define triggerPin 3 
#define echoPin 2
#define redPin 6
#define greenPin 5
#define MAX_DISTANCE 100
 
NewPing sonar(triggerPin, echoPin, MAX_DISTANCE);



// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 72 bytes
  { 255,0,0,20,0,65,0,16,18,1,129,0,13,5,39,7,13,87,97,116,
  101,114,32,76,101,118,101,108,0,66,1,18,14,30,40,177,26,67,4,13,
  58,40,5,2,26,16,129,0,18,70,31,5,130,77,111,116,111,114,32,83,
  116,97,116,117,115,0,65,112,28,77,9,9 };
  
// this structure defines all the variables and events of your control interface 
struct {

  int8_t water_level; 
  char wtp[16];  
  uint8_t motor_led_r; 
  uint8_t motor_led_g; 
  uint8_t motor_led_b;  

    // other variable
  uint8_t connect_flag;  

} RemoteXY;
#pragma pack(pop)

noDelay delayer(700);
noDelay zeroDelay(200);


void setup() 
{
  RemoteXY_Init (); 
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() 
{ 
  RemoteXY_Handler ();

  int d = sonar.ping_cm();  
  if(d==0){
    if(zeroDelay.update()){int d = sonar.ping_cm();}
  }
  d = 100-d;

  if(delayer.update())
  {
  
  if(d>0){RemoteXY.water_level = (d);}
  if(d<20){
    RemoteXY.motor_led_r = 255;
    RemoteXY.motor_led_g = 0;
    analogWrite(redPin, 2);
    analogWrite(greenPin, 0);
    strcpy  (RemoteXY.wtp, "Motor is On");
  } else if(d>20 && d<50){
    RemoteXY.motor_led_r = 255;
    RemoteXY.motor_led_g = 140;
    analogWrite(redPin, 4);
    analogWrite(greenPin, 2);
  } else if(d>50 && d<75){
    RemoteXY.motor_led_r = 255;
    RemoteXY.motor_led_g = 255;
    analogWrite(redPin, 2);
    analogWrite(greenPin, 2);
  } else if(d>75){
    RemoteXY.motor_led_r = 0;
    RemoteXY.motor_led_g = 255;
    analogWrite(redPin, 0);
    analogWrite(greenPin, 2);
    strcpy (RemoteXY.wtp, "Motor is Off");
  }
  }
}