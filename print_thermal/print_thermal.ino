#include <Adafruit_Thermal.h>
#define buttonA 5
#define buttonB 19
#define buttonC 23
#define buttonD 4
#define buttonE 18
HardwareSerial mySerial(Serial);
Adafruit_Thermal printer(&mySerial);
int antrian_A = 1;
int antrian_B = 1;
int antrian_C = 1;
int antrian_D = 1;
int antrian_E = 1;
String val_A,val_B,val_C,val_D,val_E;
 
void setup() {
//  Serial.begin(115200);
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);
  mySerial.begin(115200);
  printer.begin();
 

}
void loop() {
 int btn_A = digitalRead(buttonA);
 int btn_B = digitalRead(buttonB);
 int btn_C = digitalRead(buttonC);
 int btn_D = digitalRead(buttonD);
 int btn_E = digitalRead(buttonE);
 sortir_karcis();
// Serial.println(btn_A);
 if(btn_A == 0){
    loker_A();
  }
 if(btn_B == 0){
    loker_B();
  }
 if(btn_C == 0){
    loker_C();
  }
 if(btn_D == 0){
    loker_D();
  }
 if(btn_E == 0){
    loker_E();
  }
}

void sortir_karcis(){

  if (antrian_A < 100 && antrian_A < 10){
    val_A = "A-00" + String(antrian_A);
  }
 else if (antrian_A >= 10 && antrian_A < 100) {
    val_A = "A-0" + String(antrian_A);
  } else {
    val_A = "A-" + String(antrian_A);
    }

 if (antrian_B < 100 && antrian_B < 10){
    val_B = "B-00" + String(antrian_B);
  }
 else if (antrian_B >= 10 && antrian_B < 100) {
    val_B = "B-0" + String(antrian_B);
  } else {
    val_B = "B-" + String(antrian_B);
    }
    
 if (antrian_C < 100 && antrian_C < 10){
    val_C = "C-00" + String(antrian_C);
  }
 else if (antrian_C >= 10 && antrian_C < 100) {
    val_C = "C-0" + String(antrian_C);
  } else {
    val_C = "C-" + String(antrian_C);
    }

 if (antrian_D < 100 && antrian_D < 10){
    val_D = "D-00" + String(antrian_D);
  }
 else if (antrian_D >= 10 && antrian_D < 100) {
    val_D = "D-0" + String(antrian_D);
  } else {
    val_D = "D-" + String(antrian_D);
    }

  if (antrian_E < 100 && antrian_E < 10){
    val_E = "E-00" + String(antrian_E);
  }
 else if (antrian_E >= 10 && antrian_E < 100) {
    val_E = "E-0" + String(antrian_E);
  } else {
    val_E = "E-" + String(antrian_E);
    }
 
  }

void loker_A(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Loket Pendaftaran \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_A);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.sleep();
  delay(1000L);
  printer.wake();
  printer.setDefault();
  antrian_A = antrian_A + 1;
  
  }

void loker_B(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli Umum \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_B);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.sleep();
  delay(1000L);
  printer.wake();
  printer.setDefault();
  antrian_B= antrian_B + 1; 
  }

void loker_C(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli Gizi \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_C);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.sleep();
  delay(1000L);
  printer.wake();
  printer.setDefault();
  antrian_C = antrian_C + 1;
  
  }

void loker_D(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Kandungan Ibu & Anak \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_D);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.sleep();
  delay(1000L);
  printer.wake();
  printer.setDefault();
  antrian_D = antrian_D + 1;
  }

void loker_E(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli Infeksius \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_E);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.sleep();
  delay(1000L);
  printer.wake();
  printer.setDefault();
  antrian_E = antrian_E + 1;
  }
