#ifndef WING
#define WING
#include "math.h"
class WingClass
{
private:
    double liftSpeedMax = 50;
    double leftWingSpeed = 0;
    double rightWingSpeed = 0;

    //max mins
    double leftWingMax = 2225;
    double leftWingMin = 1534;
    double rightWingMax = 3520;
    double rightWingMin = 2577;

    //locs
    bool leftLocked = false;
    bool rightLocked = false;

public:
    Math myMath;

    //stops motor
    void stopAll()
    {
        leftWingSpeed = 0;
        rightWingSpeed = 0;
        LeftWing.move_velocity(0);
        RightWing.move_velocity(0);
    }

    void stopLeft()
    {
        leftWingSpeed = 0;
    }

    void stopRight()
    {
        rightWingSpeed = 0;
    }

    void update()
    {
        //spin motors
        if(leftWingSpeed < 0 && leftWingPot.get_value() < leftWingMax){
            LeftWing.move_velocity(myMath.toRPM(true, leftWingSpeed, LeftWing.get_gearing()));
        } else if(leftWingSpeed > 0 && leftWingPot.get_value() > leftWingMin){
            LeftWing.move_velocity(myMath.toRPM(true, leftWingSpeed, LeftWing.get_gearing()));
        } else {
            LeftWing.move_velocity(0);
        }
        if(rightWingSpeed < 0 && rightWingPot.get_value() < rightWingMax){
            RightWing.move_velocity(myMath.toRPM(true, rightWingSpeed, RightWing.get_gearing()));
        } else if(rightWingSpeed > 0 && rightWingPot.get_value() > rightWingMin){
            RightWing.move_velocity(myMath.toRPM(true, rightWingSpeed, RightWing.get_gearing()));
        } else {
            RightWing.move_velocity(0);
        }
        
    }

    void lockLeft()
    {
        leftLocked = true;
        leftLock.set_value(true);
    }

    void lockRight()
    {
        rightLocked = true;
        rightLock.set_value(true);
    }

    bool leftLockState()
    {
        return leftLocked;
    }

    bool rightLockState()
    {
        return rightLocked;
    }

    void tiltLeftWing(bool rev)
    {
        leftWingSpeed = (rev? -1 : 1) * liftSpeedMax;
    }
    void tiltRightWing(bool rev)
    {
        rightWingSpeed = (rev? -1 : 1) * liftSpeedMax;
    }

    //Same functions but i wanted to rename them for funzies and readability
    void spinLeft(bool rev)
    {
        leftWingSpeed = (rev? -1 : 1) * liftSpeedMax;
    }
    void spinRight(bool rev)
    {
        rightWingSpeed = (rev? -1 : 1) * liftSpeedMax;
    }
};
#endif // ifndef WING
