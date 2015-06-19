#include <dht.h>

//Elektronika u uredu

#include <RCSwitch.h>
#include <Time.h>

RCSwitch transmiter = RCSwitch();

int ured = 1;
int lastmotiontime = -1;
dht DHT;

void setup() {
  Serial.begin(9600);
  transmiter.enableTransmit(10); //transmitter na pin 10
  pinMode(2, INPUT_PULLUP);   //s motion senzora podatci, pin 2
  attachInterrupt(0, sendAlert, CHANGE);
  randomSeed(A0);
  
}

void loop() {

  // ocitanje senzora
  int lightOn = 2;
  
  int lastMotionBefore = getLastMotionBefore();
  int tempAndHum[] = {0,0};
  getTempAndHumidity(tempAndHum);
  
  int temp = tempAndHum[0] + 50; //because we dont want negative temperatures
   
  
  //slanje podataka ukomponiranih u 1 long broj
  long data = lightOn * 10000000;
  data =  data + (temp * 100000);
  data = data + lastMotionBefore;
  Serial.print("Sending to baza: ");Serial.println(data);
  transmiter.send(data, 32);

  // cakanje 5 sekundi
  delay(5000);
}


int getLastMotionBefore(){
  
  int currentSecond = now();
  if (lastmotiontime == -1) {
    Serial.println("No motion detected");
    return 10000;
  }
  else {
    int lastMotionBefore = currentSecond - lastmotiontime;
    Serial.print("Motion detected ");
    Serial.print(lastMotionBefore);
    Serial.println(" seconds ago");
    return lastMotionBefore;
  }
}


void getTempAndHumidity(int tempAndHum[]){
  
  int termRead = DHT.read11(6);
  switch (termRead)
  {
    case DHTLIB_OK:  
      Serial.print("OK,\t"); 
      break;
    default: 
      Serial.print("Unknown error,\t"); 
      break;
  }

  tempAndHum[0] = DHT.temperature;
  tempAndHum[1] = DHT.humidity;

  Serial.print("Temperature: ");
  Serial.print(DHT.temperature, 1);
  Serial.print(" ,\tHumidity: ");
  Serial.println(DHT.humidity ,1);
  
}



void sendAlert() {
  Serial.println("Motion detected from sensor ");
  lastmotiontime = now();
}

