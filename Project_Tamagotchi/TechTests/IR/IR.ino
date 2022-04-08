#include <IRLibRecvPCI.h> 
 
IRrecvPCI myReceiver(4);//pin number for the receiver
 
void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals"));
}
 
void loop() {
  //Continue looping until you get a complete signal received
  if (myReceiver.getResults()) { 
    Serial.println("Received");
    myReceiver.enableIRIn();      //Restart receiver
  }
}
