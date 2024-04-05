#include "car.h"
#include "globe_Test.h"
#include "GoBLE.h"


bool switchRemote = true;
void setup(){
  setupRemote();
  Goble.begin();
  Serial.begin(115200);
}
void loop(){
  int buttonState = Goble.readSwitchSelect();
  if (switchRemote){
    loopRemote();
  }else{
    loopCar();
  }
  if(buttonState == PRESSED){
    switchRemote=!switchRemote;
    if(switchRemote){
      setupRemote();
    }else{
      setupCar();
    }
  }
}
