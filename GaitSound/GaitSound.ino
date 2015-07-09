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
  tone(buzzer, 262, 300);
  delay(300);
  tone(buzzer, 392, 300);  //1 C G C
  delay(300);
  tone(buzzer, 262, 300);
  delay(2000);

  tone(buzzer, 262, 300);
  delay(300);
  tone(buzzer, 165, 300);  //2 C E C
  delay(300);
  tone(buzzer, 262, 300);
  delay(2000);
  
  tone(buzzer, 294, 300);
  delay(300);
  tone(buzzer, 294, 300);  //3 D D F
  delay(300);
  tone(buzzer, 175, 300);
  delay(2000);

  tone(buzzer, 262, 300);
  delay(300);
  tone(buzzer, 440, 300);  //4 C A A
  delay(300);
  tone(buzzer, 440, 300);
  delay(2000);

  tone(buzzer, 262, 300);
  delay(300);
  tone(buzzer, 440, 300);  //5 C A F
  delay(300);
  tone(buzzer, 349, 300);
  delay(2000);

  tone(buzzer, 247, 300);
  delay(300);
  tone(buzzer, 330, 300);  //6 B E G
  delay(300);
  tone(buzzer, 292, 300);
  delay(2000);

  tone(buzzer, 262, 300);
  delay(300);
  tone(buzzer, 262, 300);  //7 C C C
  delay(300);
  tone(buzzer, 523, 300);
  delay(2000);

  tone(buzzer, 147, 300);
  delay(300);
  tone(buzzer, 196, 300);  //8 D D G
  delay(300);
  tone(buzzer, 196, 300);
  delay(2000);
}
