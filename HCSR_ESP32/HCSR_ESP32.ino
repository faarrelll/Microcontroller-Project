#define triggerPin  5
#define echoPin     18
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distanceCm = duration*SOUND_SPEED/2;

  distanceInch = distanceCm * CM_TO_INCH;

  Serial.print("distance (cm) :");
  Serial.println(distanceCm);
  Serial.print("distance (Inch) :");
  Serial.println(distanceInch);

  delay(1000);
}
