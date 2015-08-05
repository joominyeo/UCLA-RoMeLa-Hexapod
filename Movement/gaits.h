/******************************************************************************
 * Gaits Auto-Generated by NUKE!
 *   http://arbotix.googlecode.com
 *****************************************************************************/

/* This is a bit funky -- NEVER INCLUDE gaits.h in your file! */
#ifndef GAIT_H
#define GAIT_H

/* find the translation of the endpoint (x,y,z) given our gait parameters */
extern ik_req_t (*gaitGen)(int leg);
extern void (*gaitSetup)();
extern ik_req_t endpoints[LEG_COUNT];

extern int senseGait;
extern bool downMove;

extern int offsetY;
extern int offsetX;
extern int offsetDirection[];

extern int inputs[]; //inputs from the Arduino

extern int leftZ;
extern int leftY;
extern int rightZ;
extern int rightY;
extern int rollAngle;

extern int totalSteps;

/* ripple gaits move one leg at a time
 *  for going forward, or turning left/right
 */
#define RIPPLE                  0
#define RIPPLE_SMOOTH           3
#define AMBLE                   4
#define AMBLE_SMOOTH            5
/* tripod gaits are only for hexapods */
#define TRIPOD                  6
/* movement gait is only for all six legs simultaneously */
#define MOVEMENT_PLANE          7
#define MOVEMENT_ROT            8

/* custom gait; RIP*/
#define SQUARE_GAIT             9

#define MOVING   ((Xspeed > 5 || Xspeed < -5) || (Yspeed > 5 || Yspeed < -5) || (Rspeed > 0.05 || Rspeed < -0.05))
/* Standard Transition time should be of the form (k*BIOLOID_FRAME_LENGTH)-1
 *  for maximal accuracy. BIOLOID_FRAME_LENGTH = 33ms, so good options include:
 *   32, 65, 98, etc...
 */
#define STD_TRANSITION          98   //98 for ax-12 hexapod, 32 for ax-18f

#define MAX_OFFSET 60 // maximum that the leg will deviate in the x and y direction

#else

/* Simple calculations at the beginning of a cycle. */
void DefaultGaitSetup(){
    // nothing!
}
/*For moving all 6 legs simultaneously*/
ik_req_t MovementPlaneGen(int leg){
  if( MOVING ){
    if (step == gaitLegNo[leg]){
    bodyPosX -= (Xspeed/16);
    bodyPosY -= (Yspeed/16);
      /*gaits[leg].x = -((Xspeed) * 2);
    gaits[leg].y = -((Yspeed) * 2);*/
    gaits[leg].z = Zspeed;
  }
}else{

  }
  return gaits[leg];
}

ik_req_t MovementRotGen(int leg){
    if (step == gaitLegNo[leg]){
    bodyRotX = (Yspeed * 3.141592654 / 500);
    bodyRotY = (Xspeed * 3.141592654 / 500);
    bodyRotZ = (Rspeed * 3.141592654 / 10);
  }
  return gaits[leg];
}

//Basis for the sensing gait
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
        // moves the leg down until touching the ground
        if (digitalRead(inputs[leg]) != 1){
          downMove = true;
          gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle) + (offsetX * (Xspeed/abs(Xspeed)));
          gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle) + (offsetY * offsetDirection[leg]);
          gaits[leg].z = (gaits[leg].z + DROP_SPEED);
          gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
          if (gaits[leg].z > liftHeight + maxLift){
            gaits[leg].z = -liftHeight;
            offsetY = (offsetY+yChange)%(MAX_OFFSET);
            if (offsetY == 0){
              offsetX = (offsetX+xChange)%(MAX_OFFSET);
            }
          }
        }else{
          downMove = false;
          tone(BUZZER, 262, 50);
          step = (step+1)%stepsInCycle;
        }
    }else if ((step == gaitLegNo[leg]+3) || (step == gaitLegNo[leg]-(stepsInCycle-3))){
      //moves the leg up slowly until barely not touching the ground
      if (digitalRead(inputs[leg]) == 1){
        gaits[leg].x = (gaits[leg].x - (Xspeed*cycleTime)/(4*stepsInCycle)) + (offsetX * (Xspeed/abs(Xspeed)));
        gaits[leg].y = (gaits[leg].y - (Yspeed*cycleTime)/(4*stepsInCycle)) + (offsetY * offsetDirection[leg]);
        gaits[leg].z = (gaits[leg].z - (DROP_SPEED/DROP_SPEED));
        gaits[leg].r = (gaits[leg].r - (Rspeed*cycleTime)/(4*stepsInCycle));
      }else{
        tone(BUZZER, 523, 50);
        points[leg].z = gaits[leg].z;
        step = (step+1)%stepsInCycle;
        totalSteps ++;
      }
    }else{
      if (downMove == false){
      // move body forward
      gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/(4*stepsInCycle);
      gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/(4*stepsInCycle);
      gaits[leg].z = points[leg].z;
      gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/(4*stepsInCycle);
      }
    }
  }else{//stopped
    points[leg].z = gaits[leg].z;
  }
  return gaits[leg];
}

/* Simple, fast, and rough gait. Legs will make a fast triangular stroke. */
ik_req_t DefaultGaitGen(int leg){
  if( MOVING ){
    // are we moving?
    if(step == gaitLegNo[leg]){
      // leg up, middle position
      gaits[leg].x = 0;
      gaits[leg].y = 0;
      gaits[leg].z = -liftHeight;
      gaits[leg].r = 0;
    }else if(((step == gaitLegNo[leg]+1) || (step == gaitLegNo[leg]-(stepsInCycle-1))) && (gaits[leg].z < 0)){
      // leg down position                                           NOTE: dutyFactor = pushSteps/StepsInCycle
      gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);     // travel/Cycle = speed*cycleTime
      gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);     // Stride = travel/Cycle * dutyFactor
      gaits[leg].z = 0;                                                 //   = speed*cycleTime*pushSteps/stepsInCycle
      gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);     //   we move Stride/2 here
    }else{
      // move body forward
      gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;    // note calculations for Stride above
      gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;    // we have to move Stride/pushSteps here
      gaits[leg].z = 0;                                                 //   = speed*cycleTime*pushSteps/stepsInCycle*pushSteps
      gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;    //   = speed*cycleTime/stepsInCycle
    }
  }else{ // stopped
    gaits[leg].z = 0;
  }
  return gaits[leg];
}

/* Smoother, slower gait. Legs will make a arc stroke. */
ik_req_t SmoothGaitGen(int leg){
  if( MOVING ){
    // are we moving?
    if(step == gaitLegNo[leg]){
      // leg up, halfway to middle
      gaits[leg].x = gaits[leg].x/2;
      gaits[leg].y = gaits[leg].y/2;
      gaits[leg].z = -liftHeight/2;
      gaits[leg].r = gaits[leg].r/2;
    }else if((step == gaitLegNo[leg]+1) && (gaits[leg].z < 0)){
      // leg up position
      gaits[leg].x = 0;
      gaits[leg].y = 0;
      gaits[leg].z = -liftHeight;
      gaits[leg].r = 0;
    }else if((step == gaitLegNo[leg] + 2) && (gaits[leg].z < 0)){
      // leg halfway down
      gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].z = -liftHeight/2;
      gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
    }else if((step == gaitLegNo[leg]+3) && (gaits[leg].z < 0)){
      // leg down position                                           NOTE: dutyFactor = pushSteps/StepsInCycle
      gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(2*stepsInCycle);     // travel/Cycle = speed*cycleTime
      gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(2*stepsInCycle);     // Stride = travel/Cycle * dutyFactor
      gaits[leg].z = 0;                                                 //   = speed*cycleTime*pushSteps/stepsInCycle
      gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(2*stepsInCycle);     //   we move Stride/2 here
    }else{
      // move body forward
      gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/stepsInCycle;    // note calculations for Stride above
      gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/stepsInCycle;    // we have to move Stride/pushSteps here
      gaits[leg].z = 0;                                                 //   = speed*cycleTime*pushSteps/stepsInCycle*pushSteps
      gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/stepsInCycle;    //   = speed*cycleTime/stepsInCycle
    }
  }else{ // stopped
    gaits[leg].z = 0;
  }
  return gaits[leg];
}

int currentGait = -1;

void gaitSelect(int GaitType){
  if(GaitType == currentGait)
    return;
  liftHeight = 50;
  currentGait = GaitType;
  tranTime = STD_TRANSITION;
  cycleTime = 0;
  // simple ripple, 12 steps
  if(GaitType == RIPPLE){
    gaitGen = &DefaultGaitGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_REAR] = 2;
    gaitLegNo[LEFT_MIDDLE] = 4;
    gaitLegNo[LEFT_FRONT] = 6;
    gaitLegNo[RIGHT_REAR] = 8;
    gaitLegNo[RIGHT_MIDDLE] = 10;
    pushSteps = 10;
    stepsInCycle = 12;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 262, 100);  //1 .---- C
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    //tone(BUZZER, 262, 100);
    //delay(150);
  }else if(GaitType == RIPPLE_SMOOTH){
    gaitGen = &SmoothGaitGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_REAR] = 4;
    gaitLegNo[LEFT_MIDDLE] = 8;
    gaitLegNo[LEFT_FRONT] = 12;
    gaitLegNo[RIGHT_REAR] = 16;
    gaitLegNo[RIGHT_MIDDLE] = 20;
    pushSteps = 20;
    stepsInCycle = 24;
    tranTime = 65;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 262, 100);  //2 ..--- D
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    //tone(BUZZER, 294, 100);
    //delay(150);
  }else if(GaitType == AMBLE_SMOOTH){
    gaitGen = &SmoothGaitGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_REAR] = 0;
    gaitLegNo[LEFT_FRONT] = 4;
    gaitLegNo[RIGHT_REAR] = 4;
    gaitLegNo[RIGHT_MIDDLE] = 8;
    gaitLegNo[LEFT_MIDDLE] = 8;
    pushSteps = 8;
    stepsInCycle = 12;
    tranTime = 65;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 262, 100);  //3 ...-- E
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    //tone(BUZZER, 330, 100);
    //delay(150);
  }else if(GaitType == AMBLE){
    gaitGen = &DefaultGaitGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_REAR] = 0;
    gaitLegNo[LEFT_FRONT] = 2;
    gaitLegNo[RIGHT_REAR] = 2;
    gaitLegNo[RIGHT_MIDDLE] = 4;
    gaitLegNo[LEFT_MIDDLE] = 4;
    pushSteps = 4;
    stepsInCycle = 6;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 392, 100);  //4 ....- F
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    //tone(BUZZER, 349, 100);
    //delay(150);
  }else if(GaitType == TRIPOD){
    gaitGen = &DefaultGaitGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_MIDDLE] = 0;
    gaitLegNo[RIGHT_REAR] = 0;
    gaitLegNo[LEFT_FRONT] = 2;
    gaitLegNo[RIGHT_MIDDLE] = 2;
    gaitLegNo[LEFT_REAR] = 2;
    pushSteps = 2;
    stepsInCycle = 4;
    tranTime = 65;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 392, 100);  //5 ..... G
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    //tone(BUZZER, 392, 100);
    //delay(150);
  }else if(GaitType == MOVEMENT_PLANE){
    gaitGen = &MovementPlaneGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_MIDDLE] = 0;
    gaitLegNo[RIGHT_REAR] = 0;
    gaitLegNo[LEFT_FRONT] = 0;
    gaitLegNo[RIGHT_MIDDLE] = 0;
    gaitLegNo[LEFT_REAR] = 0;
    pushSteps = 0; //Is this right?
    stepsInCycle = 1;
   // bodyPosX = 0;
   // bodyPosY = 0;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 392, 100);  //6 -.... A
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    //tone(BUZZER, 440, 100);
    //delay(150);
  }else if(GaitType == MOVEMENT_ROT){
    gaitGen = &MovementRotGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_MIDDLE] = 0;
    gaitLegNo[RIGHT_REAR] = 0;
    gaitLegNo[LEFT_FRONT] = 0;
    gaitLegNo[RIGHT_MIDDLE] = 0;
    gaitLegNo[LEFT_REAR] = 0;
    pushSteps = 0; //Is this right?
    stepsInCycle = 1;
   // bodyRotX = 0;
   // bodyRotY = 0;
   // bodyRotZ = 0;
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 392, 100);  //7 -.... A
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    tone(BUZZER, 392, 100);
    delay(150);
    //tone(BUZZER, 440, 100);
    //delay(150);
  }else if(GaitType == SQUARE_GAIT){
    bodyPosX = 20;
    senseGait = 1;
    liftHeight = 65;
    cycleTime = 0;
    gaitGen = &SquareGaitGen;
    gaitSetup = &DefaultGaitSetup;
    gaitLegNo[RIGHT_FRONT] = 0;
    gaitLegNo[LEFT_REAR] = 25;
    gaitLegNo[LEFT_MIDDLE] = 20;
    gaitLegNo[LEFT_FRONT] = 15;
    gaitLegNo[RIGHT_REAR] = 10;
    gaitLegNo[RIGHT_MIDDLE] = 5;
    pushSteps = 25;
    stepsInCycle = 30;
    tranTime = 65;

    tone(BUZZER, 392, 100);
    delay(150);
    tone(BUZZER, 262, 100);  //8 --... B
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    tone(BUZZER, 262, 100);
    delay(150);
    //tone(BUZZER, 494, 100);
    //delay(150);
  }

  if(cycleTime == 0)
    cycleTime = (stepsInCycle*tranTime)/1000.0;
  step = 0;
}

ik_req_t (*gaitGen)(int leg) = &DefaultGaitGen;
void (*gaitSetup)() = &DefaultGaitSetup;

#endif
