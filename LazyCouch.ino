/*
 Lazy Couch, 
 an Arduino Client for RES(T)mote Control (http://www.restmote.com)
 Works on Arduino Ethernet or Ethernet Shield.

 The purpose of this simple client is to play/pause your pc video player when you sit in or stand up from your couch.
 This client has a button, a force resistor and a led. 
 When the button is pushed, or the force resistor reads a big delta value, a POST /api/activeapps/smplayer/control/play is made to the server and the led is briefly lighted.

 See http://www.restmote.com/ for details.
*/
#include <SPI.h>
#include <Ethernet.h>

//constants
const int forcePin = A5;
const int ledPin = 6;
const int buttonPin = 7;
const int forceLimit = 400;

//ethernet
EthernetClient client;
byte mac[] = { 0x00, 0xA2, 0xDA, 0x00, 0xF2, 0x87 };
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
IPAddress ip(192,168,1,12);

//restmote server
char serverRemosko[] = "192.168.1.11";  
int port = 9898;
String apiVideoToggle = "/api/activeapps/smplayer/control/play";

//variables
//unsigned long lastConnectionTime = 0;
boolean lastConnected = false;
int lastSit = -1;  //indicate if someone is sitting over the force resistor last time through the main loop

void setup()
{
  Ethernet.begin(mac, ip, gateway, subnet);  // start the Ethernet connection and the server:
  Serial.begin(9600);                        // start Serial
  pinMode(forcePin, INPUT);
  pinMode(buttonPin, INPUT);  
  pinMode(ledPin, OUTPUT);  
}

void loop()
{
  digitalWrite(ledPin, LOW);

  if (client.available()) { //print to serial data from the connection
    char c = client.read();  
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  
  int digitalValue = digitalRead(buttonPin);
  int forceValue = analogRead(forcePin);
  if (forceValue > 0){
    Serial.print("forceValue ");
    Serial.println(forceValue);
  }
  int sit = forceValue > forceLimit;
  if (digitalValue == 1 || (lastSit > -1 && sit != lastSit)){
    digitalWrite(ledPin, HIGH);
    Serial.println("input triggered");
    if (!client.connected()){
      remoskoTogglePlay();
    }
    delay(50);
  }

  lastConnected = client.connected();
  lastSit = sit;
}

//makes a HTTP connection to toggle play/pause in the restmote video player 
void remoskoTogglePlay() {
  Serial.println("remoskoTogglePlay");
  if (client.connect(serverRemosko, port)) {
    Serial.println("connecting...");
    client.println("POST " + apiVideoToggle + " HTTP/1.0");
    client.println("Connection: close");
    client.println();
    //lastConnectionTime = millis();
  }  else {
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}


