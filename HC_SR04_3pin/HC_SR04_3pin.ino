#define pingPin 3

void setup() {
  Serial.begin (9600);
  delay(500);
}

void loop() {
  float duration, distance;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);

  duration = pulseIn(pingPin, HIGH);
  distance = duration /29 /2;
  Serial.print("distance : ");
  Serial.println(distance);
  delay(1000);
}
