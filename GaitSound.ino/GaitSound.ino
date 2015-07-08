#define buzzer A0

void setup(){
  pinMode(0,OUTPUT);
  pinMode(buzzer, OUTPUT);
  tone(buzzer, 262, 2000);
  delay(2000);
  tone(buzzer, 262, 2000);
  delay(2000);
  tone(buzzer, 262, 2000);
  delay(2000);
}

void loop()
{
  tone(buzzer, 262, 500);
  delay(200);
  tone(buzzer, 392, 500);  //1 C G C
  delay(200);
  tone(buzzer, 262, 500);
  delay(2000);
  
  tone(buzzer, 262, 500);
  delay(200);
  tone(buzzer, 165, 500);  //2 C E C
  delay(200);
  tone(buzzer, 262, 500);
  delay(2000);
  
  tone(buzzer, 294, 500);
  delay(200);
  tone(buzzer, 294, 500);  //3 D D F
  delay(200);
  tone(buzzer, 175, 500);
  delay(2000);
  
  tone(buzzer, 262, 500);
  delay(200);
  tone(buzzer, 440, 500);  //4 C A A
  delay(200);
  tone(buzzer, 440, 500);
  delay(2000);
  
  tone(buzzer, 262, 500);
  delay(200);
  tone(buzzer, 440, 500);  //5 C A F
  delay(200);
  tone(buzzer, 349, 500);
  delay(2000);
  
  tone(buzzer, 247, 500);
  delay(200);
  tone(buzzer, 330, 500);  //6 B E G
  delay(200);
  tone(buzzer, 292, 500);
  delay(2000);
  
  tone(buzzer, 262, 500);
  delay(200);
  tone(buzzer, 262, 500);  //7 C C C
  delay(200);
  tone(buzzer, 523, 500);
  delay(2000);
  
  tone(buzzer, 147, 500);
  delay(200);
  tone(buzzer, 196, 500);  //8 D D G
  delay(200);
  tone(buzzer, 196, 500);
  delay(2000);
}
