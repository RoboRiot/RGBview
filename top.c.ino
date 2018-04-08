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
#define MINANGLE  0
#define MAXANGLE  180

// our servo # counter
uint8_t servonum = 0;


int hAngle ;
int hDisplacement ;             // displacement angle
int vAngle ;
int vDisplacement ;             // displacement angle
const int MaxChars = 4;
char hstrValue[MaxChars+1];
int hindex = 0;
char vstrValue[MaxChars+1];
int vindex = 0;
boolean stringComplete = false;  // whether the string is complete
char inputString[MaxChars+1];         // a String to hold incoming data
int turn ;                       // indicates whether hAngle or hDisplacement is being updated; 0 for horizontal, 1 for vertical

// convAngle converts a requested angle (0-180) and converts it into pulse length for the servo
unsigned int convAngle(int angle) {
  long long num ;
  num = 31*angle/12 + SERVOMIN ;        // simplified numbers to avoid int overflows
  return num ;
}

void setup() {
  Serial.begin(9600);       // activate serial communication at 9600 baud
  Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  hAngle = 90 ;
  vAngle = 90 ;
  Serial.println(itoa(hAngle,inputString,10));
  pwm.setPWM(0,0,convAngle(hAngle)) ;
  pwm.setPWM(1,0,convAngle(vAngle)) ;
  pwm.setPWM(2,0,convAngle(180 - vAngle)) ;
  int turn = 0 ;

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
      //Serial.write(ch);
      if(hindex < MaxChars && turn == 0 && (isDigit(ch) || (hindex == 0 && ch == '-'))) { 
            hstrValue[hindex++] = ch; 
      } 
      else if (ch == ' ') {               // space separates h and v vals
        turn = 1 ;
      }
      else if(vindex < MaxChars && turn == 1 && (isDigit(ch) || vindex == 0 && ch == '-')) { 
            vstrValue[vindex++] = ch; 
      }
      else { 
            hstrValue[hindex] = '\0';
            vstrValue[vindex] = '\0';
            //Serial.println(itoa(convAngle(atoi(hstrValue)),inputString,10));
            hDisplacement = atoi(hstrValue); 
            vDisplacement = atoi(vstrValue);
            //Serial.println(hDisplacement);
            //Serial.println(vDisplacement);
            if(hAngle + hDisplacement >= MINANGLE && hAngle + hDisplacement <= MAXANGLE){
                   hAngle = hAngle + hDisplacement ;
                   pwm.setPWM(0,0,convAngle(hAngle)) ;
                   stringComplete = true ;
            }
            if(vAngle + vDisplacement >= MINANGLE && vAngle + vDisplacement <= MAXANGLE){
                   vAngle = vAngle + vDisplacement ;
                   pwm.setPWM(1,0,convAngle(vAngle)) ;
                   pwm.setPWM(2,0,convAngle(180 - vAngle)) ;
                   stringComplete = true ;
            }
            //stringComplete = true ;
              
            vindex = 0;
            hindex = 0;
            turn = 0 ;
      }  
   }

  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(itoa(hAngle,inputString,10));
    Serial.println(itoa(vAngle,inputString,10));
    stringComplete = false;
  }
}
