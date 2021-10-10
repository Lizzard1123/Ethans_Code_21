#include "custom/robot.h"

// motors
//TODO change the motor cartriages of the motors (36, 18, 6)
//TODO if motor is reversed then this boolean param flips
Motor FL(FLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor FR(FRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor BL(BLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor BR(BRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor Yarm(YarmPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES); 
Motor Xarm(XarmPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_COUNTS); 
Motor LeftWing(LeftWingPort, E_MOTOR_GEARSET_36, false, E_MOTOR_ENCODER_DEGREES); 
Motor RightWing(RightWingPort, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES); 


// controllers
Controller master(E_CONTROLLER_MASTER);
Controller partner(E_CONTROLLER_PARTNER);

// vision sensors
//Vision EYES(EYESPort);
Vision leftEye(leftEyePort);
Vision rightEye(rightEyePort);
Vision backEye(backEyePort);

//signatures for vision sensor 
//vision_signature_s_t EYES__CUSTOM_GREEN = Vision::signature_from_utility(EYES__CUSTOM_GREEN_NUM, -4949, -4509, -4729, -5669, -5035, -5352, 9.400, 0);
vision_signature_s_t MOGO_CUSTOM_YELLOW = Vision::signature_from_utility(MOGO_CUSTOM_YELLOW_NUM, 189, 1879, 1034, -3857, -2845, -3352, 0.700, 0); //TODO get the value of mogo sig
/*
vision::signature YLW (1, -623, 257, -182, -4361, -3217, -3790, 4.500, 0);
vision::signature SIG_1 (1, 189, 1879, 1034, -3857, -2845, -3352, 0.700, 0);
*/
//Optical sensor
//Optical Police(PolicePort);

//LED/digital out for pnematics
//ADIDigitalOut led('F', 1);
//ADIDigitalOut  leftLock ({{expanderPort, leftLockPort}});
//ADIDigitalOut  rightLock ({{expanderPort, rightLockPort}});
//TODO put lock ports back into expanders
ADIDigitalOut leftLock (leftLockPort);
ADIDigitalOut rightLock (rightLockPort);
ADIDigitalOut ringLock (ringLockPort);


//button / limit switch
//ADIDigitalIn tailSensor(tailSensorPort);

//pots
ADIAnalogIn liftPot('b');
ADIAnalogIn leftWingPot('a');
ADIAnalogIn rightWingPot('h');

// Odom 
//TODO put odom back 
//okapi::ADIEncoder  leftOdom('a', 'b', true);
//okapi::ADIEncoder  rightOdom('g', 'h', false);
//okapi::ADIEncoder  middleOdom('e', 'f', true);

// Staic defines
RobotMovement Robot::Movement;
WingClass Robot::Wings;
LiftClass Robot::Lift;
Math Robot::myMath;
bool Robot::teamIsBlue = false;
const double Robot::posDelay = 50;
double Robot::X = 0;
double Robot::Y = 0;
double Robot::rotation = 0;
