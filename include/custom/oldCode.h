//Old code from when moving from 20 -> 21

/*


    //set FLspeed variable
    void setFLspeed(double val)
    {
        FLspeed = val;
    }

    //set FRspeed variable
    void setFRspeed(double val)
    {
        FRspeed = val;
    }

    //set BLspeed variable
    void setBLspeed(double val)
    {
        BLspeed = val;
    }

    //set BRspeed variable
    void setBRspeed(double val)
    {
        BRspeed = val;
    }

    //get FLspeed variable
    double getFLspeed()
    {
        return FLspeed;
    }

    //get FRspeed variable
    double getFRspeed()
    {
        return FRspeed;
    }

    //get BLspeed variable
    double getBLspeed()
    {
        return BLspeed;
    }

    //get BRspeed variable
    double getBRspeed()
    {
        return BRspeed;
    }

    double getRotationTune(){
        return rotationTune;
    }

    double getDeadZone(){
        return deadZone;
    }


// upadate controller vars for bongo orientation
    void catieControl()
    {
        double LXaxis = (master.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
        double LYaxis = (master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        double RXaxis = (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)) / Movement.getRotationTune();
        double RYaxis = (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y));

        Movement.setFLspeed(RYaxis + RXaxis);
        Movement.setFRspeed(RYaxis - RXaxis);
        Movement.setBLspeed(RYaxis + RXaxis);
        Movement.setBRspeed(RYaxis - RXaxis);

        // set speeds in order to move bongo in target agle taken into account
        // current angle
        if (((LXaxis > Movement.getDeadZone()) || (LXaxis < -Movement.getDeadZone())) ||
            ((LYaxis > Movement.getDeadZone()) || (LYaxis < -Movement.getDeadZone())))
        {
            // updates speed
            double speed = myMath.TwoPointsDistance(0, 0, LXaxis, LYaxis);

            // find the angle between straight forward
            double Dangle = acos(LYaxis / speed) * 180 / M_PI;

            // negative if the x axis is on left or neg
            if (LXaxis < 0)
            {
                Dangle *= -1;
            }

            // current angle
            double currentAngle = rotation;
            Movement.addToFLspeed(myMath.sRound(
                myMath.multiplier(FLnum, currentAngle, Dangle) * speed, 3));
            Movement.addToFRspeed(myMath.sRound(
                myMath.multiplier(FRnum, currentAngle, Dangle) * speed, 3));
            Movement.addToBLspeed(myMath.sRound(
                myMath.multiplier(BLnum, currentAngle, Dangle) * speed, 3));
            Movement.addToBRspeed(myMath.sRound(
                myMath.multiplier(BRnum, currentAngle, Dangle) * speed, 3));
        }

        //under is the number in the denominator of the largest number when it equals 100
        //scales down other numbers relative to it
        double under = myMath.greatest(fabs(Movement.getFLspeed()), fabs(Movement.getFRspeed()), fabs(Movement.getBLspeed()),
                                       fabs(Movement.getBRspeed())) /
                       100;
        Movement.setFLspeed(Movement.getFLspeed() / under);
        Movement.setFRspeed(Movement.getFRspeed() / under);
        Movement.setBLspeed(Movement.getBLspeed() / under);
        Movement.setBRspeed(Movement.getBRspeed() / under);
    }

    // upadate controller vars for bongo orientation
    //AKA tyler drive
    void tylerControl()
    {
        double LXaxis = (master.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
        double LYaxis = (master.get_analog(E_CONTROLLER_ANALOG_LEFT_Y));
        double RXaxis = (master.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)) / Movement.getRotationTune();
        Movement.setFLspeed(LYaxis + LXaxis + RXaxis);
        Movement.setFRspeed(LYaxis - LXaxis - RXaxis);
        Movement.setBLspeed(LYaxis - LXaxis + RXaxis);
        Movement.setBRspeed(LYaxis + LXaxis - RXaxis);

        //under is the number in the denominator of the largest number when it equals 100
        //scales down other numbers relative to it
        double under = myMath.greatest(fabs(Movement.getFLspeed()), fabs(Movement.getFRspeed()), fabs(Movement.getBLspeed()),
                                       fabs(Movement.getBRspeed())) /
                       100;

        Movement.setFLspeed(Movement.getFLspeed() / under);
        Movement.setFRspeed(Movement.getFRspeed() / under);
        Movement.setBLspeed(Movement.getBLspeed() / under);
        Movement.setBRspeed(Movement.getBRspeed() / under);
    }


    //handle outake checking
    static void handleOutake(void *)
    {
        delay(1500);
        int offset = 50;
        int redTarget = 360;
        int blueTarget = 200;
        // while false lol
        while (true)
        {
            if (ballToggle)
            {
                // red
                if (teamIsBlue)
                {
                    if (Police.get_hue() <= offset || Police.get_hue() >= redTarget - offset && Police.get_proximity() > 100)
                    {
                        printf("triggered");
                        waitUntilBallPasses();
                    }
                }
                else
                {
                    if (Police.get_hue() >= blueTarget - offset && Police.get_hue() <= blueTarget + offset && Police.get_proximity() > 100)
                    {
                        printf("triggered");
                        waitUntilBallPasses();
                    }
                }
            }
            else
            {
                //printf("Disabled");
            }
            c::task_delay(15);
        }
    }





// checks to see if a ball has passed TODO make async
    static bool passBall()
    {
        // define precautions if ball doesnt output
        int maxTime = 5000; //time in ms before shutoff
        int maxIterations = maxTime / delayVisionTime;
        int iterations = 0;
        const int darkThreshold = 100;
        bool currentBall = false;

        while (true)
        {
            // gets current value of reflectivity of line tracker
            // high val == light enviroment
            double val = outtakeSense.get_proximity();
            //printf("value: %d \n", val);
            if (iterations >= maxIterations)
            {
                printf("end \n");
                return false;
            }
            else
            {
                iterations++;
            }

            if (val >= darkThreshold)
            {
                currentBall = true;
            }
            else if (val <= darkThreshold && currentBall)
            {
                // if the ball has been logged and the value reads light again
                currentBall = false;
                printf("found \n");
                return false;
            }

            delay(delayVisionTime);
        }
    }

    //handler for the outake process
    static void waitUntilBallPasses()
    {
        printf("werhg");
        Movement.flywheel.flywheelset(true);
        //uptake has to go down to output now
        Movement.uptake.setSpeed(30);

        // max flywheel speed
        Movement.flywheel.setSpeed(100);
        //Flywheel.move_velocity(myMath.toRPM(true, 100,
        //                                    Flywheel.get_gearing()));
        Movement.customFlush();

        // wait until the ball passes the back
        passBall();

        // sets flywheel to output top
        Movement.flywheel.setSpeed(Movement.flywheel.speedMedium);

        // corrects uptake
        Movement.uptake.setSpeed(100);

        Movement.customFlushRev();
    }








    void toggleEject()
    {
        ballToggle = !ballToggle;
        led.set_value(ballToggle ? 0 : 1);
    }


bool untilColorFound(bool isBlue)
    {
        int offset = 50;
        int redTarget = 360;
        int blueTarget = 200;
        double colorVal = Police.get_hue();
        if (isBlue)
        {
            if (colorVal <= offset || colorVal >= redTarget - offset && Police.get_proximity() > 100)
            {
                return false;
            }
        }
        else
        {
            if (colorVal >= blueTarget - offset && colorVal <= blueTarget + offset && Police.get_proximity() > 100)
            {
                return false;
            }
        }
        return true;
    }
    int count = 0;
    int limit = 2;
    bool untilFlyhweelPower()
    {
        double threshold = 8;
        double val = Flywheel.get_power();
        if (val >= threshold && count >= limit)
        {
            count = 0;
            return false;
        }
        else if (val >= threshold)
        {
            count++;
        }
        return true;
    }

    bool untilUptakePower()
    {
        double val = Lift.get_power();
        if (val >= threshold && count >= limit)
        {
            count = 0;
            return false;
        }
        else if (val >= threshold)
        {
            count++;
        }
        return true;
    }
    //update bongo current pos






    void setRotation(double val)
    {
        rotation = val;
    }

    //returns bongo's orientation
    double getRotation()
    {
        return rotation;
    }


// sets intake and uptake to flush if true
    void flush(bool setting)
    {
        intake.open(setting);
        uptake.flush(setting);
    }

    //experimental kinda flush uptake and output w/ flywheel
    void customFlush()
    {
        printf("Insidf");
        uptake.flush(true);
        flywheel.outputBall(true);
    }

    //sets uptake flush to false and flywheel output to false
    void customFlushRev()
    {
        printf("Insidg");
        uptake.flush(false);
        flywheel.outputBall(false);
    }


//async PID for lining up with ball
  double lineUpBall(bool set = false)
  {
    printf("Finding");
    vision_object_s_t BLUEBALL = Big_Brother.get_by_sig(0, Big_Brother_CUSTOMBLUE_SIG_NUM);
    vision_object_s_t REDBALL = Big_Brother.get_by_sig(0, Big_Brother_CUSTOMRED_SIG_NUM);
    if (REDBALL.width >= ball_widthLimit)
    {
      printf("Found Red Ball");
      ball_width = REDBALL.width;
      // neg right pos left
      ball_CX = REDBALL.x_middle_coord;
      //ball_targetCX = -1.3286 * ball_width + 140.619 + ball_offset;
      ball_targetCX = 158 - ball_CX + ball_offset;
      ball_error = ball_CX - ball_targetCX;
      printf("ball_error: %f \n", ball_error);
      FLspeed += (ball_Pval * ball_error + ball_Dval * ((ball_error - ball_error_past) / 5));
      FRspeed -= (ball_Pval * ball_error + ball_Dval * ((ball_error - ball_error_past) / 5)); this is not how you pid you fucking idiot
      BLspeed += (ball_Pval * ball_error + ball_Dval * ((ball_error - ball_error_past) / 5));
      BRspeed -= (ball_Pval * ball_error + ball_Dval * ((ball_error - ball_error_past) / 5)time delay);
      if (set)
      {
        moveFL(ball_Pval * ball_error);
        moveFR(ball_Pval * -ball_error);
        moveBL(ball_Pval * ball_error);
        moveBR(ball_Pval * -ball_error);
      }
      ball_error_past = ball_error;
    }
    else if (BLUEBALL.width >= ball_widthLimit)
    {
      printf("Found Blue Ball");
      ball_width = BLUEBALL.width;
      // neg right pos left
      ball_CX = BLUEBALL.x_middle_coord;
      //ball_targetCX = -1.3286 * ball_width + 140.619 + ball_offset;
      ball_targetCX = 158 - ball_CX + ball_offset;
      ball_error = ball_CX - ball_targetCX;
      printf("ball_error: %f \n", ball_error);
      FLspeed += ball_Pval * ball_error;
      FRspeed -= ball_Pval * ball_error;
      BLspeed += ball_Pval * ball_error;
      BRspeed -= ball_Pval * ball_error;
      if (set)
      {
        moveFL(ball_Pval * ball_error);
        moveFR(ball_Pval * -ball_error);
        moveBL(ball_Pval * ball_error);
        moveBR(ball_Pval * -ball_error);
      }
    }
    return ball_error;
  }


  directional arrays for cardinal directions
  FL FR BL BR
  true if negative

  //motor map to move forward
  bool upDirections[4] = {false, false, false, false};
  //motor map to move right
  bool rightDirections[4] = {false, true, true, false};
  //motor map to move down
  bool downDirections[4] = {true, true, true, true};
  //motor map to move left
  bool leftDirections[4] = {true, false, false, true};

  // clears rotations for all motor encoders
  void clearRotations()
  {
    c::motor_set_zero_position(FLPort, 0);
    c::motor_set_zero_position(FRPort, 0);
    c::motor_set_zero_position(BLPort, 0);
    c::motor_set_zero_position(BRPort, 0);
  }


  // speed using arrows
  double cardinalspeed = 30;
double headingScale = .5;
// moves in cardinal directions
  void cardinalMove(bool forward[])
  {
    double FLcurrent = c::motor_get_position(FLPort);
    double FRcurrent = c::motor_get_position(FRPort);
    double BLcurrent = c::motor_get_position(BLPort);
    double BRcurrent = c::motor_get_position(BRPort);
    double average = myMath.getAverage(fabs(FLcurrent), fabs(FRcurrent),
                                       fabs(BLcurrent), fabs(BRcurrent));
    double FLscale = myMath.scale(average, fabs(FLcurrent), headingScale);
    double FRscale = myMath.scale(average, fabs(FRcurrent), headingScale);
    double BLscale = myMath.scale(average, fabs(BLcurrent), headingScale);
    double BRscale = myMath.scale(average, fabs(BRcurrent), headingScale);

    FLspeed =
        forward[0] ? -(cardinalspeed + FLscale) : (cardinalspeed + FLscale);
    FRspeed =
        forward[1] ? -(cardinalspeed + FRscale) : (cardinalspeed + FRscale);
    BLspeed =
        forward[2] ? -(cardinalspeed + BLscale) : (cardinalspeed + BLscale);
    BRspeed =
        forward[3] ? -(cardinalspeed + BRscale) : (cardinalspeed + BRscale);
  }




*/