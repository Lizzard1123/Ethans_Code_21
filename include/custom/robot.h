#ifndef ROBOT
#define ROBOT

#include "movement.h"
#include "auton.h"
#include <climits>

class Robot
{
private:
    // global X pos of bongo declared in global.cpp
    static double X;
    // global Y pos of bongo declared in global.cpp
    static double Y;
    //current heading of bongo
    static double rotation;
    // global position update delay for bongo declared in global.cpp
    static const double posDelay;
    // global team color of bongo declared in global.cpp
    static bool teamIsBlue;
    // auton selector num
    int autonCodeNum;
    // true if bongo is on left for auton
    bool left;
    // bongo has been initialized
    bool initz = false;
    

    //tower PID vars
    // neg right pos left angle
    double offset = -25;
    double error;
    double targetCX;
    double CX;
    double width;
    double error_past = 0;
    double Dval = 2;
    double Pval = .3;
    double widthLimit = 20;

public:
    /*robot subsytems*/
    // class handler for movement + other funtions
    static RobotMovement Movement;
    //Auton funtions
    AutonMaker Auton;
    // custom math reference
    static Math myMath;

    // sets team color to red
    void setRed()
    {
        teamIsBlue = false;
    }

    // sets team color to blue
    void setBlue()
    {
        teamIsBlue = true;
    }

    // sets auton number
    void setAutonNum(int num)
    {
        autonCodeNum = num;
    }

    // sets auton side, true is left
    void sideIsLeft(bool yes)
    {
        left = yes;
    }

    // returns true if team is blue
    bool getColor()
    {
        return teamIsBlue;
    }

    // returns auton num
    int getAutonNum()
    {
        return autonCodeNum;
    }

    //returns x variable
    double getX()
    {
        return X;
    }
    //returns y variable
    double getY()
    {
        return Y;
    }
    //returns the rotation of bongo
    static double getRotation(){
        return rotation;
    }
    // returns if bongo is on left during auton
    bool getSide()
    {
        return left;
    }

    void autonomous(){
        Auton.Autonomous(autonCodeNum, left, teamIsBlue);
    }

    void catieControl(){
        Movement.catieControl(rotation);
    }

    void tylerControl(){
        Movement.tylerControl();
    }

    // PID syncronous movement from current location to target X , Y set speed along the way
    int PIDMove(double targetX, double targetY, int maxspeed = 100)
    {
        // in ms
        int PIDspeed = 100;
        // tolerance in inches
        double tolerance = 1;
        bool reachedGoal;
        double speed = 0;
        double error;
        double derivative;
        double integral = 0;
        double Pval = 13;
        double Ival = 0;
        double Dval = 0;

        while (true)
        {
            // find the error distance bertween current and target point
            error = myMath.TwoPointsDistance(X, Y, targetX, targetY);

            // find the intergral part for I
            if ((speed != 0) && (speed <= 20) && (integral < 1000) && (error < 1000))
            {
                integral += error;
            }

            // find the derivative part for D
            derivative = error / PIDspeed;

            // PID ALGO
            speed = (error * Pval) + (integral * Ival) + (derivative * Dval);

            // get current angle
            double currentAngle = rotation;
            double Dangle = myMath.angleBetween(X, Y, targetX, targetY);

            // fancy algo
            double FLAuton = myMath.sRound(
                myMath.multiplier(FLnum, currentAngle, Dangle) * speed, 3);
            double FRAuton = myMath.sRound(
                myMath.multiplier(FRnum, currentAngle, Dangle) * speed, 3);
            double BLAuton = myMath.sRound(
                myMath.multiplier(BLnum, currentAngle, Dangle) * speed, 3);
            double BRAuton = myMath.sRound(
                myMath.multiplier(BRnum, currentAngle, Dangle) * speed, 3);
            double under = myMath.greatest(fabs(FLAuton), fabs(FRAuton),
                                           fabs(BLAuton), fabs(BRAuton)) /
                           100;
            FLAuton = (FLAuton / under);
            FRAuton = (FRAuton / under);
            BLAuton = (BLAuton / under);
            BRAuton = (BRAuton / under);
            printf("FLAuton %f \n", FLAuton);
            printf("FRAuton %f \n", FRAuton);
            printf("BLAuton %f \n", BLAuton);
            printf("BRAuton %f  \n", BRAuton);
            printf("currentAngle %f  \n", currentAngle);
            printf("Dangle %f \n", Dangle);
            printf("error %f \n", error);
            printf("X:  %f \n", X);
            printf("Y:  %f \n", Y);
            // Drive Bongo
            Movement.moveFL(FLAuton);
            Movement.moveFR(FRAuton);
            Movement.moveBL(BLAuton);
            Movement.moveBR(BRAuton);

            // if the pid loop has reached target
            if (((error <= tolerance) && (error >= -tolerance)))
            {
                reachedGoal = true;
            }
            else
            {
                reachedGoal = false;
            }

            if (reachedGoal)
            {
                Movement.moveLeft(0);
                Movement.moveRight(0);
                break;
            }

            delay(PIDspeed);
        }
        return 1;
    }

    // PID syncronous turning TODO merge with movement
    int PIDTurn(double target)
    {
        double tolerance = .5;
        int PIDspeed = 10;
        double motorSpeed = 0;
        double turnTarget = target;
        bool reachedGoal;
        double error;
        double Derivative = 0;
        double integralone = 0;
        double Pval = 2.5;
        //double Ival = .1;
        // double Ival = 0;
        //double Dval = .7;
        double Ival = 0;
        double Dval = 0;

        // double Dval = 0;
        double headingVal;

        while (true)
        {
            // update left and right odom values
            headingVal = rotation;
            printf("Heading: %f", headingVal);

            // find the error of both sides  for P
            error = turnTarget - headingVal;

            // find the intergral part for I
            // integralone = 0;
            //if ((integralone == -4294967295.4294967295) || (integralone == 1045))
            //{
            //  integralone = 0;
            //}

            //if (motorSpeed <= 20)
            //{
            //  integralone += error;
            //}

            // find the derivative part for D
            //Derivative = error / PIDspeed;

            // PID ALGO
            motorSpeed = (error * Pval) + (integralone * Ival) + (Derivative * Dval);

            // if its above 100%
            if (motorSpeed > 100)
            {
                motorSpeed = 100;
            }
            else if (motorSpeed < -100)
            {
                motorSpeed = -100;
            }

            // Drive Bongo
            // if (negative) {
            Movement.moveLeft(motorSpeed);
            Movement.moveRight(-motorSpeed);

            // if the pid loop has reached target
            if ((error <= tolerance) && (error >= -tolerance))
            {
                reachedGoal = true;
            }
            else
            {
                reachedGoal = false;
            }

            if (reachedGoal)
            {
                Movement.moveLeft(0);
                Movement.moveRight(0);
                break;
            }

            delay(PIDspeed);
        }
        return 1;
    }

    //PID turnMove turns and moves
    int PIDMoveTurn(double targetX, double targetY, double target, int maxspeed = 100)
    {
        // in ms
        int PIDspeed = 20;
        // tolerance in inches
        double tolerance = .5;
        bool reachedGoal;
        double speed = 0;
        double error;
        double derivative;
        double integral = 0;
        double Pval = 13;
        double Ival = 0;
        double Dval = 0;

        double t_tolerance = .5;
        int t_PIDspeed = 20;
        double t_motorSpeed = 0;
        double t_turnTarget = target;
        bool t_reachedGoal;
        double t_error;
        double t_Derivative;
        double t_integralone = 0;
        double t_Pval = 2.5;
        double t_Ival = .1;
        // double Ival = 0;
        double t_Dval = .7;
        // double Dval = 0;
        double t_headingVal;

        while (true)
        {
            // find the error distance bertween current and target point
            error = myMath.TwoPointsDistance(X, Y, targetX, targetY);

            // find the intergral part for I
            if ((speed != 0) && (speed <= 20) && (integral < 1000) && (error < 1000))
            {
                integral += error;
            }

            // find the derivative part for D
            derivative = error / PIDspeed;

            // PID ALGO
            speed = (error * Pval) + (integral * Ival) + (derivative * Dval);

            // get current angle
            double currentAngle = rotation;
            double Dangle = myMath.angleBetween(X, Y, targetX, targetY);

            // fancy algo
            double FLAuton = myMath.sRound(
                myMath.multiplier(FLnum, currentAngle, Dangle) * speed, 3);
            double FRAuton = myMath.sRound(
                myMath.multiplier(FRnum, currentAngle, Dangle) * speed, 3);
            double BLAuton = myMath.sRound(
                myMath.multiplier(BLnum, currentAngle, Dangle) * speed, 3);
            double BRAuton = myMath.sRound(
                myMath.multiplier(BRnum, currentAngle, Dangle) * speed, 3);
            double under = myMath.greatest(fabs(FLAuton), fabs(FRAuton),
                                           fabs(BLAuton), fabs(BRAuton)) /
                           100;
            FLAuton = (FLAuton / under);
            FRAuton = (FRAuton / under);
            BLAuton = (BLAuton / under);
            BRAuton = (BRAuton / under);

            /*
      TURNN
      */

            // update left and right odom values
            t_headingVal = rotation;

            // find the error of both sides  for P
            error = t_turnTarget - t_headingVal;

            // find the intergral part for I
            // integralone = 0;
            if ((t_integralone == -4294967295.4294967295) || (t_integralone == 1045))
            {
                t_integralone = 0;
            }

            if (t_motorSpeed <= 20)
            {
                t_integralone += t_error;
            }

            // find the derivative part for D
            t_Derivative = t_error / t_PIDspeed;

            // PID ALGO
            t_motorSpeed = (t_error * t_Pval) + (t_integralone * t_Ival) + (t_Derivative * t_Dval);

            // if its above 100%
            if (t_motorSpeed > 100)
            {
                t_motorSpeed = 100;
            }
            else if (t_motorSpeed < -100)
            {
                t_motorSpeed = -100;
            }

            // if the pid loop has reached target
            if ((t_error <= t_tolerance) && (t_error >= -t_tolerance))
            {
                t_reachedGoal = true;
            }
            else
            {
                t_reachedGoal = false;
            }

            // Drive Bongo
            Movement.moveFL(FLAuton + t_motorSpeed);
            Movement.moveFR(FRAuton + t_motorSpeed);
            Movement.moveBL(BLAuton - t_motorSpeed);
            Movement.moveBR(BRAuton - t_motorSpeed);

            // if the pid loop has reached target
            if (((error <= tolerance) && (error >= -tolerance)))
            {
                reachedGoal = true;
            }
            else
            {
                reachedGoal = false;
            }

            if (reachedGoal && t_reachedGoal)
            {
                Movement.moveLeft(0);
                Movement.moveRight(0);
                break;
            }

            delay(PIDspeed);
        }
        return 1;
    }

    void resetOdom()
    {
        X = 0;
        Y = 0;
        rotation = 0;
    }

    static void updatePos(void *)
    {
        //I derived the original formula and for the reiteration and added wheel i combined it with work done here
        //https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-186-mobile-autonomous-systems-laboratory-january-iap-2005/study-materials/odomtutorial.pdf
        double wheelCircumfrence = 10.11;
        double wheelSmallCircumfrence = 8.65795;
        //bigger increases angle more
        double wheelSeperation = 9.2;
        double head = rotation;
        double rightOdomVal;
        double leftOdomVal;
        double middleOdomVal;
        while (true)
        {
            //just in case i forget to code properly
            if (isnan(X))
            {
                X = 0;
            }
            if (isnan(Y))
            {
                Y = 0;
            }
            //update heading part
            double changeOfHeading = (leftOdom.get() - rightOdom.get()) / wheelSeperation;
            rotation += changeOfHeading;
            //set new
            //TODO i thought this went before but try accuracy when its after computed distance moved idk i think its right
            head = rotation;
            //find isolated forward and sideways movement
            double forwardMovement = (rightOdom.get() + leftOdom.get()) / 2;
            double sidewaysMovement = middleOdom.get();
            //to distance
            forwardMovement = myMath.toInch(forwardMovement, wheelCircumfrence);
            sidewaysMovement = myMath.toInch(sidewaysMovement, wheelSmallCircumfrence);

            // forward wheels in relation to coordinates
            //HEY YOU
            //mit student says switch cos and sin on these here if your code breaks its bc hes smarter than you and you need to fix this
            Y += forwardMovement * cos(head * M_PI / 180);
            X += forwardMovement * sin(head * M_PI / 180);
            //crackhead
            // sideways wheel in relation to cooridantes
            Y -= sidewaysMovement * sin(head * M_PI / 180);
            X += sidewaysMovement * cos(head * M_PI / 180);
            //debug
            printf("right: %f", rightOdom.get());
            printf("left: %f", leftOdom.get());
            // reset
            rightOdom.reset();
            leftOdom.reset();
            middleOdom.reset();
            c::task_delay(posDelay);
        }
    }

    void testOdom()
    {
        //PIDTurn(180);
        PIDTurn(90);
    }

    void testOdom2()
    {
        //IDMove(0, 15);
        PIDMove(0, 0);
        PIDTurn(0);
    }

    // set current position of bongo
    void setPos(double x, double y)
    {
        X = x;
        Y = y;
    }

    // prints to debug screen current position of bongo
    void debugPos()
    {
        std::string xPos = "X: " + std::to_string(X);
        std::string yPos = "Y: " + std::to_string(Y);
        std::string roationPos = "roation: " + std::to_string(rotation);
        lv_label_set_text(debugXLabel, xPos.c_str());
        lv_label_set_text(debugYLabel, yPos.c_str());
        lv_label_set_text(debugRotationLabel, roationPos.c_str());
    }

    // change current team (swap) pressing L1 and L2 at same time when called
    void changeTeam()
    {
        if ((master.get_digital(E_CONTROLLER_DIGITAL_L1)) &&
            (master.get_digital(E_CONTROLLER_DIGITAL_L2)))
        {
            teamIsBlue = !teamIsBlue;
        }
    }

    //calls upon subsystems and updates them
    static void updateEverything(void *)
    {
        while (true)
        {
            //update subsystem motors in their methods respectivley 
            c::task_delay(10);
        }
    }

    //returns true if bongo has initialized
    bool isinit()
    {
        return initz;
    }

    //starts up threads
    void initThreads()
    {
        // control updates from intake uptake flywheel
        //Task control(updateEverything, nullptr, TASK_PRIORITY_DEFAULT,
        //             TASK_STACK_DEPTH_DEFAULT, "control");

        // track location
        Task updatePosition(updatePos, nullptr, TASK_PRIORITY_DEFAULT,
                            TASK_STACK_DEPTH_DEFAULT, "updatePos");

        //handler for outakepros
        //Task sort(handleOutake, nullptr, TASK_PRIORITY_DEFAULT,
        //          TASK_STACK_DEPTH_DEFAULT, "sort");
        initz = true;
    }

    //async PID for changing angle to mogo
    double lineUpMogo(int sideNum, bool moveThem = false)
    {
        // take signature from side
        vision_object_s_t mogo;
        //objects on right will require motors to move in opposite direction of left and back
        int rotationMultiplier;
        switch (sideNum)
        {
        case leftSideNum:
            mogo = leftEye.get_by_sig(0, MOGO_CUSTOM_YELLOW_NUM);
            rotationMultiplier = 1;
            break;
        case RightSideNum:
            mogo = rightEye.get_by_sig(0, MOGO_CUSTOM_YELLOW_NUM);
            rotationMultiplier = -1;
            break;
        case BackSideNum:
            mogo = backEye.get_by_sig(0, MOGO_CUSTOM_YELLOW_NUM);
            rotationMultiplier = 1;
            break;
        }
        if (mogo.width >= widthLimit)
        {
            width = mogo.width;
            CX = mogo.x_middle_coord;
            //custom formula for dist turn
            printf("Error: %f", error);
            //158 is center of vison view
            targetCX = 158 - CX + offset;
            error = CX - targetCX;
            // RN its just a P loop D if you add this + Dval * ((error - error_past) / 5) /*time delay*/
            Movement.addToFLspeed(Pval * error * rotationMultiplier);
            Movement.addToFRspeed(Pval * -error * rotationMultiplier);
            Movement.addToBLspeed(Pval * error * rotationMultiplier);
            Movement.addToBRspeed(Pval * -error * rotationMultiplier);
            if (moveThem)
            {
                Movement.move();
            }
            error_past = error;
        }
        return error;
    }
};

extern Robot Bongo;

#endif // ifndef ROBOT