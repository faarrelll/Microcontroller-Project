#define water A0
#define buz D3
void setup()
{
    pinMode(water,INPUT);
    pinMode(buz,OUTPUT);
    Serial.begin(9600);
    delay(10);
 
    // We start by connecting to a WiFi network

}
 

void loop()
{
    int s1 = analogRead(water);
    Serial.println(s1);
    if(s1>350){
      digitalWrite(buz,HIGH);
      } else {
        digitalWrite(buz,LOW);
        }
 
}
