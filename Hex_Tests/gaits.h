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
/* ripple gaits move one leg at a time
 *  for going forward, or turning left/right
 */
#define RIPPLE                  0
#define RIPPLE_SMOOTH           3
#define AMBLE                   4
#define AMBLE_SMOOTH            5
/* tripod gaits are only for hexapods */
#define TRIPOD                  6

#define MOVING   ((Xspeed > 5 || Xspeed < -5) || (Yspeed > 5 || Yspeed < -5) || (Rspeed > 0.05 || Rspeed < -0.05))
/* Standard Transition time should be of the form (k*BIOLOID_FRAME_LENGTH)-1
 *  for maximal accuracy. BIOLOID_FRAME_LENGTH = 33ms, so good options include:
 *   32, 65, 98, etc...
 */
#define STD_TRANSITION          98   //98 for ax-12 hexapod, 32 for ax-18f

#else

/* Simple calculations at the beginning of a cycle. */
void DefaultGaitSetup(){
    // nothing!
}

/* Simple, fast, and rough gait. Legs will make a fast triangular stroke. */
ik_req_t DefaultGaitGen(int leg){
  if( MOVING ){
    // are we moving?
    if(step == gaitLegNo[leg]){ // it looks like "step" is a global variable
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
  }
  if(cycleTime == 0)
    cycleTime = (stepsInCycle*tranTime)/1000.0;
  step = 0;
}

ik_req_t (*gaitGen)(int leg) = &DefaultGaitGen;
void (*gaitSetup)() = &DefaultGaitSetup;

#endif
