#include "Adafruit_PWMServoDriver.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  110 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575// this is the 'maximum' pulse length count (out of 4096) // 600

// our servo # counter
uint8_t servonum = 0;


int newAngle ;
const int MaxChars = 4;
char strValue[MaxChars+1];
int index = 0;
boolean stringComplete = false;  // whether the string is complete
String inputString = "";         // a String to hold incoming data


void setup() {
  Serial.begin(9600);       // activate serial communication at 9600 baud
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  newAngle = (SERVOMIN + SERVOMAX) / 2 ;
  pwm.setPWM(0,0,SERVOMIN) ;

  yield();
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  /*// Drive each servo one at a time
  Serial.println(servonum);
  
  pwm.setPWM(0, 0, SERVOMAX);

  delay(500);
 
  pwm.setPWM(0, 0, SERVOMIN);

  delay(500);

  servonum ++;
  if (servonum > 7) servonum = 0;
  */
  // Detect serial input changes and send them to the servo
  while(Serial.available()) 
   {
      char ch = Serial.read();
      Serial.write(ch);
      if(index < MaxChars && isDigit(ch)) { 
            strValue[index++] = ch; 
      } else { 
            strValue[index] = 0; 
            newAngle = atoi(strValue); 
            if(newAngle > SERVOMIN && newAngle < SERVOMAX){
                   pwm.setPWM(0,0,newAngle) ;
                   stringComplete = true ;
            }
            else  {Serial.println("Invalid angle");}
            index = 0;
      }  
   }
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(strValue);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
