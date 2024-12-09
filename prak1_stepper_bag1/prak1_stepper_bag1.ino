//Memutar motor stepper dengan pelan (step by step)
#include <Stepper.h>

const int stepsPerRevolution = 48;  // nilai banyaknya step per putaran

// menginisialisasi stepper menggunakan pin 8, 9, 10, dan 11 secara berurutan
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int stepCount = 0;         // nilai step

void setup() {
  // inisialisasi serial 9600
  Serial.begin(9600);
}

void loop() {
  // perubahan setiap 1 langkah
  myStepper.step(1);
  Serial.print("steps:");
  Serial.println(stepCount); //menuliskan langkah di serial monitor
  stepCount++;
  delay(500);
}
