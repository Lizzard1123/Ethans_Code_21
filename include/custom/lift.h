#ifndef LIFT
#define LIFT
#include "math.h"
class LiftClass
{
private:
    double maxLiftSpeed = 100;
    double PIDdelay = 10;
    bool emergencyStop = false;
    bool bigTowerIsOnLeft = true;
    //update bools
    bool autonLoadRings;
    // Screen/Interface stuff 
    Controller outputController = partner; 
    bool autonMoving = false;
    bool confirmState = false;
    double tweakSpeedDial = 4;
    //X arm PID
    double XPval = 1;
    double XIval = 0;
    double XDval = 0;
    double Xerror;
    double XlastError = 0;
    double Xtotal = 0;
    double Xtolerance = 1;
    //Y arm PID
    double YPval = 1;
    double YIval = 0;
    double YDval = 0;
    double Yerror;
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
    double torqueSpeed = 50;
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

    bool PIDMoveXarm(double target){
        Xerror = target - Xarm.getPosition() % 360; // we just want any value in that are dont care about rotaion
        double speed = XPval * Xerror + XIval * Xtotal - XDval * ((Xerror - XlastError) / PIDdelay);
        if(speed <= 20){
            Xtotal += Xerror;
        }

        //Spin motor
        Xarm.move_velocity(myMath.toRPM(bigTowerIsOnLeft, speed, Xarm.get_gearing()));

        //check if its there
        if(fabs(Xerror - Xtarget) < Xtolerance){
            stopXarm();
            Xtotal = 0;
            return true;
        }
        XlastError = Xerror;
        return false;
    }

    bool PIDMoveYarm(double pos){
        Yerror = target - liftPot.value();
        double speed = YPval * Yerror + YIval * Ytotal - YDval * ((Yerror - YlastError) / PIDdelay);
        if(speed <= 20){
            Ytotal += Yerror;
        }

        //Spin motor
        Yarm.move_velocity(myMath.toRPM(false, speed, Yarm.get_gearing()));

        //check if its there
        if(fabs(Yerror - Ytarget) < Ytolerance){
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
            delay(PIDdelay)
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
            delay(PIDdelay);
        }
        confirmState = false;
    }

    void goToDrop(int level, bool left){
        goToIdleAuton(false);
        double Xcoord = left? Xleft : Xright;
        double Ycoord = getLevelCoord(level);
        while(goToPoint(Xcoord, Ycoord, false) && !emergencyStop){
            delay(PIDdelay)
        }
        confirmState = false; //just in case you hit it before the confirm it would break
        confirmationSequence();
        goToIdleAuton(false);
    }

    void update()
    {
        // check for user updates from other task to run auto arm functions 
        if(autonLoadRings){
            loadRingsSequence();
            autonLoadRings = false;
        }
    }

    void loadRings(){
        autonMoving = true;
        autonLoadRings = true;
    }

    void confirmDrop(){ // TODO setup piston unlock here
        confirmState = true;
    }

    void toggleEmergencyStop(){
        emergencyStop = !emergencyStop;
    }

    void toggleBigSide(){
        bigTowerIsOnLeft = !bigTowerIsOnLeft;
    }
};
#endif // ifndef LIFT
