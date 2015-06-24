#include <dht.h>

//Elektronika u uredu

#include <RCSwitch.h>
#include <Time.h>

//define library for LightSensor
#include <BH1750FVI.h> // Sensor Library
#include <Wire.h> // I2C Library
BH1750FVI LightSensor;
RCSwitch transmiter = RCSwitch();
int officeNo = 2;
boolean motionDetectedSinceLastSend = false;
dht DHT;

void setup() {
  Serial.begin(9600);
  
//aktivacija LightSenzora
  LightSensor.begin();
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);
  Serial.println("Ucitavanje podataka...");
  transmiter.enableTransmit(10); //transmitter na pin 10
  pinMode(2, INPUT_PULLUP);   //s motion senzora podatci, pin 2
  attachInterrupt(0, sendAlert, CHANGE);
  randomSeed(A0);
}

void loop() {
     
  // ocitanje senzora
  int lightValue = getLight();
  int tempAndHum[] = {0,0};
  getTempAndHumidity(tempAndHum);
  int temp = tempAndHum[0] + 50; //because we dont want negative temperatures
  Serial.print("temp var: ");Serial.println(temp);
  int hum = tempAndHum[1];
  //slanje podataka ukomponiranih u 1 long broj
  long data = packData(lightValue, temp, hum);
  motionDetectedSinceLastSend = false;
  Serial.print("Sending to baza: ");Serial.println(data);
  transmiter.send(data, 32);
  // cekanje 5 sekundi
  delay(5000);
}

long packData(long lightValue, long tmp, long hum){
  //slanje podataka ukomponiranih u 1 long broj
  long result = 0;
  long lightCategory = getLightCategory(lightValue);
  long lightPart = lightCategory * 100000000;    //8 nula
  Serial.print("lightPart: ");Serial.println(lightPart);
  long officePart = officeNo * 100000;   //5 nula
  Serial.print("officePart: ");Serial.println(officePart);
  Serial.print("temp: "); Serial.println(tmp);
  long tempPart = tmp * 1000; 
  Serial.print("tempPart: ");Serial.println(tempPart);
  long humPart = hum * 10;
  Serial.print("humPart: ");Serial.println(humPart);
  long motionPart = motionDetectedSinceLastSend ? 1 : 0;
  Serial.print("motionPart: ");Serial.println(motionPart);
  
  result = officePart + lightPart + tempPart + humPart + motionPart;
  Serial.print("result: ");Serial.println(result);
  
  return result;
}

int getLightCategory(long lightValue){
  if(lightValue <= 3)
    return 1;
  else if (lightValue <= 30)
    return 2;
  else if (lightValue <= 100)
    return 3;
  else
    return 4; 
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

int getLight(){
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Svjetlost: ");
  Serial.print(lux);
  Serial.println(" lux");
  return lux;
}

void sendAlert() {
  Serial.println("Motion detected from sensor ");
  motionDetectedSinceLastSend = true;
}


