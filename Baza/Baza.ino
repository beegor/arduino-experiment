//Arduino Uno R3 BAZA

#include <RCSwitch.h>
RCSwitch receiver = RCSwitch();

void setup() {
  
  Serial.begin(9600);
  receiver.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
  //pinMode(13, OUTPUT);
  //digitalWrite(13, LOW);
  //Bridge.begin();
}

void loop() {
  //Bridge.begin();
  if (receiver.available()) {
    dataReceived();
  }
  delay(1000);
}

void dataReceived(){
 long value = receiver.getReceivedValue();
    Serial.println("R3 received info , motion detected "); 
    Serial.print(value);
    Serial.println(" seconds ago");
    receiver.resetAvailable();
     //sendToServer(value);
  }

//void sendToServer(long motionSecondsAgo)
  //HttpClient client;
   //client.get("http://192.168.0.100:8080/portal");


