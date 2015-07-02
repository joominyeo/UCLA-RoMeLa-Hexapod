#include <ax12.h>
#include <BioloidController.h>
#include <Arduino.h>
#include <math.h>
#define LEG_COUNT   6
#define X_COXA      60  // MM between front and back legs /2
#define Y_COXA      60  // MM between front/back legs /2
#define M_COXA      100  // MM between two middle legs /2

void Setup(){
Serial.begin(9600);
}

/* Servo ouptut values (output of 3dof leg solver). */
typedef struct{
    int coxa;
    int femur;
    int tibia;
} ik_sol_t;

/* A leg position request (output of body calcs, input to simple 3dof solver). */
typedef struct{
    int x;
    int y;
    int z;
    float r;
} ik_req_t;

/* Actual positions, and indices of array. */
extern ik_req_t endpoints[LEG_COUNT];
#define RIGHT_FRONT    0
#define RIGHT_REAR     1
#define LEFT_FRONT     2
#define LEFT_REAR      3
#define RIGHT_MIDDLE   4
#define LEFT_MIDDLE    5

/* Parameters for manipulating body position */
extern float bodyRotX;    // body roll
extern float bodyRotY;    // body pitch
extern float bodyRotZ;    // body rotation
extern int bodyPosX;
extern int bodyPosY;

/* IK Engine */
ik_req_t endpoints[LEG_COUNT];
float bodyRotX = 0;             // body roll (rad)
float bodyRotY = 0;             // body pitch (rad)
float bodyRotZ = 0;             // body rotation (rad)
int bodyPosX = 0;               // body offset (mm)
int bodyPosY = 0;               // body offset (mm)
int Xspeed;                     // forward speed (mm/s)
int Yspeed;                     // sideward speed (mm/s)
float Rspeed;                   // rotation speed (rad/s)

extern BioloidController bioloid;
void setupIK(){
  endpoints[RIGHT_FRONT].x = 52;
  endpoints[RIGHT_FRONT].y = 118;
  endpoints[RIGHT_FRONT].z = 97;

  //liftHeight = 50;
  //stepsInCycle = 1;
  //step = 0;
}
/* Body IK solver: compute where legs should be. */
ik_req_t bodyIK(int X, int Y, int Z, int Xdisp, int Ydisp, float Zrot){
    ik_req_t ans;

    float cosB = cos(bodyRotX);
    float sinB = sin(bodyRotX);
    float cosG = cos(bodyRotY);
    float sinG = sin(bodyRotY);
    float cosA = cos(bodyRotZ+Zrot);
    float sinA = sin(bodyRotZ+Zrot);

    int totalX = X + Xdisp + bodyPosX;
    int totalY = Y + Ydisp + bodyPosY;

    ans.x = totalX - int(totalX*cosG*cosA + totalY*sinB*sinG*cosA + Z*cosB*sinG*cosA - totalY*cosB*sinA + Z*sinB*sinA) + bodyPosX;
    ans.y = totalY - int(totalX*cosG*sinA + totalY*sinB*sinG*sinA + Z*cosB*sinG*sinA + totalY*cosB*cosA - Z*sinB*cosA) + bodyPosY;
    ans.z = Z - int(-totalX*sinG + totalY*sinB*cosG + Z*cosB*cosG);

    return ans;
}

int mins[] = {222, 225, 159, 164, 279, 158, 223, 229, 159, 156, 272, 155, 226, 233, 158, 157, 271, 157};
int maxs[] = {790, 792, 855, 862, 857, 747, 788, 794, 859, 857, 860, 747, 789, 789, 858, 860, 859, 743};

/* Legs */
#define L_COXA      52  // MM distance from coxa servo to femur servo
#define L_FEMUR     82 // MM distance from femur servo to tibia servo
#define L_TIBIA     140 // MM distance from tibia servo to foot

/*Servo IDs */
#define RF_COXA 2
#define RF_TIBIA 6
#define RF_FEMUR 4

/* find the translation of the coxa point (x,y) in 3-space, given our rotations */
ik_req_t bodyIK(int X, int Y, int Z, int Xdisp, int Ydisp, float Zrot);
/* given our leg offset (x,y,z) from the coxa point, calculate servo values */
ik_sol_t legIK(int X, int Y, int Z);

/* Convert radians to servo position offset. */
int radToServo(float rads){
  float val = (rads*100)/51 * 100;
  return (int) val;
}

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
    ik_req_t req, gait;
    ik_sol_t sol;

    // right front leg
    req = bodyIK(endpoints[RIGHT_FRONT].x+gait.x, endpoints[RIGHT_FRONT].y+gait.y, endpoints[RIGHT_FRONT].z+gait.z, X_COXA, Y_COXA, gait.r);
    sol = legIK(endpoints[RIGHT_FRONT].x+req.x+gait.x,endpoints[RIGHT_FRONT].y+req.y+gait.y,endpoints[RIGHT_FRONT].z+req.z+gait.z);
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
