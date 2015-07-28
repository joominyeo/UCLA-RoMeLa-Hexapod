int senseNum[] = {A0, A2, A3, A5, A1, A4};
int LEDNum[] = {2, 4, 5, 7, 3, 6};
int threshold[] = {5, 5, 5, 5, 5, 5};
int x = 0;

//Right Front == 0
//Right Rear == 1
//Left Front == 2
//Left Rear == 3
//Right Middle == 4
//Left Middle == 5

void setup(){
  for(x = 0; x < 6; x++)
  {
    pinMode(senseNum[x], INPUT); //for the FSRs
    pinMode(LEDNum[x], OUTPUT); //for the LEDs
    pinMode(LEDNum[x] + 6, OUTPUT); //for the Arbotix
  }
  Serial.begin(9600);
}

void loop(){
  if(x == 0){
    Serial.println(" ");
  } else{
    Serial.print(" ");
  }
  Serial.print(analogRead(senseNum[x]));
  if (analogRead(senseNum[x]) > threshold[x])
  {
    digitalWrite(LEDNum[x], HIGH);
    digitalWrite(LEDNum[x] + 6, HIGH);
  }
  else
  {
    digitalWrite(LEDNum[x], LOW);
    digitalWrite(LEDNum[x] + 6, LOW);
  }
  x = (x+1)%6;
}
