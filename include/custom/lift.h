#ifndef LIFT
#define LIFT
#include "math.h"
class LiftClass
{
private:
    double maxLiftSpeed = 100;
    double torqueSpeed = 50;
    double PIDdelay = 1;
    bool emergencyStop = false;
    bool promptConfirm = false;
    bool bigTowerIsOnLeft = true;
    bool manualControl = false;
    bool turnOffManualControl = false;
    bool reachedHalfDrop = false;
    //update bools
    bool autonLoadRings;
    bool autonUserDrop;
    int autonUserDropLevel = 1;
    bool autonUserDropSideLeft = true;
    // Screen/Interface stuff 
    bool autonMoving = false;
    bool confirmState = false;
    double previousJoystickX = 0;
    double previousJoystickY = 0;
    double currentJoystickX = 0;
    double currentJoystickY = 0;
    double tweakSpeedDial = 8;
    //X arm PID
    double XPval = .15;
    double XIval = 0;
    double XDval = .5;
    double XlastError = 0;
    double Xtotal = 0;
    double Xtolerance = 3;
    //Y arm PID
    double YPval = 1;
    double YIval = 0;
    double YDval = 0;
    double YlastError = 0;
    double Ytotal = 0;
    double Ytolerance = 5;
    //Points
    double XidleTop = 973;
    double XidleBottom = 0;
    double Yidle = 2190;
    double Xrings = 0;
    double Yrings = 1107;
    //Drops
    double Xleft = 934;
    double Xright = 879;
    double Ytall = 2375;
    double Ymed = 2300;
    double XmedLeft = -258; //pos is right
    double XmedRight = 258;
    double Ysmall = 1958;
public:
    Math myMath;
    //stops motor
    void stopAll()
    {
        Xarm.move_velocity(0);
        Yarm.move_velocity(0);
    }

    void stopXarm(){
        Xarm.move_velocity(0);
    }

    void stopYarm(){
        Yarm.move_velocity(0);
    }

    bool PIDMoveXarm(double target){ // fmod maybe
        double Xerror = target - Xarm.get_position(); // we just want any value in that are dont care about rotaion
        double speed = XPval * Xerror + XIval * Xtotal - XDval * ((Xerror - XlastError) / PIDdelay);
        if(speed <= 20){
            Xtotal += Xerror;
        }

        //Spin motor
        printf("spinning X at %f\n", speed);
        if(speed > 20){
            speed = 20;
        } else if (speed < -20){
            speed = -20;
        }
        printf("corrected spinning X at %f\n", speed);
        Xarm.move_velocity(myMath.toRPM(!bigTowerIsOnLeft, speed, Xarm.get_gearing()));

        //check if its there
        printf("Xerror at %f\n", Xerror);
        if(fabs(Xerror) <= Xtolerance){
            printf("Stopping Xarm");
            stopXarm();
            Xtotal = 0;
            return true;
        }
        XlastError = Xerror;
        return false;
    }

    bool PIDMoveYarm(double target){
        double Yerror = target - liftPot.get_value();
        double speed = YPval * Yerror + YIval * Ytotal - YDval * ((Yerror - YlastError) / PIDdelay);
        if(speed <= 20){
            Ytotal += Yerror;
        }

        //Spin motor
        printf("spinning Y at %f\n", speed);
        Yarm.move_velocity(myMath.toRPM(true, speed, Yarm.get_gearing()));

        //check if its there
        printf("Yerror: %f", fabs(Yerror));
        if(fabs(Yerror) <= Ytolerance){
            printf("Stopping Yarm");
            stopYarm();
            Ytotal = 0;
            return true;
        }
        YlastError = Yerror;
        return false;
    }
    
    bool goToPoint(double X, double Y, bool Xfirst){
        printf("going to point: %f, %f\n", X, Y);
        if(emergencyStop){
            stopAll();
            return true;
        }
        //half drop prevents it from blocking second pid update if the first stage has been reached but is out of place
        if(Xfirst){
            if(!PIDMoveXarm(X) && !reachedHalfDrop){ // if Xarm isnt in the right spot return false
                return false;
            } else {
                reachedHalfDrop = true;
                PIDMoveXarm(X);
            }
            if(PIDMoveYarm(Y)){ // if Y arm is in right spot return true
                reachedHalfDrop = false;
                return true;
            }
        } else {
            if(!PIDMoveYarm(Y) && !reachedHalfDrop){ // if Xarm isnt in the right spot return false
                return false;
            } else {
                reachedHalfDrop = true;
                PIDMoveYarm(Y); // idle keep arm in place while other moves
            }
            if(PIDMoveXarm(X)){ // if Y arm is in right spot return true
                reachedHalfDrop = false;
                return true;
            }
        }
        return false;
    }

    bool goToIdle(bool xfirst){
        return goToPoint(XidleBottom, Yidle, xfirst);
    }

    bool goToRings(){
        return goToPoint(Xrings, Yrings, true);
    }

    void goToIdleAuton(bool top){
        while(!goToIdle(top) && !emergencyStop){
            printf("going to idle auton delay\n");
            delay(PIDdelay);
        }
    }

    void loadRingsSequence(){
        goToIdleAuton(false);
        ringLock.set_value(HIGH);
        while(!goToRings() && !emergencyStop){
            delay(PIDdelay);
        }
        ringLock.set_value(LOW);
        goToIdleAuton(false);
    }

    double getLevelCoord(int level){
        switch(level){
            case 1:
                return Ysmall;
                break;
            case 2:
                return Ymed;
                break;
            case 3:
                return Ytall;
                break;
        }
        return 0;
    }

    double getXCoord(int level, bool left){
        switch(level){
            case 1:
                return left? XmedLeft: XmedRight;
                break;
            case 2:
                return left? XmedLeft: XmedRight;
                break;
            case 3:
                return left? Xleft: Xright;
                break;
        }
        return 0;
    }

    void confirmationSequence(){
        while(!confirmState){
            if(emergencyStop){
                stopAll();
                break;
            }
            Xarm.move_velocity(myMath.toRPM(false, currentJoystickX / tweakSpeedDial, Xarm.get_gearing()));
            Yarm.move_velocity(myMath.toRPM(false, currentJoystickY / tweakSpeedDial, Yarm.get_gearing()));
            delay(PIDdelay);
        }
    }

    void goToDrop(){
        printf("starting drop\n");
        goToIdleAuton(false);
        printf("Is idled\n");
        double Xcoord = getXCoord(autonUserDropLevel, autonUserDropSideLeft);
        double Ycoord = getLevelCoord(autonUserDropLevel);
        while(!goToPoint(Xcoord, Ycoord, false) && !emergencyStop){
            printf("going to point\n");
            delay(PIDdelay);
        }
        if(emergencyStop){
            stopAll();
        }
        confirmState = false; //just in case you hit it before the confirm it would break
        promptConfirm = true;
        confirmationSequence();
        delay(1001);
        promptConfirm = false;
        confirmState = false;
        goToIdleAuton(true);
    }

    void update()
    {
        // check for user updates from other task to run auto arm functions 
        if(autonLoadRings && !manualControl){
            printf("starting load rings");
            loadRingsSequence();
            autonLoadRings = false;
            autonMoving = false;
        }
        if(autonUserDrop && !manualControl){
            printf("herer\n");
            goToDrop();
            printf("shouldnt be here\n");
            autonUserDrop = false;
            autonMoving = false;
        }
        if(turnOffManualControl && manualControl){
            printf("manual controll");
            turnOffManualControl = false;
            manualControl = false;
            Yarm.move_velocity(0);
        }
    }

    //auton buttons

    void loadRings(){
        autonMoving = true;
        autonLoadRings = true;
    }

    void userDrop(){
        autonMoving = true;
        autonUserDrop = true;
    }

    //other important ones

    void confirmDrop(){ // TODO setup piston unlock here
        ringLock.set_value(HIGH);
        confirmState = true;
    }

    void toggleEmergencyStop(){
        emergencyStop = !emergencyStop;
    }

    void toggleBigSide(){
        bigTowerIsOnLeft = !bigTowerIsOnLeft;
    }
    
    //tower drop helper functions

    void increaseLevel(){
        if(autonUserDropLevel < 3){
            autonUserDropLevel++;
        }
    }

    void decreaseLevel(){
        if(autonUserDropLevel > 1){
            autonUserDropLevel--;
        }
    }

    //joystick controllers

    void setJoystickX(double val){
        previousJoystickX = currentJoystickX;
        currentJoystickX = val;
    }

    void setJoystickY(double val){
        previousJoystickY = currentJoystickY;
        currentJoystickY = val;
    }

    void handleVals(){
        //joystick levels
        if(currentJoystickY > 50 && previousJoystickY < 50){
            increaseLevel();
        } else if (currentJoystickY < -50 && previousJoystickY > -50){
            decreaseLevel();
        }

        //side of sort
        if(currentJoystickX > 50 && previousJoystickX < 50){
            autonUserDropSideLeft = false;
        } else if (currentJoystickX < -50 && previousJoystickX > -50){
            autonUserDropSideLeft = true;
        }
    }

    //power lifting 

    void stopPowerLifting(){
        turnOffManualControl = true;
    }

    void powerLiftUp(){
        if(!autonMoving){ //TODO true and false here might need to be switched
            manualControl = true;
            Yarm.move_velocity(myMath.toRPM(true, torqueSpeed, Yarm.get_gearing()));
        }
    }

    void powerLiftDown(){
        if(!autonMoving){
            manualControl = true;
            Yarm.move_velocity(myMath.toRPM(false, torqueSpeed, Yarm.get_gearing()));
        }
    }

    //Controller text
    void updateText(){
        //partner.clear();
        //partner.set_text(0,0,"hi");
        if(emergencyStop){
            stopAll();
            partner.clear();
            delay(55);
            partner.set_text(1,4,"Emergency Stop");
        } else if(manualControl){
            partner.clear();
            delay(55);
            partner.set_text(1,4,"Manual Control");
        } else if(promptConfirm && autonUserDrop){
            partner.clear();
            delay(55);
            partner.set_text(1,4,"Confirm?");
        } else {
           std::string firstLinePart1 =  bigTowerIsOnLeft? "0      " : "       ";
           std::string firstLinePart2 = (autonUserDropLevel >= 3)? "000000" : "      " ;
           std::string firstLinePart3 = !bigTowerIsOnLeft?"      0":"       ";
           std::string secondLinePart1 = autonUserDropSideLeft?"Left  ":"       ";
           std::string secondLinePart2 = (autonUserDropLevel >= 2)?"000000":"      ";
           std::string secondLinePart3 = !autonUserDropSideLeft?"  Right":"       ";
           std::string thirdLinePart1 = bigTowerIsOnLeft?"0      ":"       ";
           std::string thirdLinePart2 = (autonUserDropLevel >= 1)?"000000":"       " ;
           std::string thirdLinePart3 = !bigTowerIsOnLeft?"      0":"      ";
           partner.clear();
           delay(55);
           std::string firstLine = "";
           firstLine.append(firstLinePart1);
           firstLine.append(firstLinePart2);
           firstLine.append(firstLinePart3);
           std::string secondLine = "";
           secondLine.append(secondLinePart1);
           secondLine.append(secondLinePart2);
           secondLine.append(secondLinePart3);
           std::string thirdLine = "";
           thirdLine.append(thirdLinePart1);
           thirdLine.append(thirdLinePart2);
           thirdLine.append(thirdLinePart3);
           partner.set_text(0,0, firstLine);
           delay(55);
           partner.set_text(1,0, secondLine);
           delay(55);
           partner.set_text(2,0, thirdLine);
        }
        delay(100);
    }
};
#endif // ifndef LIFT
