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
    dataReceived();
  }
  delay(1000);
}

void dataReceived(){
  long value = receiver.getReceivedValue();

  int motionBeforSecond = value % 10000;
  int temp = (value / 100000) % 100 - 50; // we added 50 on nano side
  int lightOnValue = value / 10000000;
  String lightOn = lightOnValue == 1 ? "On" : "Off";
  
  Serial.println("R3 received info\t"); 
  Serial.print("Motion before ");
  Serial.print(motionBeforSecond);
  Serial.print(" seconds\tTemp: ");
  Serial.print(temp);
  Serial.print("\tLigh: ");
  Serial.println(lightOn);
  
  receiver.resetAvailable();
  sendToServer(motionBeforSecond, temp, lightOn);
}

void sendToServer(int motionBeforSecond, int temp, String lightOn ) {

    client.print("GET /receive?");
    client.print("motionBeforeSecond=");
    client.print(motionBeforSecond);
    client.print("&temp=");
    client.print(temp);
    client.print("&lightOn=");
    client.print(lightOn);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.0.100");
    client.println("Connection: keep-alive");
    client.println();
    client.flush();
}

