#include <ax12.h>
#include <BioloidController.h>
#include <Arduino.h>
#include <math.h>
extern BioloidController bioloid;

#define AX12_HEXAPOD

void Setup(){
Serial.begin(38400);
  pinMode(0,OUTPUT);

  delay (1000);

  float voltage = (ax12GetRegister (1, AX_PRESENT_VOLTAGE, 1)) / 10.0;

  Serial.print ("System Voltage: ");

  Serial.print (voltage);

  Serial.println (" volts.");

  if (voltage < 10.0)


  // stand up slowly
  bioloid.poseSize = 18;
  bioloid.readPose();
  doIK();
  bioloid.interpolateSetup(1000);
  while(bioloid.interpolating > 0){
    bioloid.interpolateStep();
    delay(3);

  }

}

#define L_COXA      52  // MM distance from coxa servo to femur servo
#define L_FEMUR     82 // MM distance from femur servo to tibia servo
#define L_TIBIA     140 // MM distance from tibia servo to foot

#define RF_COXA 2
#define RF_TIBIA 6
#define RF_FEMUR 4

int mins[] = {222, 225, 159, 164, 279, 158, 223, 229, 159, 156, 272, 155, 226, 233, 158, 157, 271, 157};
int maxs[] = {790, 792, 855, 862, 857, 747, 788, 794, 859, 857, 860, 747, 789, 789, 858, 860, 859, 743};

typedef struct{
    int coxa;
    int femur;
    int tibia;
} ik_sol_t;

int radToServo(float rads){
  float val = (rads*100)/51 * 100;
  return (int) val;
}

ik_sol_t legIK(int X, int Y, int Z);

/* Simple 3dof leg solver. X,Y,Z are the length from the Coxa rotate to the endpoint. */

ik_sol_t legIK(int X, int Y, int Z){
    ik_sol_t ans;

    // first, make this a 2DOF problem... by solving coxa
    ans.coxa = radToServo(atan2(X,Y));
    long trueX = sqrt(sq((long)X)+sq((long)Y)) - L_COXA;  
    long im = sqrt(sq((long)trueX)+sq((long)Z));    // length of imaginary leg

    // get femur angle above horizon...
    float q1 = -atan2(Z,trueX);                                  
    long d1 = sq(L_FEMUR)-sq(L_TIBIA)+sq(im);             //   Femur ==> /
    long d2 = 2*L_FEMUR*im;                               //            /
    float q2 = acos((float)d1/(float)d2);                 //Coxa v     /  <== Angle
    ans.femur = radToServo(q1+q2);                        //----------/_______________ <== Horizon

    // and tibia angle from femur...                        // Femur    ==>   /\
    d1 = sq(L_FEMUR)-sq(im)+sq(L_TIBIA);                    //               /  \
    d2 = 2*L_TIBIA*L_FEMUR;                                 //              / ^  \
    ans.tibia = radToServo(acos((float)d1/(float)d2)-1.57); //             /  ^   \
                                                            //            / Angle? \ <== Tibia
    return ans;                                             //

}

void doIK(){
  int servo;
  ik_sol_t sol;
  sol = legIK(52, 118, 97);
  servo = 368 + sol.coxa;
    if(servo < maxs[RF_COXA-1] && servo > mins[RF_COXA-1])
        bioloid.setNextPose(RF_COXA, servo);
    else{
        Serial.print("RF_COXA FAIL: ");
        Serial.println(servo);
    }
    servo = 524 + sol.femur;
    if(servo < maxs[RF_FEMUR-1] && servo > mins[RF_FEMUR-1])
        bioloid.setNextPose(RF_FEMUR, servo);
    else{
        Serial.print("RF_FEMUR FAIL: ");
        Serial.println(servo);
    }
    servo = 354 + sol.tibia;
    if(servo < maxs[RF_TIBIA-1] && servo > mins[RF_TIBIA-1])
        bioloid.setNextPose(RF_TIBIA, servo);
    else{
        Serial.print("RF_TIBIA FAIL: ");
        Serial.println(servo);
    }
}

