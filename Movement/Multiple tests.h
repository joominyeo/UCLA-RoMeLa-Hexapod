//this variant depends on the input from each sensor in their respective foot

int senseNum[] = {A1, A2, A3, A4, A5, A6};
int offsetDirection[] = {1, 1, -1, -1, 1, -1}
int offsetY = 0;
int offsetX = 0;
#define maxOffset  40
#define thresholdValue  15

ik_req_t SquareGaitGen(int leg){
   if( MOVING ){
    if(step == gaitLegNo[leg]){
      // leg up, first position
      gaits[leg].x = gaits[leg].x;
      gaits[leg].y = gaits[leg].y;
      gaits[leg].z = -liftHeight;
      gaits[leg].r = 0;
    }else if(((step == gaitLegNo[leg]+1) || (step == gaitLegNo[leg]-(stepsInCycle-1))) && (gaits[leg].z < 0)){
      // leg up, second position
      offsetX = 0;
      offsetY = 0;
      gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].z = -liftHeight;
      gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      }else if ((step == gaitLegNo[leg]+2) || (step == gaitLegNo[leg]-(stepsInCycle-2))){
        // leg down position
        if (digitalRead(senseNum[leg]) < thresholdValue){
        downMove = 1;
        gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle) + offsetX;
        gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle) + (offsetY * offsetDirection[leg]);
        gaits[leg].z = (gaits[leg].z + dropSpeed);
        gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
        if (gaits[leg].z > liftHeight){
          gaits[leg].z = -liftHeight;
          //offsetX += (floor((offsetY/maxOffset) + (20/maxOffset))*20);
          offsetY = (offsetY+20)%(maxOffset);
          offsetX += (abs(ceil(offsetY/maxOffset) - 1) * 10);//Verified to work mathematically; if the y offset reaches its max, then the x offset will increase by 10 and the cycle will restart
          //maybe change the max offset by -20 every time the leg moves, that way the gait steps in a triangular pattern rather than a rectangular one
        }
        }else{
          downMove = 0;
          tone(BUZZER, 523, 100);
          points[leg].z = gaits[leg].z;
          step = (step+1)%stepsInCycle;
        }
    }else{
      if (downMove == 0){
      // move body forward
      gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/(2*stepsInCycle);
      gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/(2*stepsInCycle);
      gaits[leg].z = points[leg].z;
      gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/(2*stepsInCycle);
      }
    }
  }else{//stopped
    gaits[leg].z = points[leg].z;
    //digitalWrite(1, LOW);
  }
  return gaits[leg];
}

------------------------------------------------------------------------------------------------------------------------------------

//Off course? Turn a little bit and get straight. Rather than stopping, continuously shift direction

int direction = [initial compass degrees or whatever];

Rspeed = (abs(direction - [current compass direction]) / (direction - [current compass direction])) * (1 / 20); //just plug this into the movement function
/*
if (([current compass direction] > (direction + 5)) || ([current compass direction] < (direction - 5))){
  Xspeed = 0;
  Rspeed = (abs(direction - [current compass direction]) / (direction - [current compass direction])) * (1 / 20);
}
else{
  Rspeed = 0;
  Xspeed = 125;
  //run the rest of the movement code
}*/
------------------------------------------------------------------------------------------------------------------------------------

//legs higher than others? adjust the body angle
int frontZ;
int frontX;
int midZ;
int midX;
int rearZ;
int rearX;
int pitchAngle;

int pitchCalc(){
  midZ = (gaits[RIGHT_MIDDLE].z + gaits[LEFT_MIDDLE].z)/2;
  midX = (gaits[RIGHT_MIDDLE].x + gaits[LEFT_MIDDLE].x)/2;
  frontZ = ((gaits[RIGHT_FRONT].z + gaits[LEFT_FRONT].z)/2) - midZ;
  frontX = ((gaits[RIGHT_FRONT].x + gaits[LEFT_FRONT].x)/2) - midX;
  rearZ = ((gaits[RIGHT_REAR].z + gaits[LEFT_REAR].z)/2) - midZ;
  rearX = ((gaits[RIGHT_REAR].x + gaits[LEFT_REAR].x)/2) - midX;
  pitchAngle = (abs(atan2(frontZ, frontX)) + abs(atan2(rearZ, rearX)))/2;
  return pitchAngle;
}

int leftZ;
int leftY;
int rightZ;
int rightY;
int rollAngle;

int rollCalc(){
  leftZ = (gaits[LEFT_FRONT].z + gaits[LEFT_MIDDLE].z + gaits[LEFT_REAR].z)/3;
  leftY = (gaits[LEFT_FRONT].y + gaits[LEFT_MIDDLE].y + gaits[LEFT_REAR].y)/3;
  rightZ = (gaits[RIGHT_FRONT].z + gaits[RIGHT_MIDDLE].z + gaits[RIGHT_REAR].z)/3;
  rightY = (gaits[RIGHT_FRONT].y + gaits[RIGHT_MIDDLE].y + gaits[RIGHT_REAR].y)/3;
  rollAngle = (atan2(leftZ - rightZ, leftY - rightY));
  return rollAngle;
}

bodyRotX = rollCalc();
bodyRotY = pitchCalc();

------------------------------------------------------------------------------------------------------------------------------------

//all 3 together
int senseNum[] = {A1, A2, A3, A4, A5, A6};
int offsetDirection[] = {1, 1, -1, -1, 1, -1}
int offsetY = 0;
int offsetX = 0;
#define maxOffset  40
#define thresholdValue  15

int direction = [initial compass degrees or whatever];

int frontZ;
int frontX;
int midZ;
int midX;
int rearZ;
int rearX;
int pitchAngle;

int leftZ;
int leftY;
int rightZ;
int rightY;
int rollAngle;

if (([current compass direction] > (direction + 5)) || ([current compass direction] < (direction - 5))){
  Xspeed = 0;
  Rspeed = (abs(direction - [current compass direction]) / (direction - [current compass direction])) * (1 / 20);
}else{
  Rspeed = 0;
  Xspeed = 125;
  ik_req_t SquareGaitGen(int leg){
     if( MOVING ){
      if(step == gaitLegNo[leg]){
        // leg up, first position
        gaits[leg].x = gaits[leg].x;
        gaits[leg].y = gaits[leg].y;
        gaits[leg].z = -liftHeight;
        gaits[leg].r = 0;
      }else if(((step == gaitLegNo[leg]+1) || (step == gaitLegNo[leg]-(stepsInCycle-1))) && (gaits[leg].z < 0)){
        // leg up, second position
        offsetX = 0;
        offsetY = 0;
        gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
        gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
        gaits[leg].z = -liftHeight;
        gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
        }else if ((step == gaitLegNo[leg]+2) || (step == gaitLegNo[leg]-(stepsInCycle-2))){
          // leg down position
          if (digitalRead(senseNum[leg]) < thresholdValue){
          downMove = 1;
          gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle) + offsetX;
          gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle) + (offsetY * offsetDirection[leg]);
          gaits[leg].z = (gaits[leg].z + dropSpeed);
          gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
          if (gaits[leg].z > liftHeight){
            gaits[leg].z = -liftHeight;
            offsetX += (floor((offsetY/maxOffset) + (10/maxOffset))*10); //Verified to work mathematically; if the y offset reaches its max, then the x offset will increase by 10 and the cycle will restart
            offsetY = (offsetY+10)%(maxOffset);
            //maybe change the max offset by -10 every time the leg moves, that way the gait steps in a triangular pattern rather than a rectangular one
          }
          }else{
            bodyRotX = rollCalc();
            bodyRotY = pitchCalc();
            downMove = 0;
            tone(BUZZER, 523, 100);
            points[leg].z = gaits[leg].z;
            step = (step+1)%stepsInCycle;
          }
      }else{
        if (downMove == 0){
        // move body forward
        gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/(2*stepsInCycle);
        gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/(2*stepsInCycle);
        gaits[leg].z = points[leg].z;
        gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/(2*stepsInCycle);
        }
      }
    }else{//stopped
      gaits[leg].z = points[leg].z;
    }
    return gaits[leg];
  }
}

--------------------------------------------------------------------------------------------------------------------------------------------

//set the body rotation angle to the opposite of that which the IMU reads
//e.g. the IMU reads 8 degrees of roll, so the bodyRotX will change by -8 degrees.
//e.g. the IMU reads 34 degrees of pitch, so the bodyRotY will change by -34 degrees.
bodyRotX -= [RollIMU * 3.141592654 / 180];
bodyRotY -= [PitchIMU * 3.141592654 / 180];
//depending on the roll/pitch/yaw side, add tan(theta)/2 to the lower and subtract the same to the : maybe in doIK()
