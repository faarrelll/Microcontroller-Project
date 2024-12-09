
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
char text;
void setup() {
  radio.begin();
  Serial.begin(9600);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.print("Masukan data = ");

}

void loop() {
  
  if (Serial.available() > 0) {
    text = Serial.read();
    Serial.println(text);
    radio.write(&text, sizeof(text));
      
  }
  
  


}
