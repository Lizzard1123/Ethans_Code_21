#ifndef ROBOT
#define ROBOT
#include "wings.h"
#include "lift.h"
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
    double mt_offset = -25;
    double mt_error;
    double mt_targetCX;
    double mt_CX;
    double mt_width;
    double mt_error_past = 0;
    double mt_Dval = 2;
    double mt_Pval = .3;
    double mt_widthLimit = 20;

    double t_tolerance = .5;
    double t_Pval = 2.5;
    double t_Ival = 0;//double Ival = .1;// double Ival = 0;
    double t_Dval = 0;//double Dval = .7;// double Dval = 0;

    int PIDspeed = 50;// in ms
    double m_tolerance = .2;// tolerance in inches
    double m_Pval = 8; //13
    double m_Ival = .25; //0
    double m_Dval = 0; //0

public:
    /*robot subsytems*/
    static WingClass Wings;
    static LiftClass Lift;
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
    int PIDMove(double targetX, double targetY, double maxspeed = 100)
    {
        bool reachedGoal;
        double speed = 0;
        double prevError;
        double error;
        double derivative;
        double integral = 0;

        while (true)
        {
            // find the error distance bertween current and target point
            error = myMath.TwoPointsDistance(X, Y, targetX, targetY);

            // find the intergral part for I
            if ((speed != 0) && (speed <= 15) && (integral < 1000) && (error < 1000))
            {
                integral += error;
            }

            // find the derivative part for D
            //this should be change of error 
            derivative =  error / PIDspeed;

            // PID ALGO
            speed = (error * m_Pval) + (integral * m_Ival) + (derivative * m_Dval);

            // get current angle
            double Dangle = myMath.angleBetween(X, Y, targetX, targetY);

            // fancy algo
            double FLAuton = myMath.sRound(
                myMath.multiplier(FLnum, rotation, Dangle) * speed, 3);
            double FRAuton = myMath.sRound(
                myMath.multiplier(FRnum, rotation, Dangle) * speed, 3);
            double BLAuton = myMath.sRound(
                myMath.multiplier(BLnum, rotation, Dangle) * speed, 3);
            double BRAuton = myMath.sRound(
                myMath.multiplier(BRnum, rotation, Dangle) * speed, 3);
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
            printf("rotation %f  \n", rotation);
            printf("Dangle %f \n", Dangle);
            printf("error %f \n", error);
            printf("X:  %f \n", X);
            printf("Y:  %f \n", Y);
            // Drive Bongo
            Movement.moveFL(myMath.maxSpeed(FLAuton, maxspeed));
            Movement.moveFR(myMath.maxSpeed(FRAuton, maxspeed));
            Movement.moveBL(myMath.maxSpeed(BLAuton, maxspeed));
            Movement.moveBR(myMath.maxSpeed(BRAuton, maxspeed));

            // if the pid loop has reached target
            if (fabs(error) <= m_tolerance)
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
    int PIDTurn(double target, double maxspeed = 100)
    {
        double motorSpeed = 0;
        double turnTarget = target;
        bool reachedGoal;
        double error;
        double Derivative = 0;
        double integralone = 0;

        while (true)
        {
            //printf("Heading: %f", headingVal);
            // find the error of both sides  for P
            error = turnTarget - rotation;

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
            motorSpeed = (error * t_Pval) + (integralone * t_Ival) + (Derivative * t_Dval);

            // Drive Bongo
            Movement.moveLeft(myMath.maxSpeed(motorSpeed, maxspeed));
            Movement.moveRight(myMath.maxSpeed(-motorSpeed, maxspeed));

            // if the pid loop has reached target
            if (fabs(error) <= t_tolerance)
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
    int PIDMoveTurn(double targetX, double targetY, double target, double maxspeed = 100)
    {
        //move PID info
        bool m_reachedGoal;
        double m_speed = 0;
        double m_prevError;
        double m_error;
        double m_derivative;
        double m_integral = 0;

        //turn PID info
        double t_speed = 0;
        bool t_reachedGoal;
        double t_error;
        double t_derivative = 0;
        double t_integral = 0;

        while (true)
        {
            // find the error distance bertween current and target point
            m_error = myMath.TwoPointsDistance(X, Y, targetX, targetY);
            // find the error of both sides for P
            t_error = target - rotation;

            // find the intergral part for I
            if ((m_speed != 0) && (m_speed <= 15) && (m_integral < 1000) && (m_error < 1000))
            {
                m_integral += m_error;
            }

            // find the derivative part for D
            //this should be change of error 
            m_derivative =  m_error / PIDspeed;

            // PID ALGO
            m_speed = (m_error * m_Pval) + (m_integral * m_Ival) + (m_derivative * m_Dval);
            // PID ALGO
            t_speed = (t_error * t_Pval) + (t_integral * t_Ival) + (t_derivative * t_Dval);

            // get current angle
            double Dangle = myMath.angleBetween(X, Y, targetX, targetY);

            // fancy algo
            double FLAuton = myMath.sRound(
                myMath.multiplier(FLnum, rotation, Dangle) * m_speed + t_speed, 3);
            double FRAuton = myMath.sRound(
                myMath.multiplier(FRnum, rotation, Dangle) * m_speed - t_speed, 3);
            double BLAuton = myMath.sRound(
                myMath.multiplier(BLnum, rotation, Dangle) * m_speed + t_speed, 3);
            double BRAuton = myMath.sRound(
                myMath.multiplier(BRnum, rotation, Dangle) * m_speed - t_speed, 3);
            double under = myMath.greatest(fabs(FLAuton + t_speed), fabs(FRAuton + t_speed),
                                           fabs(BLAuton + t_speed), fabs(BRAuton + t_speed)) /
                           100;
            FLAuton = (FLAuton / under);
            FRAuton = (FRAuton / under);
            BLAuton = (BLAuton / under);
            BRAuton = (BRAuton / under);
            printf("FLAuton %f \n", FLAuton);
            printf("FRAuton %f \n", FRAuton);
            printf("BLAuton %f \n", BLAuton);
            printf("BRAuton %f  \n", BRAuton);
            printf("Heading: %f", rotation);
            printf("Dangle %f \n", Dangle);
            printf("target %f \n", target);
            printf("error %f \n", m_error);
            printf("error %f \n", t_error);
            printf("X:  %f \n", X);
            printf("Y:  %f \n", Y);
            // Drive Bongo
            Movement.moveFL(myMath.maxSpeed(FLAuton, maxspeed));
            Movement.moveFR(myMath.maxSpeed(FRAuton, maxspeed));
            Movement.moveBL(myMath.maxSpeed(BLAuton, maxspeed));
            Movement.moveBR(myMath.maxSpeed(BRAuton, maxspeed));

            // if the pid loop has reached target
            if (fabs(m_error) <= m_tolerance)
            {
                m_reachedGoal = true;
            }
            else
            {
                m_reachedGoal = false;
            }

            if (fabs(t_error) <= t_tolerance)
            {
                t_reachedGoal = true;
            }
            else
            {
                t_reachedGoal = false;
            }

            if (m_reachedGoal && t_reachedGoal)
            {
                Movement.moveLeft(0);
                Movement.moveRight(0);
                break;
            }
            //delay
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
        double wheelSeperation = 9.48; //9.2
        double head = rotation;
        double rightOdomVal;
        double leftOdomVal;
        double middleOdomVal;
        while (true)
        {
            //just in case i forget to code properly
            if (isnan(X) || fabs(X) > 144)
            {
                X = 0;
            }
            if (isnan(Y) || fabs(Y) > 144)
            {
                Y = 0;
            }
            
            
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
            //heading//update heading part
            double changeOfHeading = (leftOdom.get() - rightOdom.get()) / wheelSeperation;
            rotation += changeOfHeading;
            //set new
            //TODO i thought this went before but try accuracy when its after computed distance moved idk i think its right
            head = rotation;
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
        PIDTurn(0);
    }

    void testOdom2()
    {
        PIDMoveTurn(0, 0, 0, 50);
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
        std::string xArmDebug = "XArm: " + std::to_string(Xarm.get_position());
        std::string yArmDebug = "YArm: " + std::to_string(liftPot.get_value());
        lv_label_set_text(debugXLabel, xPos.c_str());
        lv_label_set_text(debugYLabel, yPos.c_str());
        lv_label_set_text(debugRotationLabel, roationPos.c_str());
        lv_label_set_text(debugXarmLabel, xArmDebug.c_str());
        lv_label_set_text(debugYarmLabel, yArmDebug.c_str());
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
    static void updateWings(void *)
    {
        while (true)
        {
            //update subsystem motors in their methods respectivley 
            Wings.update();
            c::task_delay(10);
        }
    }

    //calls upon subsystems and updates them
    static void updateLift(void *)
    {
        while (true)
        {
            //update subsystem motors in their methods respectivley 
            Lift.update();
            c::task_delay(10);
        }
    }

    //updates controller text
    static void updateControllerText(void *)
    {
        while (true)
        {
            //update subsystem motors in their methods respectivley 
            Lift.updateText();
        }
    }

    //updates controller rumble
    static void updateControllerRumble(void *)
    {
        bool forGood = false;
        bool forBad = true;
        while (true)
        {
            //update subsystem motors in their methods respectivley 
            if(forGood){
                if(competition::is_connected() && !competition::is_autonomous()){
                    //halfway and 30 sec timer rumble for drivers
                    c::task_delay(37500);
                    master.rumble("....");
                    partner.rumble("....");
                    c::task_delay(37500);
                    master.rumble("----");
                    partner.rumble("----");
                }
            } else if (forBad){
                c::task_delay(69420);
                master.rumble(".--. ");
                master.rumble(". ");
                master.rumble("-. ");
                master.rumble(".. ");
                master.rumble("... ");
                partner.rumble(".--. ");
                partner.rumble(". ");
                partner.rumble("-. ");
                partner.rumble(".. ");
                partner.rumble("... ");
            }
            
            
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
        // control updates from Lift
        Task controlLift(updateLift, nullptr, TASK_PRIORITY_DEFAULT,
                     TASK_STACK_DEPTH_DEFAULT, "control lift");
        // control updates from wings
        Task controlWings(updateWings, nullptr, TASK_PRIORITY_DEFAULT,
                     TASK_STACK_DEPTH_DEFAULT, "control wings");
        //controlls controller controlling control text
        printf("updateTexter");
        Task controlText(updateControllerText, nullptr, TASK_PRIORITY_DEFAULT,
                     TASK_STACK_DEPTH_DEFAULT, "control controller Text");
        //rumble her
        Task controlRumble(updateControllerRumble, nullptr, TASK_PRIORITY_DEFAULT,
                     TASK_STACK_DEPTH_DEFAULT, "control controller rumble");
        // track locationFty
        //Task updatePosition(updatePos, nullptr, TASK_PRIORITY_DEFAULT,
        //                    TASK_STACK_DEPTH_DEFAULT, "updatePos");
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
        { //TODO fix this rotation multiplier idk which is which
        case LeftSideNum:
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
        if (mogo.width >= mt_widthLimit)
        {
            mt_width = mogo.width;
            mt_CX = mogo.x_middle_coord;
            //custom formula for dist turn
            printf("Error: %f", mt_error);
            //158 is center of vison view
            mt_targetCX = 158 - mt_CX + mt_offset;
            mt_error = mt_CX - mt_targetCX;
            // RN its just a P loop D if you add this + Dval * ((error - error_past) / 5) /*time delay*/
            Movement.addToFLspeed(mt_Pval * mt_error * rotationMultiplier);
            Movement.addToFRspeed(mt_Pval * -mt_error * rotationMultiplier);
            Movement.addToBLspeed(mt_Pval * mt_error * rotationMultiplier);
            Movement.addToBRspeed(mt_Pval * -mt_error * rotationMultiplier);
            if (moveThem)
            {
                Movement.move();
            }
            mt_error_past = mt_error;
        }
        return mt_error;
    }
};

extern Robot Bongo;

#endif // ifndef ROBOT