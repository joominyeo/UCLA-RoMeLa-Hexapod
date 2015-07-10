//semi-code stuff

//establish sensor position
#define FSR  A1

//walk until the FSR reaches the threshold value
while (analogRead(FSR) < 500){
  step downwards
}

//set the new endpoints to the leg's current position
endpoints[leg].x = gaits[leg].x;
endpoints[leg].y = gaits[leg].y;
endpoints[leg].z = gaits[leg].z;
