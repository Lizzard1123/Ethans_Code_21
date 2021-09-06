#if 0 // OUTDATED
#include "custom/robot.h"
/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol()
{
    //space to run stuff once before begin driving
    // change teams failsafe
    if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_A))
    {
        Bongo.changeTeam();
    }
    
    //toggle intake
    if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)){
        Bongo.Intake.toggle();
    }
    
    //locks
    if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)){
        Bongo.Wings.toggleLeftLock();
    } else if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)){
        Bongo.Wings.toggleRightLock();
    } else if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
        Bongo.Tail.toggleLock();
    }
    while (true)
    {
        //prints to screen the position and rotation of bongo
        Bongo.debugPos();

        // tyler control
        Bongo.tylerControl();
        // catie control
        //Bongo.catieControl();

        //Left wing
        if(master.get_digital(E_CONTROLLER_DIGITAL_L1)){ //tilt up
            Bongo.Wings.tiltLeftWing(Bongo.Wings.speedMedium);
        } else if(master.get_digital(E_CONTROLLER_DIGITAL_L2)){ //tilt down
            Bongo.Wings.tiltLeftWing(-Bongo.Wings.speedMedium);
        } else { //stops left wing motor
            Bongo.Wings.tiltRightWing(0);
        }
        //right wing
        if(master.get_digital(E_CONTROLLER_DIGITAL_R1)){ //tilt up
            Bongo.Wings.tiltRightWing(Bongo.Wings.speedMedium);
        } else if(master.get_digital(E_CONTROLLER_DIGITAL_R2)){ //tilt down
            Bongo.Wings.tiltRightWing(-Bongo.Wings.speedMedium);
        } else { //stops left wing motor
            Bongo.Wings.tiltRightWing(0);
        }
        //back wing
        if(master.get_digital(E_CONTROLLER_DIGITAL_X)){ //tilt up
            Bongo.Tail.tiltTail(Bongo.Tail.speedMedium);
        } else if(master.get_digital(E_CONTROLLER_DIGITAL_B)){ //tilt down
            Bongo.Tail.tiltTail(-Bongo.Tail.speedMedium);
        } else { //stops left wing motor
            Bongo.Tail.tiltTail(0);
        }
        
        //lift control
        if(master.get_digital(E_CONTROLLER_DIGITAL_A)){
            Bongo.Intake.setSpeed(Bongo.Intake.liftSpeedHigh);
        } else {
            Bongo.Intake.setSpeed(Bongo.Intake.liftSpeedLow);
        }
        // starts the spin on motors or cuts power
        //debug
        if(master.get_digital(E_CONTROLLER_DIGITAL_A)){
            Bongo.resetOdom();
        }
        Bongo.debugPos();
        if(master.get_digital(E_CONTROLLER_DIGITAL_B)){
            Bongo.testOdom2();
        }
        if(master.get_digital(E_CONTROLLER_DIGITAL_Y)){
            Bongo.testOdom();
        }
        Bongo.Movement.move();
        //delay between updates
        delay(20);
    }
}
#endif