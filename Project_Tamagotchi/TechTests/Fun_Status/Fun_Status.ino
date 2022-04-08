
/*
MKR1000 connecting to MQTT broker test.mosquitto.org

MQTT protocol will be used for communication
  MKR1000 --  MQTT broker --- ComApp --- ProtoPie
 */

#include <SPI.h>
#include <WiFiSSLClient.h>
#include <WiFi101.h>

#include <MQTTClient.h>

// WLAN 
const char ssid[] = "HAMKvisitor"; //  your network SSID (name)
const char pass[] = "hamkvisitor";    // your network password (use for WPA)

//char *client_id = "<write here a unique client ID>";
char *client_id = "Id_Tama_01";    // needs to be unique !!
char *user_id = "";   // empty if no username and password will be used.
char *authToken = ""; // empty if no username and password will be used.

//char *hostname = "<your mqtt broker server name or IP address>";
char *hostname = "test.mosquitto.org";
int pubInterval = 1000;

const int OnOffButton = A6;           //
const int fadePinStatus = 5;          // must be a pin that supports PWM. 0...8 on MKR1000

// PWM steps per fade step.  More fades faster; less fades slower.
const int pwmStepsPerFade = 14;

int fadePWM;

//use this class if you connect using SSL WiFiSSLClient net;
WiFiClient net;
MQTTClient MQTTc;

unsigned long lastMillis = 0; 

void setup() 
{
  Serial.begin(9600);
  delay(5000);    // waiting to let you open the serial monitor
  WiFi.begin(ssid, pass);
  Serial.println("Connected to WLAN");
  printWiFiStatus();
  
 //Connecting to the MQTT broker client.begin("<hostname>", 1883, net);
  MQTTc.begin(hostname, 1883, net);
  MQTTc.onMessage(messageReceived);
  connect();
  
// Set up the I/O pins
  pinMode(fadePinStatus, OUTPUT);
  pinMode(OnOffButton,INPUT);
  fadePWM = 255;
  analogWrite(fadePinStatus, fadePWM);   // sets PWM duty cycle
}

void loop() {
   MQTTc.loop();  // Cut for testing without mqtt broker
 
  analogWrite(fadePinStatus, fadePWM);
  
     // publish a message every nnn milli seconds
     if(millis() - lastMillis > pubInterval) 
     {
      Serial.println("Publishing to MQTT broker...");
        if(!MQTTc.connected()) {    // Cut for testing without MQTT
         connect();                 // Cut for testing without MQTT
        }                           // Cut for testing without MQTT
        lastMillis = millis();
         //Cut for testing without MQTT
         //MQTTc.publish("try-me", "[BALL]:" + String(analogRead(ainputPin)));  
//         MQTTc.publish("try-me", "[BALL]:" + String(analogRead(ainputPin)));  
        //fadePWM = 255;
     }
    Serial.print("Voltage  ");
//    Serial.println(analogRead(ainputPin));
    delay(500);
    
// end of loop
}

void connect() 
{
  Serial.print("checking WLAN...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Connecting WLAN...  ");
    Serial.print(".");
    delay(500);
  }
 
  Serial.print("\nconnecting MQTT broker....");
  while (!MQTTc.connect(client_id,user_id,authToken)) 
  {
    Serial.print(".");
    delay(3000);
  }
  Serial.println("\nconnected!");

  MQTTc.subscribe("push-me");
}

void messageReceived(String &topic, String &payload) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();

  // String content is received from a ProtoPie connectd app. 
  // There is a certain structure included in the MQTT payload.
  String message = payload.substring(0, payload.indexOf(":"));
  String value = payload.substring(payload.indexOf(":")+1,payload.length());
  if(message == "[LED]"){
    fadePWM = value.toInt();
   
  }
  if(message == "[LEDON]"){
    fadePWM = 255;
  }
  if(message == "[LEDOFF]"){
    fadePWM = 0;
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
