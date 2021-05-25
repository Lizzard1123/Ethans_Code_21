#ifndef GLOBAL
#define GLOBAL
#include "main.h"
//motor ports
#define FLPort 5 
#define FRPort 2
#define BLPort 1 
#define BRPort 9 
#define leftEyePort 99 //TODO assign port
#define rightEyePort 99 //TODO assign port
#define backEyePort 99 //TODO assign port
//other defines
#define leftSideNum 0
#define RightSideNum 1
#define BackSideNum 2

//motors
extern Motor FL;
extern Motor FR;
extern Motor BL;
extern Motor BR;

//controllers
extern Controller master;

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
//extern ADIAnalogIn rightArm;

//LED
//extern ADIDigitalOut led;

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

