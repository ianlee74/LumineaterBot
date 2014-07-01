#include "SoftServo.h"

#define SERVO_LEFT_PIN 1    // Servo control line (orange) on Trinket Pin #1
#define SERVO_RIGHT_PIN 0   // Servo control line (orange) on Trinket Pin #0
#define LDR_LEFT_ANPIN 1    // Left LDR Analog pin (AN1 on the Trinket is the pin labeled #2)
#define LDR_RIGHT_ANPIN 2   // Right LDR Analog pin (AN2 on the Trinket is the pin labeled #4)
#define LED 1               // LED GPIO pin #1.

Adafruit_SoftServo servo_left, servo_right;

const int turnCW = 0;
const int turnCCW = 180;

// Current LDR values.
long ldr_left_anpin_val, ldr_right_anpin_val;

// Ambient light levels measured upon startup.
int ldr_left_anpin_ambient = 0;
int ldr_right_anpin_ambient = 0;

// Variables used during determination of ambient levels.
const int ambient_sample_time_ms = 2000;
int start_time = 0;
int ambient_sample_end_time_ms = -1;

// The sensitivity required to move the bot. (the level above ambient)
const int sensitivity = 100;

void setup() 
{  
  // Attach servos... and off we go!
  servo_left.attach(SERVO_LEFT_PIN);
  servo_right.attach(SERVO_RIGHT_PIN);
  
  // Setup LED
  pinMode(LED, OUTPUT);
} 

void measureAmbientLight()
{
  start_time = millis();
  ambient_sample_end_time_ms = start_time + ambient_sample_time_ms;
  ldr_left_anpin_val = analogRead(LDR_LEFT_ANPIN);
  ldr_right_anpin_val = analogRead(LDR_RIGHT_ANPIN);
  
  if(ldr_left_anpin_val > ldr_left_anpin_ambient) ldr_left_anpin_ambient = ldr_left_anpin_val;
  if(ldr_right_anpin_val > ldr_right_anpin_ambient) ldr_right_anpin_ambient = ldr_right_anpin_val;  
  delay(15);
  if(millis() > start_time + ambient_sample_time_ms) return;
}

void blinkLed()
{
    digitalWrite(LED, HIGH);
    delay(15);    
    digitalWrite(LED, LOW);
}

void turnLeft()
{
  servo_right.write(turnCW);
  servo_right.refresh();
}

void turnRight()
{
  servo_left.write(turnCCW);
  servo_left.refresh();  
}

void loop() 
{ 
  // For the first couple seconds, determine the ambient light value.
  if(ambient_sample_end_time_ms == -1) measureAmbientLight();
  
  // Read LDRs
  ldr_left_anpin_val = analogRead(LDR_LEFT_ANPIN);
  ldr_right_anpin_val = analogRead(LDR_RIGHT_ANPIN);
  
  if(ldr_left_anpin_val > ldr_left_anpin_ambient + sensitivity)
  {
    blinkLed();
    turnLeft();
  }
  
  if(ldr_right_anpin_val > ldr_right_anpin_ambient + sensitivity)
  {
    blinkLed();
    turnRight();
  }
  delay(15);  
}

