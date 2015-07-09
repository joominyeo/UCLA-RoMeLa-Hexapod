#define BUZZER A0

void setup(){
  pinMode(0,OUTPUT);
  pinMode(BUZZER, OUTPUT);
  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 262, 300);
  delay(300);
}

void loop()
{
  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 392, 300);  //1 C G C
  delay(300);
  tone(BUZZER, 262, 300);
  delay(300);

  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 165, 300);  //2 C E C
  delay(300);
  tone(BUZZER, 262, 300);
  delay(300);

  tone(BUZZER, 294, 300);
  delay(300);
  tone(BUZZER, 294, 300);  //3 D D F
  delay(300);
  tone(BUZZER, 175, 300);
  delay(300);

  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 440, 300);  //4 C A A
  delay(300);
  tone(BUZZER, 440, 300);
  delay(300);

  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 440, 300);  //5 C A F
  delay(300);
  tone(BUZZER, 349, 300);
  delay(300);

  tone(BUZZER, 247, 300);
  delay(300);
  tone(BUZZER, 330, 300);  //6 B E G
  delay(300);
  tone(BUZZER, 292, 300);
  delay(300);

  tone(BUZZER, 262, 300);
  delay(300);
  tone(BUZZER, 262, 300);  //7 C C C
  delay(300);
  tone(BUZZER, 523, 300);
  delay(300);

  tone(BUZZER, 147, 300);
  delay(300);
  tone(BUZZER, 196, 300);  //8 D D G
  delay(300);
  tone(BUZZER, 196, 300);
  delay(300);
}
