//Arduino Uno R3 BAZA

#include <RCSwitch.h>

#include <SPI.h>
#include <Ethernet.h>

RCSwitch receiver = RCSwitch();


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "192.168.0.100";  
  
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

EthernetClient client;

void setup() { 
  receiver.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
 
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }



  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  Serial.println("connecting...");
  if (client.connect(server, 9999)) {
    Serial.println("connected");
  } 
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  

}

void loop() {
  if (receiver.available()) {
    Serial.println("Receiver available");
    dataReceived();
  }
  delay(1000);
}

void dataReceived(){
  long value = receiver.getReceivedValue();
  Serial.print("Data received: ");Serial.println(value);

  long lightValue = value / 100000000;
  long officeNo = (value / 100000) % 10;
  long temp = (value / 1000) % 100 - 50;    
  long hum  = (value / 10) % 100;
  long motionBeforSecond = value % 10; 
  
  Serial.println("R3 received info");
  
  Serial.print("Value= ");Serial.println(value);  //privremena provjera...
 
  Serial.print("Broj ureda: ");       
  Serial.println(officeNo);    
                
  Serial.print("Svjetlost: ");          
  Serial.println(lightValue);
  
  Serial.print("Temperatura:");          
  Serial.println(temp);
  
  Serial.print("Vlaznost :");             
  Serial.print(hum);   
  Serial.println(" %");
  
  Serial.print("Motion: ");
  Serial.println(motionBeforSecond);
    
  receiver.resetAvailable();
  sendToServer(officeNo, hum, motionBeforSecond, temp, lightValue);
}

void sendToServer(int officeNo, int hum, int motionDetected, int temp, int lightOn ) {

    client.print("GET /receive?");
    client.print("officeNo=");
    client.print(officeNo);
    client.print("&motionDetected=");
    client.print(motionDetected);
    client.print("&temp=");
    client.print(temp);
    client.print("&hum=");
    client.print(hum);
    client.print("&light=");
    client.print(lightOn);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.0.100");
    client.println("Connection: keep-alive");
    client.println();
    client.flush();
}

