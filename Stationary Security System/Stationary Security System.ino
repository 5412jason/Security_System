#include <Servo.h>
//#include <NewPing.h>
#include <dht11.h>


Servo x_servo;
Servo y_servo;

const int threshold = 10;

/* Pins */
const int x_servo_pin = 2;
const int y_servo_pin = 3;
const int motion_pin = 12;
const int wifi_alert_pin = 11;
const int buzzer_pin = 22;
const int led_pin = 13;

const int gas_pin = A0;
const int gas_warning_level = 200;


dht11 DHT11;
#define DHT11PIN 4
const float temp_limits[2] = {20.0, 55.0}; // Lower limit, higher limit
const float humid_limits[2] = {200.0, 240.0}; //To low = possible bad stuff, fire hazard (dry environment) too high = open windows, flood, bad stuff.

//NewPing sensorM(7, 8, 200);
//NewPing sensorR(9, 10, 200);
//NewPing sensorL(5, 6, 200);

int pos = 0;

int initial[3] = {0, 0, 0};

const int pos_constraints[4][2] = {{120, 179}, {60, 119}, {0, 59}, {0,180}};

void SweepX(int lower, int upper){
  x_servo.write(lower);
  
  for(pos = lower; pos <= upper; pos++){
    x_servo.write(pos);
    delay(50);
  }
  for(pos = upper; pos >= lower; pos -= 1){
    x_servo.write(pos);
    delay(50);
  }
}

bool CheckGas(){
  int gas_level = analogRead(gas_pin);

  Serial.print("Gas Level: ");
  Serial.println(gas_level);
  
  if(gas_level >= gas_warning_level){
    return true;
  }
  else{
    return false;
  }
}

float GetTemp(){
  float temp = (float)DHT11.temperature;
  return temp;
}

float GetHumid() {
  float humid = (float)DHT11.humidity;
  return humid;
}

bool CheckTempHumid(){
  float humid = GetHumid();
  float temp = GetTemp();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" Humid: ");
  Serial.println(humid);

  if(humid_limits[0] >= humid || humid_limits[1] <= humid){
    return true;
  }
  else if(temp_limits[0] >= temp || temp_limits[1] <= temp){
    return true;
  }
  else{
    return false;
  }
}

bool MotionDetect(){
  int mValue = digitalRead(motion_pin);
  if(mValue == HIGH){
    return true;
  }
  else{
    return false;
  }
}

void Alert(){
  Serial.println("ALERT");

  digitalWrite(led_pin, HIGH);
  tone(buzzer_pin, 1000);
  digitalWrite(wifi_alert_pin, HIGH);
  delay(15);
  digitalWrite(wifi_alert_pin, LOW);
}

void DisableAlert(){
  Serial.println("Disabled Alert");
  digitalWrite(led_pin, LOW);
  tone(buzzer_pin, 0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(buzzer_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(wifi_alert_pin, OUTPUT);
  pinMode(motion_pin, INPUT);

  x_servo.attach(x_servo_pin);
  y_servo.attach(y_servo_pin);
  
  // set initial startup distances
  /*for(int y = 0; y < 3; y++){
    delay(20);
      if(y == 0){
        initial[y] = sensorL.ping_cm();
      }
      else if(y == 1){
        initial[y] = sensorM.ping_cm();
      }
      else if(y == 2){
        initial[y] = sensorR.ping_cm();
      }
      Serial.println(initial[y]);
  }*/
}

void loop() {
  // put your main code here, to run repeatedly
  for(pos; pos <= 180; pos++){
    Detection();
    x_servo.write(pos);
    delay(50);
  }

  for(pos; pos >= 0; pos--){
    Detection();
    x_servo.write(pos);
    delay(50);
  }
}

void Detection(){
  /*if(CheckTempHumid()){
    Alert();
  }else{
    DisableAlert();
  }*/
  /*if(CheckGas()){
    Alert();
  }
  else{
    DisableAlert();
  }*/
  if(MotionDetect()){
    Alert();
    /*for (int x = 0; x < 3; x++){
      delay(200);
      
      int new_dist = 0;

      if(x == 0){
        new_dist = sensorL.ping_cm();
      }
      else if(x == 1){
        new_dist = sensorM.ping_cm();
      }
      else if(x == 2){
        new_dist = sensorR.ping_cm();
      }

      int dist_difference = initial[x] - new_dist;

      if(dist_difference > 5){
        SweepX(pos_constraints[x][0], pos_constraints[x][1]);
      }

      Serial.print(new_dist);
      Serial.print(", ");
    }*/

    DisableAlert();
    Serial.println();
  }
}

