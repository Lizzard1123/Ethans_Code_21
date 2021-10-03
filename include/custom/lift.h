#ifndef LIFT
#define LIFT
#include "math.h"
class LiftClass
{
private:
    double maxLiftSpeed = 100;
    double torqueSpeed = 50;
    double PIDdelay = 10;
    bool emergencyStop = false;
    bool bigTowerIsOnLeft = true;
    bool manualControl = false;
    bool turnOffManualControl = false;
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
    double tweakSpeedDial = 4;
    //X arm PID
    double XPval = 1;
    double XIval = 0;
    double XDval = 0;
    double XlastError = 0;
    double Xtotal = 0;
    double Xtolerance = 1;
    //Y arm PID
    double YPval = 1;
    double YIval = 0;
    double YDval = 0;
    double YlastError = 0;
    double Ytotal = 0;
    double Ytolerance = 1;
    //Points
    double XidleTop;
    double XidleBottom;
    double Yidle;
    double Xrings;
    double Yrings;
    //Drops
    double Xleft;
    double Xright;
    double Ytall;
    double Ymed;
    double Ysmall;
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
        Xarm.move_velocity(myMath.toRPM(bigTowerIsOnLeft, speed, Xarm.get_gearing()));

        //check if its there
        if(fabs(Xerror - target) < Xtolerance){
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
        Yarm.move_velocity(myMath.toRPM(false, speed, Yarm.get_gearing()));

        //check if its there
        if(fabs(Yerror - target) < Ytolerance){
            stopYarm();
            Ytotal = 0;
            return true;
        }
        YlastError = Yerror;
        return false;
    }
    
    bool goToPoint(double X, double Y, bool Xfirst){
        bool reachedPoints = false;
        if(Xfirst){
            if(!PIDMoveXarm(X)){ // if Xarm isnt in the right spot return false
                return false;
            }
            PIDMoveXarm(X);
            if(PIDMoveYarm(Y)){ // if Y arm is in right spot return true
                return true;
            }
        } else {
            if(!PIDMoveYarm(Y)){ // if Xarm isnt in the right spot return false
                return false;
            }
            PIDMoveYarm(Y); // idle keep arm in place while other moves
            if(PIDMoveXarm(X)){ // if Y arm is in right spot return true
                return true;
            }
        }
        return false;       
    }

    bool goToIdle(bool topIdle){
        return goToPoint(topIdle? XidleTop : XidleBottom, Yidle, topIdle);
    }

    bool goToRings(){
        return goToPoint(Xrings, Yrings, true);
    }

    void goToIdleAuton(bool top){
        while(goToIdle(top) && !emergencyStop){
            delay(PIDdelay);
        }
    }

    void loadRingsSequence(){
        goToIdleAuton(false);
        while(goToRings() && !emergencyStop){
            delay(PIDdelay);
        }
        // TODO setup piston lock here
        goToIdleAuton(true);
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

    void confirmationSequence(){
        while(confirmState){
            Xarm.move_velocity(myMath.toRPM(false, currentJoystickX / tweakSpeedDial, Xarm.get_gearing()));
            Yarm.move_velocity(myMath.toRPM(false, currentJoystickY / tweakSpeedDial, Yarm.get_gearing()));
            delay(PIDdelay);
        }
        confirmState = false;
    }

    void goToDrop(){
        goToIdleAuton(false);
        double Xcoord = autonUserDropSideLeft? Xleft : Xright;
        double Ycoord = getLevelCoord(autonUserDropLevel);
        while(goToPoint(Xcoord, Ycoord, false) && !emergencyStop){
            delay(PIDdelay);
        }
        confirmState = false; //just in case you hit it before the confirm it would break
        confirmationSequence();
        goToIdleAuton(false);
    }

    void update()
    {
        // check for user updates from other task to run auto arm functions 
        if(autonLoadRings && !manualControl){
            loadRingsSequence();
            autonLoadRings = false;
            autonMoving = false;
        }
        if(autonLoadRings && !manualControl){
            goToDrop();
            autonUserDrop = false;
            autonMoving = false;
        }
        if(turnOffManualControl){
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
            partner.clear();
            delay(55);
            partner.set_text(1,4,"Emergency Stop");
        } else if(manualControl){
            partner.clear();
            delay(55);
            partner.set_text(1,4,"Manual Control");
        } else if(confirmState){
            partner.clear();
            delay(55);
            partner.set_text(1,4,"Confirm?");
        } else {
            /*
            //update side of big tower
            if(bigTowerIsOnLeft){
                partner.set_text(0,0,"▉");
                partner.set_text(1,0,"▉");
                partner.set_text(2,0,"▉");
            } else {
                partner.set_text(0,18,"▉");
                partner.set_text(1,18,"▉");
                partner.set_text(2,18,"▉");
            }
            //update side picker
            if(autonUserDropSideLeft){
                partner.set_text(1,2,"LEFT");
            } else {
                partner.set_text(1,12,"RIGHT");
            }
            //update level of choice
            for(int i = 0; i < autonUserDropLevel; i++){
                partner.print(i,7, "▉▉▉▉▉");
            }
            */
           //
           std::string totalLine = "";
           std::string firstLinePart1 =  bigTowerIsOnLeft? "▉    " : "     ";
           totalLine.append(firstLinePart1);
           std::string firstLinePart2 = (autonUserDropLevel == 3)? "▉▉▉▉▉" : "     " ;
           totalLine.append(firstLinePart2);
           std::string firstLinePart3 = !bigTowerIsOnLeft?"    ▉":"     ";
           totalLine.append(firstLinePart3);
           std::string secondLinePart1 = autonUserDropSideLeft?"Left ":"     ";
           totalLine.append(secondLinePart1);
           std::string secondLinePart2 = (autonUserDropLevel == 2)?"▉▉▉▉▉":"     ";
           totalLine.append(secondLinePart2);
           std::string secondLinePart3 = !autonUserDropSideLeft?"Right":"     ";
           totalLine.append(secondLinePart3);
           std::string thirdLinePart1 = bigTowerIsOnLeft?"▉    ":"     ";
           totalLine.append(thirdLinePart1);
           std::string thirdLinePart2 = (autonUserDropLevel == 1)?"▉▉▉▉▉":"     " ;
           totalLine.append(thirdLinePart2);
           std::string thirdLinePart3 = !bigTowerIsOnLeft?"     ":"    ▉";
           totalLine.append(thirdLinePart3);
           partner.set_text(0,0, totalLine);
           printf(totalLine.data()); //.c_str();
           //if not
           /*
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
           */
        }
        delay(55);
    }
};
#endif // ifndef LIFT
