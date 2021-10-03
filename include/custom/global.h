#ifndef GLOBAL
#define GLOBAL
#include "main.h"
//motor ports
#define FLPort 20 
#define FRPort 11
#define BLPort 19
#define BRPort 12 
#define LeftWingPort 8
#define RightWingPort 99 //TODO assign port
#define YarmPort 9
#define XarmPort 10
#define leftEyePort 99 //TODO assign port
#define rightEyePort 99 //TODO assign port
#define backEyePort 99 //TODO assign port
//other defines
#define LeftSideNum 0
#define RightSideNum 1
#define BackSideNum 2
#define expanderPort 99 //TODO assign port
#define leftLockPort 'f'
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
extern ADIDigitalOut leftLock;
extern ADIDigitalOut rightLock;
extern ADIDigitalOut backLock;

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

