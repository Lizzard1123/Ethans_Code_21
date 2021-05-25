#ifndef WING
#define WING
#include "math.h"
class WingClass
{
private:
    double liftspeedMax = 50;
    double leftWingSpeed = 0;
    double rightWingSpeed = 0;

    bool leftLocked = false;
    bool rightLocked = false;

public:
    double speedMedium = 63;
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
        LeftWing.move_velocity(0);
    }

    void stopRight()
    {
        RightWing.move_velocity(0);
    }

    void update()
    {
        //spin motors
        LeftWing.move_velocity(myMath.toRPM(true, (leftWingSpeed / 127) * liftspeedMax, LeftWing.get_gearing()));
        RightWing.move_velocity(myMath.toRPM(true, (rightWingSpeed / 127) * liftspeedMax, RightWing.get_gearing()));
        //update left lock
        if (leftLocked)
        {
            leftLock.set_value(HIGH);
        }
        else
        {
            leftLock.set_value(LOW);
        }
        //update right lock
        if (rightLocked)
        {
            rightLock.set_value(HIGH);
        }
        else
        {
            rightLock.set_value(LOW);
        }
    }

    void toggleLeftLock()
    {
        leftLocked = !leftLocked;
    }

    void toggleRightLock()
    {
        rightLocked = !rightLocked;
    }

    bool leftLockState()
    {
        return leftLocked;
    }

    bool rightLockState()
    {
        return rightLocked;
    }

    void setLeftLockState(bool val)
    {
        leftLocked = val;
    }

    void setRightLockState(bool val)
    {
        rightLocked = val;
    }

    void tiltLeftWing(double val)
    {
        leftWingSpeed = val;
    }
    void tiltRightWing(double val)
    {
        rightWingSpeed = val;
    }
};
#endif // ifndef WING
