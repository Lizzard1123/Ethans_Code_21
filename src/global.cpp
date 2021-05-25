#include "custom/robot.h"

// motors
Motor FL(FLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor FR(FRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor BL(BLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor BR(BRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);

// controllers
Controller master(E_CONTROLLER_MASTER);

// vision sensors
//Vision EYES(EYESPort);
Vision leftEye(leftEyePort);
Vision rightEye(rightEyePort);
Vision backEye(backEyePort);

//signatures for vision sensor 
//vision_signature_s_t EYES__CUSTOM_GREEN = Vision::signature_from_utility(EYES__CUSTOM_GREEN_NUM, -4949, -4509, -4729, -5669, -5035, -5352, 9.400, 0);
vision_signature_s_t MOGO_CUSTOM_YELLOW = Vision::signature_from_utility(MOGO_CUSTOM_YELLOW_NUM, -4949, -4509, -4729, -5669, -5035, -5352, 9.400, 0); //TODO get the value of mogo sig

//Optical sensor
//Optical Police(PolicePort);

//LED
//ADIDigitalOut led('F', 1);

//pots
//ADIAnalogIn rightArm(8);

// Odom
okapi::ADIEncoder  leftOdom('e', 'f', false);
okapi::ADIEncoder  rightOdom('a', 'b', false);
okapi::ADIEncoder  middleOdom('c', 'd', true);

// Staic defines
RobotMovement Robot::Movement;
Math Robot::myMath;
bool Robot::teamIsBlue = false;
const double Robot::posDelay = 20;
double Robot::X = 0;
double Robot::Y = 0;
double Robot::rotation = 0;
