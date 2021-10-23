#ifndef GLOBAL
#define GLOBAL
#include "main.h"
//motor ports
#define FLPort 20 
#define FRPort 11
#define BLPort 19
#define BRPort 12 
#define LeftWingPort 8
#define RightWingPort 13 
#define YarmPort 9
#define XarmPort 10
#define leftEyePort 7 
#define rightEyePort 1 
#define backEyePort 99 
//other defines
#define LeftSideNum 0
#define RightSideNum 1
#define BackSideNum 2
#define expanderPort 15 
#define leftLockPort {{expanderPort, 'c'}} // e is left wing lock
#define rightLockPort {{expanderPort, 'h'}} 
#define ringLockPort {{expanderPort, 'g'}}
#define leftWingLock {{expanderPort, 'e'}}
#define rightWingLock {{expanderPort, 'f'}}


//motors
extern Motor FL;
extern Motor FR;
extern Motor BL;
extern Motor BR;
extern Motor LeftWing;
extern Motor RightWing;
extern Motor Yarm;
extern Motor Xarm;

//controllers
extern Controller master;
extern Controller partner;

//vision sensors
//extern Vision EYES;
extern Vision leftEye;
extern Vision rightEye;
extern Vision backEye;

//sigs
//#define EYES__CUSTOM_GREEN_NUM 1
#define MOGO_CUSTOM_YELLOW_NUM 1
//extern vision_signature_s_t  EYES__CUSTOM_GREEN;
extern vision_signature_s_t MOGO_CUSTOM_YELLOW;

//opical sensor
//extern Optical Police;

//pots
extern ADIAnalogIn liftPot;
extern ADIAnalogIn leftWingPot;
extern ADIAnalogIn rightWingPot;

//LED
//extern ADIDigitalOut led;
extern ADIDigitalOut leftLock;
extern ADIDigitalOut rightLock;
extern ADIDigitalOut ringLock;
extern ADIDigitalOut leftLockWing;
extern ADIDigitalOut rightLockWing;


//limit switch / button
//extern ADIDigitalIn tailSensor;

//Odom
extern okapi::ADIEncoder  leftOdom;
extern okapi::ADIEncoder  rightOdom;
extern okapi::ADIEncoder  middleOdom;

//pots
//extern ADIAnalogIn TeamColor;

//labels debug
extern lv_obj_t *debugXLabel;
extern lv_obj_t *debugYLabel;
extern lv_obj_t *debugRotationLabel;
extern lv_obj_t *debugXarmLabel;
extern lv_obj_t *debugYarmLabel;

//global teamcolor
extern bool teamIsBlue;

#endif

