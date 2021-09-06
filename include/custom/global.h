#ifndef GLOBAL
#define GLOBAL
#include "main.h"
//motor ports
#define FLPort 18 
#define FRPort 12
#define BLPort 10
#define BRPort 2 
#define LeftWingPort 99 //TODO assign port
#define RightWingPort 99 //TODO assign port
#define YarmPort 99 //TODO assign port
#define XarmPort 99 //TODO assign port
#define leftEyePort 99 //TODO assign port
#define rightEyePort 99 //TODO assign port
#define backEyePort 99 //TODO assign port
//other defines
#define LeftSideNum 0
#define RightSideNum 1
#define BackSideNum 2
#define expanderPort 99 //TODO assign port
#define leftLockPort 99 //TODO assign port
#define rightLockPort 99 //TODO assign port

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

//LED
//extern ADIDigitalOut led;
extern ADIAnalogOut leftLock;
extern ADIAnalogOut rightLock;
extern ADIAnalogOut backLock;

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

//global teamcolor
extern bool teamIsBlue;

#endif

