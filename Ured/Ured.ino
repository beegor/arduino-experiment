//Elektronika u uredu

#include <RCSwitch.h>
#include <Time.h>
RCSwitch transmiter = RCSwitch();

int ured = 1;
int lastmotiontime = -1;

void setup() {
  Serial.begin(9600);
  transmiter.enableTransmit(10); //s motion senzora podatci
  pinMode(2, INPUT_PULLUP);      //transmitter na pin2
  attachInterrupt(0, motionDetected, CHANGE);
  randomSeed(A0);
}


void loop() {
  int currentSecond = now();
  if(lastmotiontime == -1){
    Serial.println("No motion detected");
  }
  else {
     int lastMotionBefore = currentSecond - lastmotiontime;
     Serial.print("Motion detected: ");
     Serial.print(lastMotionBefore);
     Serial.println(" seconds ago");   
     int lightOn = 1;
     int temp = 23;
     long data = lightOn * 10000000;
     data += temp * 100000;
     data += lastMotionBefore;  
     transmiter.send(data,32);
   }
   delay(5000);
}


void motionDetected(){
  Serial.println("Motion detected from sensor ");
  lastmotiontime = now();
}

