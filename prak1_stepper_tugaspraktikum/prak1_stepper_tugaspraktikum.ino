#include <Stepper.h>

const int stepsPerRevolution = 48;   // nilai banyaknya step per putaran
const int potPin = A0;               // pin analog untuk membaca potensiometer
const int minDelay = 10;             // delay minimum antar langkah

// menginisialisasi stepper menggunakan pin 8, 9, 10, dan 11 secara berurutan
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int stepDelay = 100;                 // delay antar langkah, default 100ms

void setup() {
  // inisialisasi serial 9600
  Serial.begin(9600);
}

void loop() {
  // membaca nilai potensiometer dan mengubah nilai delay sesuai dengan nilai potensiometer
  int potValue = analogRead(potPin);
  stepDelay = map(potValue, 0, 1023, minDelay, 500);

  // gerakan ke sudut 90 derajat
  myStepper.setSpeed(200);           // kecepatan motor stepper dalam satuan rpm
  myStepper.step(12);                // jumlah langkah untuk mencapai sudut 90 derajat

  // gerakan ke sudut 270 derajat
  myStepper.setSpeed(100);           // kecepatan motor stepper dalam satuan rpm
  myStepper.step(-36);               // jumlah langkah untuk mencapai sudut 270 derajat
  
  // menunda untuk beberapa saat
  delay(stepDelay);
}
