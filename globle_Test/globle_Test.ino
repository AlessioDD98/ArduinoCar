/*********************************************************************
 * DFMobile GoBLE_APP
 *********************************************************************
 * This example is aimed to control DFMobile basic motion by APP GoBle
 * Created 2015-3-12
 * By Gavin
 * 
 * Button ID:
 * SWITCH_UP      -- 1
 * SWITCH_RIGHT   -- 2
 * SWITCH_DOWN    -- 3
 * SWITCH_LEFT    -- 4
 * SWITCH_SELECT  -- 5
 * SWITCH_START   -- 6
 ********************************************************************/
#include "DFMobile.h"
#include <Servo.h>

DFMobile Robot (7,6,4,5);
#include "Metro.h"
// Speed is modified by mapping vlaue of GamePad Stick
int LeftWheelSpeed;
int RightWheelSpeed;

//GoBle configuration library, help user to identify control button and stick on Gamepad
#include "GoBLE.h"
int joystickX, joystickY;
int buttonState[7];

int ledPin = 13;

void setup (){
  Robot.Direction (LOW,LOW);// (left direction,right direction); 
  pinMode(ledPin,OUTPUT);
  Goble.begin();
  Serial.begin(115200);
}

void loop (){
  if(Goble.available()){
    joystickX = Goble.readJoystickX();  
    joystickY = Goble.readJoystickY();
    Serial.print("joystickX: ");
    Serial.print(joystickX);
    Serial.print("joystickY: ");
    Serial.println(joystickY);

    buttonState[SWITCH_UP]     = Goble.readSwitchUp();
    buttonState[SWITCH_LEFT]   = Goble.readSwitchLeft();
    buttonState[SWITCH_RIGHT]  = Goble.readSwitchRight();
    buttonState[SWITCH_DOWN]   = Goble.readSwitchDown();
    buttonState[SWITCH_SELECT] = Goble.readSwitchSelect();
    buttonState[SWITCH_START]  = Goble.readSwitchStart();

    //enable to map the value from (1~255) to (-255~255)
    int SpeedX=2*joystickX-256;
    int SpeedY=2*joystickY-256;
    Serial.print("Speed: ");
    Serial.print(SpeedX);
    Serial.print("  ");
    Serial.println(SpeedY);

    if (SpeedX>200 || SpeedX<-200){           //when joystick X is pushed up
      int difference = SpeedY;
      if(SpeedY<60 && SpeedY>-60)difference = 0;
      LeftWheelSpeed=SpeedX + (difference);                 
      RightWheelSpeed=SpeedX - (difference);
      Robot.Speed (LeftWheelSpeed,RightWheelSpeed);
    }         
    else if (SpeedY>200 || SpeedY<-200){           
      LeftWheelSpeed=SpeedY-80;               //when joystick Y is pushed up
      RightWheelSpeed=-SpeedY-80;
      Robot.Speed(LeftWheelSpeed,RightWheelSpeed);
    }
    else if (SpeedX==0 && SpeedY==0){
      Robot.Speed(0,0);
    }
    
    if (buttonState[1] == PRESSED){
        digitalWrite(ledPin,HIGH);  
    }
    if (buttonState[1] == RELEASED){
        digitalWrite(ledPin,LOW);
    }
  }
}
