#include <Servo.h>

char apiKey[45] = "AIzaSyA0WdVqzk4Nw2f08GaEvMU97wvlkufLJiU";

Servo myservo;

String point1 = "50.424536, -104.595330";  // Wascana Parkway & 23th Ave
String point2 = "50.423426, -104.618089";  // 23th Ave & Albert St
String point3 = "50.434832, -104.618060";  // Ablert St & Regina Ave

int ledPins[] = {2,3,4,5,6,7};

int pos = 0;

int input[] = {0,0,0};

int temp;



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

myservo.attach(9);

for(int i = 0; i < 6; i++)
  pinMode(ledPins[i],OUTPUT);
/*
myservo.write(pos);
delay(2000);
myservo.write(90);
delay(2000);*/

}

void loop() {
  // put your main code here, to run repeatedly:

  
  pos = 0;

for(int i = 0; i < 6; i++)
{
  digitalWrite(ledPins[i], LOW);
}

for(int i = 0; i < 3; i++)
{
  input[i] = random(0 , 100);
  pos += input[i];
  Serial.print(input[i]);
  Serial.print("   ");
  if(input[i] < 50)
  {
    temp = input[i] * 1023 / 50;
    analogWrite(ledPins[i*2],temp);
  }
  else
  {
    temp = (input[i] - 50) * 1023 / 50;
    analogWrite(ledPins[i*2+1], temp);
  }
}
Serial.println(" ");

if(pos >= 150)
myservo.write(160);
else
myservo.write(0);
delay(5000);
}


