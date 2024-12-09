//Mengendalikan sudut putaran sesuai dengan potensio

#include <Stepper.h>

// ubah angka ini dengan step yang anda butuhkan
#define STEPS 100

// menginisialisasi stepper menggunakan pin 8, 9, 10, dan 11 secara berurutan
Stepper stepper(STEPS, 8, 9, 10, 11);

// kembali membaca nilai masukkan dari potensio
int previous = 0;

void setup() {
  // mengatur kecepatan motor menjadi 30 RPMs
  Serial.begin(9600);
  stepper.setSpeed(30);
}

void loop() {
  // memperoleh nilai dari potensio
  int val = analogRead(0);

  //menyamakan step dengan nilai potensio
  stepper.step(val - previous);
  previous = val;
  Serial.print(val);
  Serial.print("               ");
  Serial.println(previous);
  
}
