const int flexPin = A10;

int value;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  value = analogRead(flexPin);
  
  
  Serial.println(value);
  
  delay(5000);
}
