#include "SoftServo.h"

#define SERVO_LEFT_PIN 1    // Servo control line (orange) on Trinket Pin #3
#define SERVO_RIGHT_PIN 0   // Servo control line (orange) on Trinket Pin #4
#define LDR_LEFT_ANPIN 1    // Left LDR Analog pin (AN1 on the Trinket is the pin labeled #2)
#define LDR_RIGHT_ANPIN 2   // Right LDR Analog pin (AN2 on the Trinket is the pin labeled #4)
#define LED 1               // LED GPIO pin #1.

Adafruit_SoftServo servo_left, servo_right;

const int turnCW = 0;
const int turnCCW = 180;

// Duration of a ping, distance in inches, distance converted to cm.
long LDR_LEFT_ANPIN_val, LDR_RIGHT_ANPIN_val;

// Number in cm when the rover will reverse and try to navigate around.
int LDR_LEFT_ANPIN_ambient = 0;
int LDR_RIGHT_ANPIN_ambient = 1;

const int ambient_sample_time_ms = 2000;
int start_time = 0;
int ambient_sample_end_time_ms = -1;
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
  LDR_LEFT_ANPIN_val = analogRead(LDR_LEFT_ANPIN);
  LDR_RIGHT_ANPIN_val = analogRead(LDR_RIGHT_ANPIN);
  
  if(LDR_LEFT_ANPIN_val > LDR_LEFT_ANPIN_ambient) LDR_LEFT_ANPIN_ambient = LDR_LEFT_ANPIN_val;
  if(LDR_RIGHT_ANPIN_val > LDR_RIGHT_ANPIN_ambient) LDR_RIGHT_ANPIN_ambient = LDR_RIGHT_ANPIN_val;  
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
  LDR_LEFT_ANPIN_val = analogRead(LDR_LEFT_ANPIN);
  LDR_RIGHT_ANPIN_val = analogRead(LDR_RIGHT_ANPIN);
  
  if(LDR_LEFT_ANPIN_val > LDR_LEFT_ANPIN_ambient + sensitivity)
  {
    blinkLed();
    turnLeft();
  }
  
  if(LDR_RIGHT_ANPIN_val > LDR_RIGHT_ANPIN_ambient + sensitivity)
  {
    blinkLed();
    turnRight();
  }
  delay(15);
  return;
  
  /*
  // establish variables for duration of the ping, and the distance 
  // result in inches and centimeters.
  duration = 0;
  inches = 0;
  cm = 0;

  // Long distances will cause the servos to misbehave... cap at 50 cm.
  if ( cm > 50 ) {
    cm = 50;
  }

  if ( cm < obstacle ) {
    // back_track * delay(15) = distance the rover will back-up during 
    // obstacle avoidance.
    for (int i = 0; i < back_track; i++) {
      // Magic numbers... will always backup the same direction. Can you 
      // think of a better way to navigate obstacles?
      servo_left.write(150);
      servo_left.refresh();
      servo_right.write(50);
      servo_right.refresh();  
      delay(15);
    }
  }
  */
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
