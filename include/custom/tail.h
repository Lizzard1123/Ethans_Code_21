#ifndef TAIL
#define TAIL
#include "math.h"
class TailClass
{
private:
    double maxLiftSpeed = 50;
    double liftspeed = 0;
    //toggle lock, controlls the pneumatic lock and automatic lift
    bool locked = false;
    //switches between manual control of lift and auton
    bool manual = true;

public:
    Math myMath;
    double speedMedium = 63;
    //stops motor
    void stopAll()
    {
        liftspeed = 0;
        BackWing.move_velocity(0);
    }

    void update()
    {
        //spin motors
        if (manual) //manual control of lift
        {
            BackWing.move_velocity(myMath.toRPM(true, (liftspeed / 127) * maxLiftSpeed, BackWing.get_gearing()));
        }
        else
        { //auton control of lift rises until the button/limit switch is pressed then stops and holds
            if (locked)
            {
                if(tailSensor.get_value() == LOW){
                    BackWing.move_velocity(myMath.toRPM(true, maxLiftSpeed, BackWing.get_gearing()));
                }
            }
            else
            {
                stopAll();
            }
        }

        //update right lock
        if (locked)
        {
            backLock.set_value(HIGH);
        }
        else
        {
            backLock.set_value(LOW);
        }
    }

    void toggleLock()
    {
        locked = !locked;
    }

    bool lockState()
    {
        return locked;
    }

    void setManual(){
        manual = true;
    }
     
    void setAutomatic(){
        manual = false;
    }

    void setLockState(bool val)
    {
        locked = val;
    }

    void tiltTail(double val)
    {
        liftspeed = val;
    }
};
#endif // ifndef WING
