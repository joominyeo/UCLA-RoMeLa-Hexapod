ik_req_t SquareGaitGen(int leg){
   if( MOVING ){
    if(step == gaitLegNo[leg]){
      // leg up, first position
      gaits[leg].x = 0;
      gaits[leg].y = 0;
      //gaits[leg].z = (points[leg].z - liftHeight);
      gaits[leg].z = -liftHeight;
      gaits[leg].r = 0;
    }else if(((step == gaitLegNo[leg]+1) || (step == gaitLegNo[leg]-(stepsInCycle-1))) && (gaits[leg].z < 0)){
      // leg up, second position
      gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      //gaits[leg].z = (points[leg].z - liftHeight);
      gaits[leg].z = -liftHeight;
      gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);

  /*  }else if((((step == gaitLegNo[leg]+2) || (step == gaitLegNo[leg]-(stepsInCycle-2))) && (gaits[leg].z < 0)) /* || analogRead(leg) != [threshold value]*///){
      // leg down position
      /*gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
      gaits[leg].z = 0;
      gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle); */
      }else if ((step == gaitLegNo[leg]+2) || (step == gaitLegNo[leg]-(stepsInCycle-2))){
        // leg down position
        if (digitalRead(sensorValue) == LOW){
        downMove = 1;
        gaits[leg].x = (Xspeed*cycleTime*pushSteps)/(4*stepsInCycle);
        gaits[leg].y = (Yspeed*cycleTime*pushSteps)/(4*stepsInCycle);
        gaits[leg].z = (gaits[leg].z + dropSpeed);
        gaits[leg].r = (Rspeed*cycleTime*pushSteps)/(4*stepsInCycle);
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
          if (digitalRead(sensorValue) == LOW){
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
            downMove = 0;
            tone(BUZZER, 523, 100);
            points[leg].z = gaits[leg].z;
            step = (step+1)%stepsInCycle;
          }
      }else{
        if (downMove == 0){
        // move body forward
        gaits[leg].x = gaits[leg].x - (Xspeed*cycleTime)/(4*stepsInCycle);
        gaits[leg].y = gaits[leg].y - (Yspeed*cycleTime)/(4*stepsInCycle);
        gaits[leg].z = points[leg].z;
        gaits[leg].r = gaits[leg].r - (Rspeed*cycleTime)/(4*stepsInCycle);
        }
      }
    }else{//stopped
      gaits[leg].z = points[leg].z;
    }
    return gaits[leg];
