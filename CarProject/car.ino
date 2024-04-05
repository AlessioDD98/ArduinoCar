/***************************************************
DFRobot
ROB0117 Cherokey 4WD
Sonar Dodge
***************************************************
This example uses a URM sensor to drive the robot and avoid obstacles

Updated 2015-12-31
By Matt

GNU Lesser General Public License.
See <http://www.gnu.org/licenses/> for details.
All above must be included in any redistribution
****************************************************/

/***********Notice and Troubleshooting***************
For help and info visit the wiki page for this product:
https://www.dfrobot.com/wiki/index.php?title=Basic_Kit_for_Cherokey_4WD_SKU:ROB0117
For any other problems, post on the DFRobot forum or email techsupport@dfrobot.com
****************************************************/
#include <Servo.h>
#include <Metro.h>

Metro measureDistance = Metro(50) ;
Metro sweepServo = Metro(20);

int speedPin_M1;     //M1 Speed Control
int speedPin_M2;     //M2 Speed Control
int directionPin_M1;     //M1 Direction Control
int directionPin_M2;     //M2 Direction Control
unsigned long actualDistance;
Servo myservo;  // create servo object to control a servo
int pos ;
int sweepFlag;

int URPWM; // PWM Output 0－25000US，Every 50US represent 1cmk
int URTRIG; // PWM trigger pin
uint8_t EnPwmCmd[4]= {0x44, 0x02, 0xbb, 0x01}; // distance measure command

void setupCar(){                                 // Serial initialization
  myservo.attach(13);
  Serial.begin(9600);                         // Sets the baud rate to 9600
  SensorSetup();
  measureDistance = Metro(50);
  sweepServo = Metro(20);
  
  speedPin_M1 = 5;     //M1 Speed Control
  speedPin_M2 = 6;     //M2 Speed Control
  directionPin_M1 = 4;     //M1 Direction Control
  directionPin_M2 = 7;     //M2 Direction Control
  actualDistance = 0;
  pos = 60;
  sweepFlag = 1;
  
  URPWM = 3; // PWM Output 0－25000US，Every 50US represent 1cmk
  URTRIG = 10; // PWM trigger pin
   // distance measure command
  delay(1000);  
  
}

void loopCar() {
  Serial.println("loop");
  if (measureDistance.check() == 1) {
    actualDistance = MeasureDistance();
    Serial.println(actualDistance);
    delay(200);
  }
  if( actualDistance<=40){
    carStop();
    if(sweepServo.check() == 1){
      Serial.println("servo is moving");
      servoSweep();
    }
    Serial.println("checking for other ways");
  }else if(actualDistance > 40) {
    if( pos < 60 ){
    carTurnRight(250, 250);
    delay(200);
    }
    if( pos > 60){
     carTurnLeft(250, 250);
     delay(200);
    }
    pos = 60;
    myservo.write(pos);
    carAdvance(100,100);
  }else{
      carStop();
  }
}

void SensorSetup() {
  pinMode(URTRIG, OUTPUT);                    // A low pull on pin COMP/TRIG
  digitalWrite(URTRIG, HIGH);                 // Set to HIGH
  pinMode(URPWM, INPUT);                      // Sending Enable PWM mode command
  for (int i = 0; i < 4; i++) {
    Serial.write(EnPwmCmd[i]);
  }
  myservo.write(60);                      

}

int MeasureDistance() { // a low pull on pin COMP/TRIG  triggering a sensor reading
  digitalWrite(URTRIG, LOW);
  digitalWrite(URTRIG, HIGH);               // reading Pin PWM will output pulses
  unsigned long distance = pulseIn(URPWM, LOW);
  if (distance == 1000) {          // the reading is invalid.
    Serial.print("Invalid");
  } else {
    distance = distance / 50;       // every 50us low level stands for 1cm
  }
  return distance;
}

void carStop() {                //  Motor Stop
  digitalWrite(speedPin_M2, 0);
  digitalWrite(directionPin_M1, LOW);
  digitalWrite(speedPin_M1, 0);
  digitalWrite(directionPin_M2, LOW);
}

void carTurnRight(int leftSpeed, int rightSpeed) {       //Move backward
  analogWrite (speedPin_M2, leftSpeed);             //PWM Speed Control
  digitalWrite(directionPin_M1, LOW);              //set LOW to reverse or HIGH to advance
  analogWrite (speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, LOW);
}

void carTurnLeft(int leftSpeed, int rightSpeed) {     //Move forward
  analogWrite (speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, HIGH);
  analogWrite (speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, HIGH);
}

void carBack(int leftSpeed, int rightSpeed) {           //Turn Left
  analogWrite (speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, LOW);
  analogWrite (speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, HIGH);
}
void carAdvance(int leftSpeed, int rightSpeed) {         //Turn Right
  analogWrite (speedPin_M2, leftSpeed);
  digitalWrite(directionPin_M1, HIGH);
  analogWrite (speedPin_M1, rightSpeed);
  digitalWrite(directionPin_M2, LOW);
}

void servoSweep() {
  //Serial.println(pos);
  if(sweepFlag ){
     if(pos>=0 && pos<=120){
        pos=pos+10;                                  // in steps of 1 degree
        myservo.write(pos);                         // tell servo to go to position in variable 'pos'
    }
      if(pos>119)  sweepFlag = false;                       // assign the variable again
  }else {
      if(pos>=0 && pos<=120){
        pos=pos-10;
        myservo.write(pos);
      }
      if(pos<1)  sweepFlag = true;
   }
}
