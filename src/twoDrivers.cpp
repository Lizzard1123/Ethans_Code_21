#if 0
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

    while (true)
    {
        //prints to screen the position and rotation of bongo
        Bongo.debugPos();

        // change teams failsafe
        if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_A))
        {
            Bongo.changeTeam();
        }

        // tyler control
        Bongo.tylerControl();
        // catie control
        //Bongo.catieControl();

        Bongo.Wings.tiltLeftWing(partner.get_analog(E_CONTROLLER_ANALOG_LEFT_X));
        Bongo.Wings.tiltRightWing(partner.get_analog(E_CONTROLLER_ANALOG_RIGHT_X));

        //back wing
        if(partner.get_digital(E_CONTROLLER_DIGITAL_UP)){ //tilt up
            Bongo.Tail.tiltTail(Bongo.Tail.speedMedium);
        } else if(partner.get_digital(E_CONTROLLER_DIGITAL_DOWN)){ //tilt down
            Bongo.Tail.tiltTail(-Bongo.Tail.speedMedium);
        } else { //stops left wing motor
            Bongo.Tail.stopAll();
        }

        //locks
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)){
            Bongo.Wings.toggleLeftLock();
        } else if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)){
            Bongo.Wings.toggleRightLock();
        } else if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
            Bongo.Tail.toggleLock();
        }

        //lift control
        if(master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)){
            Bongo.Intake.toggle();
        }
        if(partner.get_digital(E_CONTROLLER_DIGITAL_X)){
            Bongo.Intake.setSpeed(Bongo.Intake.liftSpeedHigh);
        } else {
            Bongo.Intake.setSpeed(Bongo.Intake.liftSpeedLow);
        }

        //lining up with mogo
        if(partner.get_digital(E_CONTROLLER_DIGITAL_Y)){
            Bongo.lineUpMogo(LeftSideNum);
        } else if(partner.get_digital(E_CONTROLLER_DIGITAL_B)){
            Bongo.lineUpMogo(BackSideNum);
        } else if(partner.get_digital(E_CONTROLLER_DIGITAL_A)){
            Bongo.lineUpMogo(RightSideNum);
        }

        // starts the spin on motors or cuts power
        Bongo.Movement.move();
        //delay between updates
        delay(20);
    }
}
#endif