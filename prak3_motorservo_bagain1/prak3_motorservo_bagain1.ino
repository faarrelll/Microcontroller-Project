#include <Servo.h>
Servo servoku;

void setup(){
  servoku.attach(9);
//  servoku.write(180);
//  delay(1000);
//  servoku.write(45);
//  delay(1000);
}

void loop(){
  servoku.write(10);
  delay(1000);
  servoku.write(100);
  delay(1000);
}
