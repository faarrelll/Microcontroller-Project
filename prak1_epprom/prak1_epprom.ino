#include <EEPROM.h>
byte nilai[5]= {
  0x11,0x22,0x33,0x44,0x55
  };
int address = 0;
byte value;
int addr = 0;
int i;
int a;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Menampilkan Nilai EEPROM ---------------------");
   for (a=0; a<5; a++)
{
    address=a;
    value = EEPROM.read(address);
    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println(); 
    
} delay(2000);
  Serial.println("di inputkan ---------------------");
  for (i=0; i<5; i++)
{
    address=i;
    EEPROM.write(addr, nilai[i]);
    Serial.print(address);
    Serial.print("\t");
    Serial.print(nilai[i], DEC);
    Serial.println(); 
    addr = addr + 1;
} delay(2000);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
   
    

}
