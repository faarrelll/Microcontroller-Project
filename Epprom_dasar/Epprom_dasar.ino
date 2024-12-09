#include <EEPROM.h>

int address = 0;
byte value;
int addr = 0;

void setup()
{
Serial.begin(9600);
for (int i=0; i<16; i++)
{
    address=i;
    value = EEPROM.read(address);

    Serial.print(address);
    Serial.print("\t");
    Serial.print(value, DEC);
    Serial.println();
} delay(2000);
}

void loop()
{
int val = analogRead(0);
EEPROM.write(addr, val);
Serial.print(val);
Serial.print("\t");

address=addr;
value = EEPROM.read(address);

Serial.print(address);
Serial.print("\t");
Serial.print(value, DEC);
Serial.println();
delay(1000);

addr = addr + 1;
if (addr == 16)  exit(0);
}
