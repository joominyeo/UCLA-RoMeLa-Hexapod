// Code for reading analog inputs from FSRs, outputting to serial and piping them to the Arbotix.
// Used because the Arbotix was having some issues reading analog values.

//#define SERIAL_ON

int senseNum[] = {A0, A2, A3, A5, A1, A4}; // analog pins
int LEDNum[] = {2, 4, 5, 7, 3, 6}; // digital pins
int threshold[] = {1, 1, 1, 50, 1, 1}; // threshold for each of the pins

char* legs[] = {" Right Front: ", " Right Rear: ", " Left Front: ", " Left Rear: ", " Right Middle: ", " Left Middle: "};

// Right Front  == 0
// Right Rear   == 1
// Left Front   == 2
// Left Rear    == 3
// Right Middle == 4
// Left Middle  == 5

void setup(){
  // initialize all of the pins
  for(int x = 0; x < 6; x++)
  {
    pinMode(senseNum[x], INPUT); //for the FSRs
    pinMode(LEDNum[x], OUTPUT); //for the LEDs
    pinMode(LEDNum[x] + 6, OUTPUT); //for the Arbotix
  }


  Serial.begin(115200);

}

unsigned time = millis();

void loop(){
  // Loop Timer
  Serial.print("EXEC TIME: "); Serial.print(millis()-time);
  time = millis();

  //
  for(int x = 0; x < 6; x++){
    // print out leg enum
    int reading = analogRead(senseNum[x]);

#ifdef SERIAL_ON
    Serial.print(legs[x]);
    Serial.print(reading);
#endif

    // if above threshold, turn on LED and send HIGH to Arbotix
    if (reading > threshold[x])
    {
      digitalWrite(LEDNum[x], HIGH);
      digitalWrite(LEDNum[x] + 6, HIGH);
    }
    else
    {
      digitalWrite(LEDNum[x], LOW);
      digitalWrite(LEDNum[x] + 6, LOW);
    }
  }
  Serial.println();
}
