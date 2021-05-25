#ifndef INTAKE
#define INTAKE
#include "math.h"
class IntakeClass
{
private:
  double liftspeed = 40;
  bool toggled = false;
  
public:  
  Math myMath;
  double liftSpeedLow = 40;
  double liftSpeedHigh = 100;

  //stops motor
  void stop()
  {
    Lift.move_velocity(0);
  }

  void update()
  {
    if (toggled)
    {
      Lift.move_velocity(myMath.toRPM(true, liftspeed, Lift.get_gearing()));
    }
    else
    {
      stop();
    }
  }

  //get toggled state of lift
  bool getToggle()
  {
    return toggled;
  }

  //set toggled state of lift
  void setToggle(bool setting)
  {
    toggled = setting;
  }

  //toggle lift
  void toggle()
  {
    toggled = !toggled;
  }

  //returns speed of lift
  double getSpeed()
  {
    return liftspeed;
  }

  // sets flush
  void setSpeed(double setting)
  {
    liftspeed = setting;
  }
};
#endif // ifndef INTAKE
