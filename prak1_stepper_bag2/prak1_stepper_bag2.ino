//Mengendalikan kecepatan motor stepper
#include <Stepper.h>

const int stepsPerRevolution = 200;  // ubah angka ini sesuai dengan jumlah step dalam 1 putaran


// menginisialisasi stepper menggunakan pin 8, 9, 10, dan 11 secara berurutan
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int stepCount = 0;  // nilai dari step awal

void setup() {
  Serial.print ("Speed Control Motor Stepper");
  delay(1000); //persiapan
}

void loop() {
  // membaca nilai masukkan dari potensio
  int sensorReading = analogRead(A0);
  // proses pembacaan potensio menjadi kecepatan motor
  int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
  // mengatur kecepatan motor berdasarkan potensio
  if (motorSpeed > 0) {
    myStepper.setSpeed(motorSpeed);
    myStepper.step(stepsPerRevolution / 100);
  }
}
