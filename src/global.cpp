#include "custom/robot.h"

// motors
//TODO change the motor cartriages of the motors (36, 18, 6)
//TODO if motor is reversed then this boolean param flips
Motor FL(FLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor FR(FRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor BL(BLPort, E_MOTOR_GEARSET_18, false, E_MOTOR_ENCODER_DEGREES);
Motor BR(BRPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES);
Motor Yarm(YarmPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES); 
Motor Xarm(XarmPort, E_MOTOR_GEARSET_18, true, E_MOTOR_ENCODER_DEGREES); 
Motor LeftWing(LeftWingPort, E_MOTOR_GEARSET_36, true, E_MOTOR_ENCODER_DEGREES); 
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
vision_signature_s_t MOGO_CUSTOM_YELLOW = Vision::signature_from_utility(MOGO_CUSTOM_YELLOW_NUM, -4949, -4509, -4729, -5669, -5035, -5352, 9.400, 0); //TODO get the value of mogo sig

//Optical sensor
//Optical Police(PolicePort);

//LED/digital out for pnematics
//ADIDigitalOut led('F', 1);
ADIAnalogOut leftLock ({{expanderPort, leftLockPort}});
ADIAnalogOut rightLock ({{expanderPort, rightLockPort}});

//button / limit switch
//ADIDigitalIn tailSensor(tailSensorPort);

//pots
ADIAnalogIn liftPot(8);

// Odom
okapi::ADIEncoder  leftOdom('a', 'b', true);
okapi::ADIEncoder  rightOdom('g', 'h', false);
okapi::ADIEncoder  middleOdom('e', 'f', true);

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
