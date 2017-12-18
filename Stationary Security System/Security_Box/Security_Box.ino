#include <Servo.h>

Servo x_servo;
Servo y_servo;

/* Pins */
const int x_servo_pin = 2;
const int y_servo_pin = 3;
const int motion_pin = 4;
const int wifi_alert_pin = 0;
const int buzzer_pin = 12;
const int led_pin = 11;

const unsigned int MAX_DIST = 400;

//echo, trig
int sensorL[2] = {10, 9};
int sensorM[2] = {8, 9};
int sensorR[2] = {5, 6};

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
}

void DisableAlert(){
  Serial.println("Disabled Alert");
  digitalWrite(wifi_alert_pin, LOW);
  digitalWrite(led_pin, LOW);
  tone(buzzer_pin, 0);
}

int GetDistance(int echoPin, int trigPin){

  long duration, distance;
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  distance = duration/ 58.2;

  if (distance >= MAX_DIST){
    Serial.println("MAX DISTANCE REACHED");
  }
  return (int) distance;
}

void setup() {
  
  pinMode(buzzer_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(wifi_alert_pin, OUTPUT);
  pinMode(motion_pin, INPUT);

  x_servo.attach(x_servo_pin);
  y_servo.attach(y_servo_pin);

  Serial.begin(9600);

  Serial.println("Setting Distance Sensor Pinmodes...");
  
  pinMode(sensorL[1], OUTPUT);
  digitalWrite(sensorL[1], LOW);
  pinMode(sensorM[1], OUTPUT);
  digitalWrite(sensorM[1], LOW);
  pinMode(sensorR[1], OUTPUT);
  digitalWrite(sensorR[1], LOW);

  Serial.println("Setting initial distances");
  // set initial startup distances
  
  initial[0] = GetDistance(sensorL[0], sensorL[1]);
  Serial.println(initial[0]);
  
  initial[1] = GetDistance(sensorM[0], sensorM[1]);
  Serial.println(initial[1]);

  initial[2] = GetDistance(sensorR[0], sensorR[1]);
  Serial.println(initial[1]);

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
  if(MotionDetect()){
    Alert();
    for (int x = 0; x < 3; x++){
      delay(200);
      
      int new_dist = 0;

      if(x == 0){
        new_dist = GetDistance(sensorL[0], sensorL[1]);
      }
      else if(x == 1){
        new_dist = GetDistance(sensorM[0], sensorM[1]);
      }
      else if(x == 2){
        new_dist = GetDistance(sensorR[0], sensorR[1]);
      }

      int dist_difference = initial[x] - new_dist;

      if(dist_difference > 5){
        SweepX(pos_constraints[x][0], pos_constraints[x][1]);
      }

      Serial.print(new_dist);
      Serial.print(", ");
    }

    DisableAlert();
    Serial.println();
  }
}

