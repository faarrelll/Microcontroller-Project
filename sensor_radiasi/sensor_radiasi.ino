/*
  Analog Input

  Demonstrates analog input by reading an analog sensor on analog pin 0 and
  turning on and off a light emitting diode(LED) connected to digital pin 13.
  The amount of time the LED will be on and off depends on the value obtained
  by analogRead().

  The circuit:
  - potentiometer
    center pin of the potentiometer to the analog input 0
    one side pin (either one) to ground
    the other side pin to +5V
  - LED
    anode (long leg) attached to digital output 13 through 220 ohm resistor
    cathode (short leg) attached to ground

  - Note: because most Arduinos have a built-in LED attached to pin 13 on the
    board, the LED is optional.

  created by David Cuartielles
  modified 30 Aug 2011
  By Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
*/

int sensorPin = A0;    // select the input pin for the potentiometer
//int ledPin = 13;      // select the pin for the LED
double sensorValue = 0.0000;  // variable to store the value coming from the sensor

void setup() {
  
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

void loop() {
  
  sensorValue = analogRead(sensorPin);
  double voltage = sensorValue *(5.0 /1023.0);
  double Mili_voltage = voltage/1000;
  Serial.print("Tegangan : ");
  Serial.print(voltage);
  Serial.print(" V");
  Serial.print("          ");
  Serial.print("Tegangan : ");
  Serial.print(Mili_voltage);
  Serial.println(" MV");
 
}